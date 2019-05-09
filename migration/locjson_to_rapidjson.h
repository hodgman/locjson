#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace locjson {

	typedef const rapidjson::Value& JSONValue;
	typedef       rapidjson::Value  JSONArray;
	typedef     rapidjson::Document JSONDocument;
	struct JSONBuilder 
	{
		rapidjson::StringBuffer buf;
		rapidjson::Writer<rapidjson::StringBuffer> w;
		JSONBuilder() : w(buf) {}
	};

	inline const rapidjson::Value& Parse(const rapidjson::Document& d) { return d; }
	inline int32_t                 LookupInt32(  const rapidjson::Value& v, const char* field, bool& out_error) { return v[field].GetInt(); }
	inline uint32_t                LookupUInt32( const rapidjson::Value& v, const char* field, bool& out_error) { return (uint32_t)v[field].GetInt(); }
	inline PHANTASMA_STRING        LookupString( const rapidjson::Value& v, const char* field, bool& out_error) { return (PHANTASMA_STRING)(v[field].GetString()); }
	inline const rapidjson::Value& LookupValue(  const rapidjson::Value& v, const char* field, bool& out_error) { return v[field]; }
	inline const rapidjson::Value& LookupArray(  const rapidjson::Value& v, const char* field, bool& out_error) { return v[field]; }
	inline bool                    HasField(     const rapidjson::Value& v, const char* field, bool& out_error) { return v.HasMember(field); }
	inline bool                    HasArrayField(const rapidjson::Value& v, const char* field, bool& out_error) { return v[field].IsArray(); }
	inline int32_t                 AsInt32(      const rapidjson::Value& v,                    bool& out_error) { return v.GetInt(); }
	inline uint32_t                AsUInt32(     const rapidjson::Value& v,                    bool& out_error) { return (uint32_t)v.GetInt(); }
	inline PHANTASMA_STRING        AsString(     const rapidjson::Value& v,                    bool& out_error) { return (PHANTASMA_STRING)(v.GetString()); }
	inline const rapidjson::Value& AsArray(      const rapidjson::Value& v,                    bool& out_error) { return v; }
	inline bool                    IsArray(      const rapidjson::Value& v,                    bool& out_error) { return v.IsArray(); }
	inline bool                    IsObject(     const rapidjson::Value& v,                    bool& out_error) { return v.IsObject(); }

	inline int                     ArraySize( const rapidjson::Value& v,            bool& out_error) { return v.Size(); }
	inline const rapidjson::Value& IndexArray(const rapidjson::Value& v, int index, bool& out_error) { return v[index]; }

	typedef JSONBuilder JSONBuilder;
	inline void BeginObject(JSONBuilder& b)                                   { b.w.StartObject(); }
	inline void EndObject(JSONBuilder& b)                                     { b.w.EndObject();}
	inline void AddString(JSONBuilder& b, const char* key, const char* value) { b.w.String(key); b.w.String(value); }
	inline void AddValues(JSONBuilder& ar)                                    {}
	inline void AddValues(JSONBuilder& b, const char* arg)                    { b.w.String(arg); }
	inline void AddValues(JSONBuilder& b, int32_t arg)                        { b.w.Int(arg); }
	inline void AddValues(JSONBuilder& b, uint32_t arg)                       { b.w.Int(arg); }
	template<class T, class... Args> void AddValues(JSONBuilder& b, T arg0, Args... args) 
	{
		AddValues(b, arg0);
		AddValues(b, args...);
	}
	template<class... Args> void AddArray(JSONBuilder& b, const char* key, Args... args)
	{
		b.w.String(key);
		b.w.StartArray();
		AddValues(b, args...);
		b.w.EndArray();
	}
}
