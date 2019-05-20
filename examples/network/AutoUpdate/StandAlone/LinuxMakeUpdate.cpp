#include<string>
#include<cstring>
#include<iostream>
#include<fstream>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/dir.h>
#include<sys/fcntl.h>

#include"FileSystem.h"
#include"md5.h"
#include<bzlib.h>

/**
* 文件信息数据结构
*/
struct FileInfo								///文件信息
{
    char name[_POSIX_PATH_MAX];				///<文件名(不包含路径)
    char fullname[_POSIX_PATH_MAX];			///<完整名称(包含路径)

    unsigned long long size;					///<文件长度

    union
    {
        unsigned int attrib;				///<文件属性

        struct
        {
            bool is_file:1;					///<是文件
            bool is_directory:1;			///<是目录

            bool can_read:1;				///<可以读
            bool can_write:1;				///<可以写
        };
    };
};//struct FileInfo

struct EnumFileConfig;

using EnumFolderFunc=void(*)(struct EnumFileConfig *,struct EnumFileConfig *,FileInfo &fi);
using EnumFileFunc=void(*)(struct EnumFileConfig *,FileInfo &fi);
using CREATE_SUB_CONFIG=EnumFileConfig *(*)(struct EnumFileConfig *up_efc,const std::string &sub_folder_name);
EnumFileConfig *DefaultCreateSubConfig(struct EnumFileConfig *efc,const std::string &sub_folder_name);

/**
* 枚举文件配置
*/
struct EnumFileConfig
{
    std::string         folder_name;                ///<要枚举的目录名称

    bool                proc_folder;                ///<是否处理目录
    bool                proc_file;                  ///<是否处理文件
    bool                sub_folder;                 ///<是否查找子目录

    EnumFolderFunc      cb_folder;                  ///<文件夹处理回呼函数
    EnumFileFunc        cb_file;                    ///<文件处理回呼函数

    CREATE_SUB_CONFIG   CreateSubConfig;            ///<创建子目录查找配置函数

public:

    EnumFileConfig()
    {
        proc_folder=true;
        proc_file=true;
        sub_folder=false;

        cb_folder=nullptr;
        cb_file=nullptr;

        CreateSubConfig=DefaultCreateSubConfig;
    }

    EnumFileConfig(const EnumFileConfig *efc,const std::string &sub_folder_name)
    {
        folder_name =sub_folder_name;

        proc_folder =efc->proc_folder;
        proc_file   =efc->proc_file;
        sub_folder  =efc->sub_folder;

        cb_folder   =efc->cb_folder;
        cb_file     =efc->cb_file;

        CreateSubConfig=efc->CreateSubConfig;
    }
};//struct EnumFileConfig

/**
* 组合路径名与文件名
* @param fullname 完整路径文件名
* @param pathname 路径名
* @param filename 文件名
*/
void MergeFilename(std::string &fullname,const std::string &pathname,const std::string &filename)
{
    std::string::const_iterator it=pathname.cend();

    --it;

    fullname=(  *it=='/'?
                pathname+filename:
                pathname+'/'+filename);
}

EnumFileConfig *DefaultCreateSubConfig(struct EnumFileConfig *efc,const std::string &sub_folder_name)
{
    std::string full_sub_folder_name;

    MergeFilename(full_sub_folder_name,efc->folder_name,sub_folder_name);

    return(new EnumFileConfig(efc,full_sub_folder_name));
}

/**
* 枚举一个目录内的所有文件
* @param config 枚举配置
* @return 查找到文件数据,<0表示失败
*/
int EnumFile(EnumFileConfig *config)
{
    if(!config)return(-1);

//            if(config->proc_folder&&!config->cb_folder)return(-2);            //这一行是不需要的，确实存在proc_folder=true,但没有cb_folder的情况。但留在这里。以防删掉后，未来没注意自以为是的加上这样一行
    if(config->proc_file&&!config->cb_file)return(-3);

    if(config->folder_name.empty())return(-4);

    std::string fullname;
    int count=0;

    if(config->folder_name.empty())
    {
        fullname='.';
    }
    else
    {
        fullname=config->folder_name;
    }

    DIR *dir;
    struct dirent64 *entry;
    struct stat64 statbuf;
    FileInfo fi;
    char path_buf[_POSIX_PATH_MAX]={0};
    getcwd(path_buf,_POSIX_PATH_MAX);

    chdir(fullname.c_str());
    if((dir = opendir(fullname.c_str())) == nullptr)
        return(-1);
    if((entry = readdir64(dir)) == nullptr)
        return(-1);

    EnumFileConfig *sub_efc=nullptr;
    int sub_count;

    do
    {
        if(strcmp(entry->d_name,".")==0
        ||strcmp(entry->d_name,"..")==0)
        {
            continue;
        }

        memset(&statbuf,0,sizeof(struct stat64));

        if(lstat64(entry->d_name,&statbuf)==-1)
            continue;

        memset(&fi,0,sizeof(FileInfo));

        fi.size=statbuf.st_size;

        if(S_ISDIR(statbuf.st_mode))
        {
            fi.is_file=false;
            fi.is_directory=true;
        }
        else
        {
            fi.is_file=true;
            fi.is_directory=false;
        }

        fi.can_read	=statbuf.st_mode&S_IROTH;
        fi.can_write=statbuf.st_mode&S_IWOTH;

        if(S_ISDIR(statbuf.st_mode))
        {
            if(!config->proc_folder)continue;

            if(config->sub_folder)
            {
                sub_efc=config->CreateSubConfig(config,entry->d_name);

                if(!sub_efc)
                    continue;

                sub_count=EnumFile(sub_efc);
                if(sub_count>0)count+=sub_count;
            }
        }
        else
        {
            if(!config->proc_file)continue;

            ++count;
        }

        memset(&fi,0,sizeof(FileInfo));

        strcpy(fi.name,entry->d_name);

        if(config->folder_name.empty())
        {
            strcpy(fi.fullname,fi.name);
        }
        else
        {
            strcpy(fi.fullname,config->folder_name.c_str());

            std::string::const_iterator it=config->folder_name.cend();

            --it;
            if(*it!='/')
                strcat(fi.fullname,"/");

            strcat(fi.fullname,fi.name);
        }

        if(fi.is_directory)
        {
            if(config->cb_folder)
                config->cb_folder(config,sub_efc,fi);

            delete sub_efc;
            sub_efc=nullptr;
        }
        else
        {
            if(config->cb_file)
                config->cb_file(config,fi);
        }
    }
    while((entry=readdir64(dir)));

    closedir(dir);

    chdir(path_buf);
    return(count);
}

