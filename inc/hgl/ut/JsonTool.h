#pragma once
#include<string>
#include<json/json.h>
#include<hgl/type/BaseString.h>

const std::string JsonToString(const Json::Value &jv_root);

bool ParseJson(Json::Value &root,const char *str,const int size,std::string *error_info);

bool LoadJson(Json::Value &,const hgl::OSString &filename);
bool SaveJson(Json::Value &,const hgl::OSString &filename);
