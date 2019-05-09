#pragma once

#include <cpprest/json.h>
#include <cstdint>
#include <locale>
#include <codecvt>

namespace locjson {
	typedef web::json::value JSONValue;
	typedef web::json::array JSONArray;
	typedef web::json::value JSONDocument;
	typedef web::json::value JSONBuilder;

	inline web::json::value Parse(const web::json::value& d) { return d; }

	inline int32_t          LookupInt32(  const web::json::value& v, const wchar_t* field, bool& out_error) { return v.at(field).as_integer(); }
	inline uint32_t         LookupUInt32( const web::json::value& v, const wchar_t* field, bool& out_error) { return (uint32_t)v.at(field).as_integer(); }
	inline std::wstring     LookupString( const web::json::value& v, const wchar_t* field, bool& out_error) { return v.at(field).as_string(); }
	inline web::json::value LookupValue(  const web::json::value& v, const wchar_t* field, bool& out_error) { return v.at(field); }
	inline web::json::array LookupArray(  const web::json::value& v, const wchar_t* field, bool& out_error) { return v.at(field).as_array(); }
	inline bool             HasField(     const web::json::value& v, const wchar_t* field, bool& out_error) { return v.has_field(field); }
	inline bool             HasArrayField(const web::json::value& v, const wchar_t* field, bool& out_error) { return v.has_array_field(field); }
	inline int32_t          AsInt32(      const web::json::value& v,                       bool& out_error) { return v.as_integer(); }
	inline uint32_t         AsUInt32(     const web::json::value& v,                       bool& out_error) { return (uint32_t)v.as_integer(); }
	inline std::wstring     AsString(     const web::json::value& v,                       bool& out_error) { return v.as_string(); }
	inline web::json::array AsArray(      const web::json::value& v,                       bool& out_error) { return v.as_array(); }
	inline bool             IsArray(      const web::json::value& v,                       bool& out_error) { return v.is_array(); }
	inline bool             IsObject(     const web::json::value& v,                       bool& out_error) { return v.is_object(); }
	inline int              ArraySize(    const web::json::array& a,                       bool& out_error) { return (int)a.size(); }
	inline web::json::value IndexArray(   const web::json::array& a, int index,            bool& out_error) { return a.at((size_t)index); }

	inline void BeginObject(web::json::value&) {}
	inline void EndObject(web::json::value&){}
	inline void AddString(web::json::value& root, const wchar_t* key, const wchar_t* value) { root[key] = web::json::value::string(value); }
	inline void AddValues(int idx, web::json::value& ar)                                    {}
	inline void AddValues(int idx, web::json::value& ar, const wchar_t* arg)                { ar[idx] = web::json::value::string(arg); }
	template<class T> void AddValues(int idx, web::json::value& ar, T arg)                  { ar[idx] = web::json::value::number(arg); }
	template<class T, class... Args> void AddValues(int idx, web::json::value& ar, T arg0, Args... args) 
	{
		AddValues(idx, ar, arg0);
		AddValues(idx+1, ar, args...);
	}
	template<class... Args> void AddArray(web::json::value& root, const wchar_t* key, Args... args)
	{
		web::json::value& ar = (root[key] = web::json::value::array());
		AddValues(0, ar, args...);
	}
}

