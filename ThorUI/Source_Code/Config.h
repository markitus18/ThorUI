#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#include "External_Libraries\parson\parson.h"
#include <string>
#include <vector>
#include "Vec2.h"
#include "Color.h"

//http://kgabis.github.io/parson/

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

typedef unsigned int uint;

class Config_Array;
struct Vec2;
struct Color;

class Config
{
	///Brief parson explanation
	/*
	-	Save a JSON file into a buffer then create a new file with fileSystem:
	JSON_Status json_serialize_to_buffer(const JSON_Value *value, char *buf, size_t buf_size_in_bytes);
	-	JSON nodes are called objects
	-	Append attributes to object: json_object_set_[type of attribute]. Attribute types are bool, number, string and value (any attribute or an object)
	-	Get values from a node: same function as append, but with "get"
	*/

public:
	THORUI_API Config();						//Contructor used for new files
	THORUI_API Config(const char* buffer);		//Constructor used for data read
	THORUI_API Config(JSON_Object* obj);		//Constructor used for node append
	THORUI_API ~Config();						//Free data if initialized

	THORUI_API uint Serialize(char** buffer);	//Returns a filled buffer
	THORUI_API bool NodeExists();
	THORUI_API void Release();

	//Append attributes -----------
	THORUI_API void SetNumber(const char* name, double data);
	THORUI_API void SetString(const char* name, const char* data);
	THORUI_API void SetBool(const char* name, bool data);
	THORUI_API Config_Array SetArray(const char* name);
	THORUI_API Config SetNode(const char* name);
	//Endof append attributes------

	//Get attributes --------------
	THORUI_API double GetNumber(const char* name, double default = 0) const;
	THORUI_API std::string GetString(const char* name, const char* default = "") const;
	THORUI_API bool GetBool(const char* name, bool default = true) const;
	THORUI_API Config_Array GetArray(const char* name);
	THORUI_API Config GetNode(const char* name) const;
	//Endof Get attributes---------

private:
	JSON_Value* root_value = nullptr; //Only used for file root
	JSON_Object* node = nullptr;
};

class Config_Array
{
public:
	//Contructor only to be called from Config, it would cause mem leak
	THORUI_API Config_Array();
	THORUI_API Config_Array(JSON_Array* arr);

	//Append attributes ------------
	THORUI_API void AddNumber(int number);
	THORUI_API void AddString(char* string);
	THORUI_API void AddBool(bool boolean);
	THORUI_API void AddVec2(const Vec2& vec);
	THORUI_API void AddColor(const Color& color);

	THORUI_API Config AddNode();
	//Endof append attributes-------

	//Get attributes ---------------
	THORUI_API double GetNumber(uint index, double default = 0) const;
	THORUI_API const char* GetString(uint index, const char* default = "") const;
	THORUI_API bool GetBool(uint index, bool default = true) const;
	THORUI_API Vec2 GetVec2(uint index, Vec2 default = Vec2(0, 0)) const;
	THORUI_API Color GetColor(uint index, Color default = Color::White()) const;

	THORUI_API void FillVectorNumber(std::vector<double>& vector) const;
	THORUI_API void FillVectorString(std::vector<char*>& vector) const;
	THORUI_API void FillVectorBoool(std::vector<bool>& vector) const;
	THORUI_API Config GetNode(uint index) const;
	THORUI_API uint GetSize() const;
	//Endof Get attributes----------

private:
	JSON_Array* arr;
	uint size = 0;
};

#endif //__CONFIG_H__
#pragma once
