#include<string>
#include<sstream>
#include<json/json.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StdString.h>

using namespace hgl;
using namespace hgl::filesystem;
using namespace std;

const std::string JsonToString(const Json::Value &jv_root)
{
    Json::StreamWriterBuilder builder;
    Json::StreamWriter *writer=builder.newStreamWriter();

    JSONCPP_OSTRINGSTREAM result;

    writer->write(jv_root,&result);

    delete writer;

    return std::string(result.str());
}

bool ParseJson(Json::Value &root,const char *txt,const int size,std::string *error_info)
{
    Json::CharReaderBuilder builder;
    Json::CharReader *reader=builder.newCharReader();

    const bool result=reader->parse(txt,txt+size,&root,error_info);

    delete reader;

    return result;
}

bool LoadJson(Json::Value &root,const OSString &filename)
{
    char *txt;
    int size;

    size=LoadFileToMemory(filename,(void **)&txt);

    if(size<=0)
    {
        LOG_ERROR(OS_TEXT("load json file failed,filename: ")+filename);
        return(false);
    }

    bool result;

    std::string error_info;

    result=ParseJson(root,txt,size,&error_info);
    delete[] txt;

    if(!result)
    {
        LOG_ERROR(OS_TEXT("parse json file failed,filename: ")+filename);
        return(false);
    }

    return(true);
}

bool SaveJson(Json::Value &root,const OSString &filename)
{
    const std::string txt=JsonToString(root);

    return SaveMemoryToFile(filename,txt.c_str(),txt.size());
}
