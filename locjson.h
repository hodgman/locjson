#pragma once
// This software is licensed under either the terms of the MIT license
//  or the DWTFYWPL, reproduced below.
//------------------------------------------------------------------------------
//MIT License
//------------------------------------------------------------------------------
//
//Copyright (c) 2019 Brooke Hodgman
//Permission is hereby granted, free of charge, to any person
//obtaining a copy of this software and associated documentation
//files (the "Software"), to deal in the Software without
//restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the
//Software is furnished to do so, subject to the following
//conditions:
//
//The above copyright notice and this permission notice shall be
//included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//------------------------------------------------------------------------------
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//------------------------------------------------------------------------------
// 
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
// 0. You just DO WHAT THE FUCK YOU WANT TO.
//
//------------------------------------------------------------------------------
// Purpose
//------------------------------------------------------------------------------
// I needed a simple, bare-bones, C++ JSON parser with no dependencies.
// This parser is written to be small and simple (LOC-JSON stands for lines of 
// code), so it's likely slower than other parsers and has a coarser API.
// 
// This library is also designed to be replacable. The `migration` directory
// contains:
//   locjson_to_cpprest.h
//   locjson_to_rapidjson.h
// Which implement the same API as this file, but simply convert your calls to
// the cpprest or japidjson libraries.
// If you write code that uses this locjson, you can easily swap this file out
// for one of the above files later to switch to a better JSON parser.
//
//------------------------------------------------------------------------------
// Limitations
//------------------------------------------------------------------------------
// The parser should handle any valid JSON document. However, there is no API to
// *retrieve* fractional/floating point numbers, booleans, or null.
//
// Error reporting will return a flag (or throw an exception if you opt-in) when
// invalid JSON is detected, but there's no line number reporting or anything 
// helpful like that.
//
// The JSON builder supports a single, non-nested object containing string keys,
// string values, or array values containing strings or numbers.
//
//------------------------------------------------------------------------------
// API configuration
//------------------------------------------------------------------------------
// As different C++ projects may use different primitive types, you can use the 
//  following #defines (BEFORE including locjson.h) to override the default types.
//
// #define               | typedef               | Default           | Notes
// LOCJSON_INT32         | locjson::Int32        | int32_t           |
// LOCJSON_UINT32        | locjson::UInt32       | uint32_t          |
// LOCJSON_CHAR          | locjson::Char         | char              | See Unicode section
// LOCJSON_STRING        | locjson::String       | std::string       | Must support construction from `const LOCJSON_CHAR*`
// LOCJSON_STRING_VIEW   | locjson::StringView   | std::string_view* | *If C++17 is available, else std::string
// LOCJSON_STRING_STREAM | locjson::StringStream | std::stringstream | 
//
// The behavior of this header can further be modified by using the following 
//  #defines (BEFORE including locjson.h)
// 
// #define                    | Notes                   
// LOCJSON_EXCEPTION(message) | See Exceptions section
// LOCJSON_LITERAL(x)         | See Unicode section
// LOCJSON_FUNCTION           | See Integration section         
// LOCJSON_IMPLEMENTATION     | See Integration section
//
//------------------------------------------------------------------------------
// Integration
//------------------------------------------------------------------------------
// This API is provided in the "single header" style to support simple and flexible
//  integration into your project (see https://github.com/nothings/single_file_libs).
//  The implementation of function bodies will be excluded unless you define
//  LOCJSON_IMPLEMENTATION before including locjson.h.
//
// Typical linking:
//  In one CPP file, before including locjson.h:
//   #define LOCJSON_IMPLEMENTATION
// 
// Inline linking:
//  In every CPP file that uses the API, before including locjson.h:
//   #define LOCJSON_IMPLEMENTATION
//   #define LOCJSON_FUNCTION inline
//
// Aside from LOCJSON_IMPLEMENTATION / LOCJSON_FUNCTION, you should take care
//  to ensure that every other LOCJSON_* macro is defined to the same value in
//  all of yoru CPP files that use the locjson API.
//
//------------------------------------------------------------------------------
// Exceptions
//------------------------------------------------------------------------------
// Support for C++ exceptions is opt-in. Define the following (or an alternative
//  based on your own exception classes) before including locjson.h:
// #define LOCJSON_EXCEPTION(message) throw std::runtime_error(message)
//
//------------------------------------------------------------------------------
// Unicode
//------------------------------------------------------------------------------
// To build a wide-character version of the API, define the following before
//  including locjson.h:
// #defien LOCJSON_WIDE
//
//------------------------------------------------------------------------------
// C++17
//------------------------------------------------------------------------------
// When built on a C++17 compiler, the parser will not perform any memory 
// allocations. On earlier compilers, the parser will perform a lot of temporary
// memory allocations...
//
// Microsoft Visual Studio reports the wrong value for __cplusplus by default
// for reasons. To enable C++17 suppport in MSVC, you need to add these compiler
// switches:
//  /Zc:__cplusplus /std:c++17
// OR:
//  /Zc:__cplusplus /std:c++latest
//------------------------------------------------------------------------------