std::string cur_path;
std::ofstream xml;

void AppendFileToXML(struct EnumFileConfig *,FileInfo &fi)
{
    std::string short_filename=fi.fullname+(cur_path.length()+1);

    std::cout<<"file: "<<short_filename<<std::endl;

    unsigned int filelength;
    unsigned int compress_filelength;

    char *filedata=(char *)filesystem::LoadFileToMemory(fi.fullname,&filelength);

    if(!filedata)
    {
        std::cout<<"load file to memory failed"<<std::endl;
        return;
    }

    MD5Code md5;
    MD5Code compress_md5;
    char md5str[33];
    char cmd5str[33];

    GetMD5(md5,filedata,filelength);
    DataToHexStr(md5str,(uint8 *)&md5,sizeof(MD5Code));

    char *compress_data=new char[filelength];
    compress_filelength=filelength;

    if(BZ2_bzBuffToBuffCompress(compress_data,&compress_filelength,filedata,filelength,9,0,30)==BZ_OK)
    {
        std::string compress_filename=fi.fullname;

        compress_filename+=".bz2";

        if(filesystem::SaveMemoryToFile(compress_filename,compress_data,compress_filelength)==compress_filelength)
        {
            GetMD5(compress_md5,compress_data,compress_filelength);

            DataToHexStr(cmd5str,(uint8 *)&compress_md5,sizeof(MD5Code));

            xml<<"\t<file compress_size=\""<<compress_filelength<<"\" compress_md5=\""<<cmd5str<<"\" size=\""<<filelength<<"\" md5=\""<<md5str<<"\" name=\""<<short_filename.c_str()<<"\"/>\n";
        }
        else
        {
            std::cout<<"save compress file to <"<<compress_filename.c_str()<<"> failed!"<<std::endl;
        }
    }
    else
    {
        std::cout<<"compress data failed,use uncompress data!"<<std::endl;

        //压缩失败，使用未压缩数据

        xml<<"\t<file compress_size=\""<<filelength<<"\" compress_md5=\""<<md5str<<"\" size=\""<<filelength<<"\" md5=\""<<md5str<<"\" name=\""<<short_filename.c_str()<<"\"/>\n";
    }

    delete[] compress_data;
    delete[] filedata;
}

int main(int argc,char **argv)
{
    if(argc<2)
    {
        std::cout<<"example: LinuxMakeUpdate <dirname>"<<std::endl;
        return 0;
    }

    std::cout<<"dirname: "<<argv[1]<<std::endl;
    std::cout<<"outfile: "<<argv[1]<<".xml"<<std::endl;

    std::string xml_filename;

    char path_buf[_POSIX_PATH_MAX]={0};
    getcwd(path_buf,_POSIX_PATH_MAX);

    cur_path=path_buf;

    EnumFileConfig efc;

    xml_filename=cur_path+'/';
    xml_filename+=argv[1];

    efc.folder_name=xml_filename;
    efc.sub_folder=true;
    efc.cb_file=AppendFileToXML;

    xml_filename+=".xml";

    xml.open(xml_filename,std::ios::binary|std::ios::out|std::ios::trunc);

    if(xml.is_open())
    {
        std::cout<<"create <"<<xml_filename.c_str()<<"> ok!\n"<<std::endl;
    }
    else
    {
        std::cout<<"create <"<<xml_filename.c_str()<<"> failed!\n"<<std::endl;
        return(1);
    }

    xml<<"<update>\n";

    EnumFile(&efc);

    xml<<"</update>\n";

    return 0;
}