#if !defined(LOCJSON_STRING)
#include <string>
#endif
#if !defined(LOCJSON_STRING_VIEW) && __cplusplus > 201402L
#include <string_view>
#endif
#if !defined(LOCJSON_STRING_STREAM)
#include <sstream>
#endif

#if !defined(LOCJSON_INT32) || !defined(LOCJSON_UINT32)
#include <cstdint>
#endif

#if !defined(LOCJSON_EXCEPTION)
#define LOCJSON_EXCEPTION(literal)
#endif

#if !defined(LOCJSON_FUNCTION)
#define LOCJSON_FUNCTION
#endif

#if !defined(LOCJSON_LITERAL)
# if defined(LOCJSON_WIDE)
#  define LOCJSON_LITERAL(x) L ## x
# else
#  define LOCJSON_LITERAL(x) x
# endif
#endif

#if !defined(LOCJSON_STRLEN)
# if defined(LOCJSON_WIDE)
#  define LOCJSON_STRLEN wcslen
# else
#  define LOCJSON_STRLEN strlen
# endif
#endif

#if !defined(LOCJSON_STRTOL)
# if defined(LOCJSON_WIDE)
#  define LOCJSON_STRTOL std::wcstol
# else
#  define LOCJSON_STRTOL std::strtol
# endif
#endif

namespace locjson
{
	#ifdef LOCJSON_CHAR
	typedef LOCJSON_CHAR Char;
	#elif defined(LOCJSON_WIDE)
	typedef wchar_t Char;
	#else
	typedef char Char;
	#endif

	#ifdef LOCJSON_INT32
	typedef LOCJSON_INT32 Int32;
	#else
	typedef int32_t Int32;
	#endif

	#ifdef LOCJSON_UINT32
	typedef LOCJSON_UINT32 UInt32;
	#else
	typedef uint32_t UInt32;
	#endif

	#ifdef LOCJSON_STRING
	typedef LOCJSON_STRING String;
	#elif defined(LOCJSON_WIDE)
	typedef std::wstring String;
	#else
	typedef std::string String;
	#endif

	#ifdef LOCJSON_STRING_VIEW
	typedef LOCJSON_STRING_VIEW StringView;
	#elif __cplusplus > 201402L
	# if defined(LOCJSON_WIDE)
	typedef std::wstring_view StringView;
	# else
	typedef std::string_view StringView;
	# endif
	#else
	typedef String StringView;
	#endif

	#ifdef LOCJSON_STRING_STREAM
	typedef LOCJSON_STRING_STREAM StringStream;
	#elif defined(LOCJSON_WIDE)
	typedef std::wstringstream StringStream;
	#else
	typedef std::stringstream StringStream;
	#endif

	typedef StringView JSONValue;
	typedef StringView JSONArray;
	typedef String     JSONDocument;

	struct JSONBuilder
	{
		StringStream s;
		bool empty = true;
		operator StringStream&() { return s; }
	};

    JSONValue Parse(const JSONDocument&);
	Int32     LookupInt32(const JSONValue&, const Char* field, bool& out_error);
	UInt32    LookupUInt32(const JSONValue&, const Char* field, bool& out_error);
	String    LookupString(const JSONValue&, const Char* field, bool& out_error);
	JSONValue LookupValue(const JSONValue&, const Char* field, bool& out_error);
	JSONArray LookupArray(const JSONValue&, const Char* field, bool& out_error);
	bool      HasField(const JSONValue&, const Char* field, bool& out_error);
	bool      HasArrayField(const JSONValue&, const Char* field, bool& out_error);
	Int32     AsInt32(const JSONValue&, bool& out_error);
	UInt32    AsUInt32(const JSONValue&, bool& out_error);
	String    AsString(const JSONValue&, bool& out_error);
	JSONArray AsArray(const JSONValue&, bool& out_error);
	bool      IsArray(const JSONValue&, bool& out_error);
	bool      IsObject(const JSONValue&, bool& out_error);

	int       ArraySize(const JSONArray&, bool& out_error);
	JSONValue IndexArray(const JSONArray&, int index, bool& out_error);

	void BeginObject(JSONBuilder&);
	void AddString(JSONBuilder&, const Char* key, const Char* value);
	template<class... Args>
	void AddArray(JSONBuilder&, const Char* key, Args... args);
	void EndObject(JSONBuilder&);
}

#if defined(LOCJSON_IMPLEMENTATION)
namespace locjson
{
    JSONValue Parse(const JSONDocument& doc) { return doc; }

	inline size_t SkipNumber(const JSONValue& v, size_t i, bool& out_error)
	{
		size_t j = v.find_first_not_of(LOCJSON_LITERAL("+-0123456789.eE"), i);
		if( i==j ) { LOCJSON_EXCEPTION("Invalid Number"); out_error = true; return i+1; }
		return j;
	}
	inline size_t SkipString(const JSONValue& v, size_t i, bool& out_error)
	{
		if( v[i] != '"' ) { LOCJSON_EXCEPTION("Invalid String"); out_error = true; return i+1; }
		for(++i; i<v.size();)
		{
			i =  v.find_first_of(LOCJSON_LITERAL("\"\\"), i);
			if( i == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated String"); out_error = true; return i; }
			if( v[i] == '"' ) { break; }
			if( i+1 < v.size() )
			{
				switch(v[i+1])
				{
				case '\\': case 'b': case 'f': case 'n': case 'r': case 't': case '"': i = i+2; continue;
				case 'u': i = i+6; continue;
				}
			}
			LOCJSON_EXCEPTION("Invalid Escape sequence"); out_error = true; ++i;
		}
		return i == JSONValue::npos ? i : i+1;
	}
	inline size_t SkipBoolNull(const JSONValue& v, size_t i, bool& out_error)
	{
		switch(v[i])
		{
		case 'f': if(v.compare(i, 5, LOCJSON_LITERAL("false"))) { break; } return i+5;
		case 't': if(v.compare(i, 4, LOCJSON_LITERAL("true")))  { break; } return i+4;
		case 'n': if(v.compare(i, 4, LOCJSON_LITERAL("null")))  { break; } return i+4;
		}
		LOCJSON_EXCEPTION("Invalid Value"); out_error = true; return i+1;
	}
	inline size_t SkipObject(const JSONValue& v, size_t i, bool& out_error);
	inline size_t SkipArray(const JSONValue& v, size_t i, bool& out_error)
	{
		if( v[i] != '[' ) { LOCJSON_EXCEPTION("Invalid Array"); out_error = true; return i+1; }
		++i;
		for(; i<v.size();)
		{
			i = v.find_first_not_of(LOCJSON_LITERAL(", \t\r\n\f\b"), i);
			if( i == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated array"); out_error = true; return i; }
			switch( v[i] )
			{
			case ']': return i+1;
			case '{': i = SkipObject(v, i, out_error); break;
			case '[': i = SkipArray(v, i, out_error); break;
			case '"': i = SkipString(v, i, out_error); break;
			case 't': case 'f': case 'n': i = SkipBoolNull(v, i, out_error); break;
			default:  i = SkipNumber(v, i, out_error); break;
			}
		}
		LOCJSON_EXCEPTION("Unterminated array");
		out_error = true;
		return i == JSONValue::npos ? i : i+1;
	}
	inline size_t SkipObject(const JSONValue& v, size_t i, bool& out_error)
	{
		if( v[i] != '{' ) { LOCJSON_EXCEPTION("Invalid object"); out_error = true; return i+1; }
		for(; i<v.size();)
		{
			size_t keyBegin = v.find_first_of(LOCJSON_LITERAL("\"}"), i);
			if( keyBegin == JSONValue::npos || v[keyBegin] == '}' ) { break; }//no more keys
			size_t keyEnd = v.find_first_of('"', keyBegin+1);
			if( keyEnd == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated string"); out_error = true; break; }
			size_t valueBegin = v.find_first_not_of(LOCJSON_LITERAL(" \t\r\n\f\b:"), keyEnd+1);
			if( valueBegin == JSONValue::npos ) { LOCJSON_EXCEPTION("No value following object key"); out_error = true; break; }
			Char value0 = v[valueBegin];
			switch(value0)
			{
			case '{': i = SkipObject(v, valueBegin, out_error); break;
			case '[': i = SkipArray(v, valueBegin, out_error); break;
			case '"': i = SkipString(v, valueBegin, out_error); break;
			case 't': case 'f': case 'n': i = SkipBoolNull(v, i, out_error); break;
			default:  i = SkipNumber(v, valueBegin, out_error); break;
			}
		}
		return i == JSONValue::npos ? i : i+1;
	}

	LOCJSON_FUNCTION Int32 LookupInt32(const JSONValue& v, const Char* field, bool& out_error)     { return AsInt32( LookupValue(v, field, out_error), out_error); }
	LOCJSON_FUNCTION UInt32 LookupUInt32(const JSONValue& v, const Char* field, bool& out_error)   { return AsUInt32(LookupValue(v, field, out_error), out_error); }
	LOCJSON_FUNCTION String LookupString(const JSONValue& v, const Char* field, bool& out_error)   { return AsString(LookupValue(v, field, out_error), out_error); }
	LOCJSON_FUNCTION JSONArray LookupArray(const JSONValue& v, const Char* field, bool& out_error) { return AsArray(LookupValue(v, field, out_error), out_error); }
	LOCJSON_FUNCTION bool HasField(const JSONValue& v, const Char* field, bool& out_error)         { return LOCJSON_LITERAL("") != LookupValue(v, field, out_error); }
	LOCJSON_FUNCTION bool HasArrayField(const JSONValue& v, const Char* field, bool& out_error)    { return IsArray(LookupValue(v, field, out_error), out_error); }
	LOCJSON_FUNCTION JSONValue LookupValue(const JSONValue& v, const Char* field, bool& out_error)
	{
		if( v.length() < 1 || v[0] != '{' ) { out_error = true; return LOCJSON_LITERAL(""); }
		size_t fieldLen = LOCJSON_STRLEN(field);
		for(size_t i=0; i<v.size();)
		{
			size_t keyBegin = v.find_first_of(LOCJSON_LITERAL("\"}"), i);
			if( keyBegin == JSONValue::npos || v[keyBegin] == '}' ) { break; }//no more keys
			++keyBegin;
			size_t keyEnd = v.find_first_of('"', keyBegin);
			if( keyEnd == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated string"); out_error = true; break; }
			size_t valueBegin = v.find_first_not_of(LOCJSON_LITERAL(" \t\r\n\f\b:"), keyEnd+1);
			if( valueBegin == JSONValue::npos ) { LOCJSON_EXCEPTION("No value following object key"); out_error = true; break; }
			size_t keyLen = keyEnd-keyBegin;
			bool correctKey = fieldLen == keyLen && 0==v.compare(keyBegin, keyLen, field);
			if( correctKey )
				return v.substr(valueBegin);
			Char value0 = v[valueBegin];
			switch(value0)
			{
			case '{': i = SkipObject(v, valueBegin, out_error); break;
			case '[': i = SkipArray( v, valueBegin, out_error); break;
			case '"': i = SkipString(v, valueBegin, out_error); break;
			case 't': case 'f': case 'n': i = SkipBoolNull(v, valueBegin, out_error); break;
			default:  i = SkipNumber(v, valueBegin, out_error); break;
			}
		}
		return LOCJSON_LITERAL("");
	}
	LOCJSON_FUNCTION Int32  AsInt32(const JSONValue& v, bool& out_error)
	{
		const Char* numeric = LOCJSON_LITERAL("-0123456789");
		size_t begin = v.find_first_of(numeric, 0);
		if( begin != 0 ) { LOCJSON_EXCEPTION("Invalid number"); out_error = true; return 0; }
		size_t pos = v.find_first_not_of(numeric, 0);
		if( pos == 0 ) { LOCJSON_EXCEPTION("Invalid number"); out_error = true; return 0; }
		JSONValue n = pos == JSONValue::npos ? v : v.substr(0, pos);
		return (Int32)LOCJSON_STRTOL(n.data(), 0, 10);
	}
	LOCJSON_FUNCTION UInt32 AsUInt32(const JSONValue& v, bool& out_error) { return (UInt32)AsInt32(v, out_error); }
	LOCJSON_FUNCTION String AsString(const JSONValue& v, bool& out_error)
	{
		if( v.length() < 1 || v[0] != '"' ) { LOCJSON_EXCEPTION("Casting non-string value to string"); out_error = true; return String(LOCJSON_LITERAL("")); }
		size_t pos = v.find('"', 1);
		if( pos == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated string"); out_error = true; return String(LOCJSON_LITERAL("")); }
		return String(v.substr(1, pos-1));
	}
	LOCJSON_FUNCTION JSONArray AsArray(const JSONValue& v, bool& out_error) { if(!IsArray(v, out_error)) { LOCJSON_EXCEPTION("Casting non-array value to array"); out_error = true; } return v; }
	LOCJSON_FUNCTION bool IsArray(const JSONValue& v, bool& out_error)
	{
		if( v.length() < 1 ) { out_error = true; return false; }
		return v[0] == '[';
	}
	LOCJSON_FUNCTION bool IsObject(const JSONValue& v, bool& out_error)
	{
		if( v.length() < 1 ) { out_error = true; return false; }
		return v[0] == '{';
	}

	LOCJSON_FUNCTION int ArraySize(const JSONArray& v, bool& out_error)
	{
		if( v[0] != '[' ) { LOCJSON_EXCEPTION("Invalid Array"); out_error = true; return 0; }
		int count = 0;
		for(size_t i=1; i<v.size();)
		{
			i = v.find_first_not_of(LOCJSON_LITERAL(", \t\r\n\f\b"), i);
			if( i == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated array"); out_error = true; return count; }
			switch( v[i] )
			{
			case ']': return count;
			case '{': i = SkipObject(v, i, out_error); count++; break;
			case '[': i = SkipArray(v, i, out_error);  count++; break;
			case '"': i = SkipString(v, i, out_error); count++; break;
			case 't': case 'f': case 'n': i = SkipBoolNull(v, i, out_error); break;
			default:  i = SkipNumber(v, i, out_error); count++; break;
			}
		}
		return count;
	}
	LOCJSON_FUNCTION JSONValue IndexArray(const JSONArray& v, int index, bool& out_error)
	{
		if( v[0] != '[' ) { LOCJSON_EXCEPTION("Invalid Array"); out_error = true; return LOCJSON_LITERAL(""); }
		int count = 0;
		for(size_t i=1; i<v.size();)
		{
			i = v.find_first_not_of(LOCJSON_LITERAL(", \t\r\n\f\b"), i);
			if( i == JSONValue::npos ) { LOCJSON_EXCEPTION("Unterminated array"); out_error = true; return LOCJSON_LITERAL(""); }
			if( count == index )
				return v.substr(i);
			switch( v[i] )
			{
			case ']': goto err;
			case '{': i = SkipObject(v, i, out_error); count++; break;
			case '[': i = SkipArray(v, i, out_error);  count++; break;
			case '"': i = SkipString(v, i, out_error); count++; break;
			case 't': case 'f': case 'n': i = SkipBoolNull(v, i, out_error); break;
			default:  i = SkipNumber(v, i, out_error); count++; break;
			}
		}
	err:
		LOCJSON_EXCEPTION("Array index out of bounds"); 
		out_error = true;
		return LOCJSON_LITERAL("");
	}

	LOCJSON_FUNCTION                 void AddKey(JSONBuilder& b, const Char* key)                       { if(!b.empty) { b.s << LOCJSON_LITERAL(", "); } b.empty = false; b.s << '"' << key << LOCJSON_LITERAL("\": "); }
	LOCJSON_FUNCTION                 void BeginObject(JSONBuilder& b)                                   { b.s << '{'; }
	LOCJSON_FUNCTION                 void EndObject(JSONBuilder& b)                                     { b.s << '}'; }
	LOCJSON_FUNCTION                 void AddValues(JSONBuilder& b)                                     {}
	LOCJSON_FUNCTION                 void AddValues(JSONBuilder& b, const char* arg)                    { b.s << '"' << arg << '"'; }
	template<class T>                void AddValues(JSONBuilder& b, T arg)                              { b.s << arg; }
	template<class T, class... Args> void AddValues(JSONBuilder& b, T arg0, Args... args)               { AddValues(b, arg0); b.s << LOCJSON_LITERAL(", "); AddValues(b.args...); }
	LOCJSON_FUNCTION                 void AddString(JSONBuilder& b, const Char* key, const Char* value) { AddKey(b, key); b.s << '"' << value << '"'; }
	template<class... Args>          void AddArray(JSONBuilder& b, const Char* key, Args... args)       { AddKey(b, key); b.s << '['; AddValues(b, args...); b.s << ']'; }
}
#endif
