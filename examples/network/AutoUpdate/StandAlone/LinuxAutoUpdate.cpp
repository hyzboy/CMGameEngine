#include<string>
#include<list>
#include<iostream>
#include"XMLParse.h"
#include"md5.h"
#include"MemoryBlock.h"
#include"FileSystem.h"
#include<bzlib.h>
#include<curl/curl.h>
#include<unistd.h>

bool QueryFileLength(const std::string &url,long &downloadFileLenth);
int Download(const std::string &,char *,size_t);

/**
* 取得程序自身路径全名
*/
bool GetSelfFullname(std::string &result)
{
    char path[_POSIX_PATH_MAX+1];

    const int len=readlink("/proc/self/exe",path,_POSIX_PATH_MAX);

    if(len<=0)
        return(false);

    path[len]=0;
    result=path;

    return(true);
}

struct FileCheck
{
    std::string filename;

    MD5Code md5;                    
    int size;
    
    MD5Code compress_md5;
    int compress_size;

    bool update;                //是否更新标记

public:

    FileCheck(const std::string &fn,const MD5Code &c,const int &s,const MD5Code &cc,const int &cs)
    {
        filename=fn;

        memcpy(&md5,&c,sizeof(MD5Code));
        size=s;

        memcpy(&compress_md5,&cc,sizeof(MD5Code));
        compress_size=cs;

        update=false;
    }
};//struct FileCheck

struct ConfigXMLParse:public XMLParse
{
    std::list<FileCheck> fc_list;

public:

    void StartElement(const char *element_name,const char **atts) override
    {
        if(strcmp(element_name,"file")==0)
        {
            std::string name;
            MD5Code md5;
            int size;
            MD5Code compress_md5;
            int compress_size;

            XML_START_PARSE(atts)

                xml_parse_string_u8(name)
                xml_parse_md5(md5)
                xml_parse_int(size)
                xml_parse_md5(compress_md5)
                xml_parse_int(compress_size)

            XML_END_PARSE

            fc_list.push_back(FileCheck(name,md5,size,compress_md5,compress_size));
        }
    }
};//struct ConfigXMLParse:public XMLParse

const std::string url_path="http://localhost/App/";             //更新服务器网址
const std::string compress_extname=".bz2";                      //压缩文件后缀

class UpdateCheck
{
    std::string root_path;

    ConfigXMLParse xml;

private:

    bool DownloadFile(FileCheck &fc,const std::string &filename,std::string &file_url)
    {
        long filesize;
            
        if(!QueryFileLength(file_url,filesize)) 
            return(false);
            
        if(filesize!=fc.compress_size)
            return(false);
                
        MemoryBlock<char> mb(filesize);

        int download_size=Download(file_url,mb,filesize);

        if(download_size!=filesize)
            return(false);

        MD5Code compress_md5;

        GetMD5(compress_md5,mb.data(),filesize);

        if(memcmp(compress_md5,fc.compress_md5,sizeof(MD5Code)))
            return(false);

        MemoryBlock<char> orign_data(fc.size);

        uint decompress_size=fc.size;
        if(BZ2_bzBuffToBuffDecompress(orign_data,&decompress_size,mb,filesize,0,0)!=BZ_OK)
            return(false);

        if(decompress_size!=fc.size)
            return(false);

        MD5Code md5;

        GetMD5(md5,orign_data.data(),decompress_size);

        if(memcmp(md5,fc.md5,sizeof(MD5Code)))
            return(false);

        if(filesystem::SaveMemoryToFile(filename,orign_data.data(),fc.size)!=fc.size)
            return(false);

        fc.update=true;

        return(true);
    }

    bool CheckPath(const std::string &filename)
    {
        std::string path=filename.substr(0,filename.find_last_of('/'));

        if(filesystem::IsDirectory(path))
            return(true);

        return filesystem::MakePath(path);
    }

    bool CheckFile(FileCheck &fc)
    {
        std::string filename;
        std::string fullname;
        std::string file_url;
        int size;

        filename=fc.filename;

        fullname=root_path+"/"+filename;
        file_url=url_path+filename+compress_extname;
        
        replace(fullname,'/','\\');
        replace(file_url,'/','\\');

        if(!filesystem::FileConfirm(fullname))                          //文件不存在，下载
        {
            if(!CheckPath(fullname))                                    //检测目录，如果不存在则创建
                return(false);

            return DownloadFile(fc,fullname,file_url);
        }

        void *data=filesystem::LoadFileToMemory(fullname,&size);

        if(!data)return(false);

        MD5Code md5;

        GetMD5(md5,data,size);

        if(memcmp(md5,fc.md5,sizeof(MD5Code)))                          //MD5不一致，下载
            return DownloadFile(fc,fullname,file_url);

        return(true);
    }

    bool CheckFileList(std::list<FileCheck> &fc_list)
    {
        std::list<FileCheck>::iterator it=fc_list.begin();

        do
        {
            if(!CheckFile(*it))
                return(false);

            ++it;
        }while(it!=fc_list.end());

        return(true);
    }

public:

    bool Check(const std::string &work_path)
    {
        root_path=work_path;

        long filesize;
        std::string url=url_path+"update.xml";
        
        if(!QueryFileLength(url,filesize))      //查询XML文件长度
            return(false);
        
        char *buffer=new char[filesize+1];
        
        int download_result=Download(url,buffer,filesize);
        
        if(download_result!=filesize)
            return(false);
        
        buffer[filesize]=0;

        xml.Start();

        bool result=xml.Parse(buffer,filesize,true);

        if(result)
        {
            std::cout<<"Parse update.xml OK!"<<std::endl;
            CheckFileList(xml.fc_list);
        }
        else
        {
            std::cout<<"Parse update.xml failed"<<std::endl;
        }

        delete[] buffer;
        return(true);
    }

    /**
    * 检测某个文件是否有更新
    */
    bool CheckFileUpdate(const std::string &filename)
    {
        std::list<FileCheck>::iterator it=xml.fc_list.begin();

        do
        {
            if(it->filename==filename)
                return it->update;

            ++it;
        }while(it!=xml.fc_list.end());

        return(false);
    }
};//class UpdateCheck

/*自动更新流程
 * 判断自身文件名是否下划线开头，如果不是。则复制一个出来。并执行带下划线的版本。
 * 带下划线版本更新完发现如果本身不带下划的文件有更新，则重动启动原本不带下划线的版本。否则执行正常流程
 */

#define DEBUG_UPDATE

bool AutoUpdate()
{
    std::string self_fullname;
    std::string self_workpath;
    std::string self_filename;

    if(!GetSelfFullname(self_fullname))return(false);   //取得自身文件名
    
    const int off=self_fullname.find_last_of('/');

    self_workpath=self_fullname.substr(0,off);
    self_filename=self_fullname.substr(off+1);

#ifndef DEBUG_UPDATE
    if(*(self_filename.cbegin())!='_')                                             //自身文件名是否下划线开头
    {
       std::string backup_self=self_workpath+"/_"+self_filename;                   //创建一个下划线开头的版本

       if(filesystem::FileConfirm(backup_self))
           if(!filesystem::FileDelete(backup_self))
           {
               std::cerr<<"delete \""<<backup_self<<"\" failed"<<std::endl;
               return(false);
           }

       if(!filesystem::FileCopy(backup_self,self_fullname))
       {
           std::cerr<<"Create backup app filename failed,filename:"<<backup_self<<std::endl;
           return(false);
       }

       execvp(backup_self.c_str(),nullptr);
       exit(0);
    }
    else
#endif//DEBUG_UPDATE
    {
        std::cout<<"Hello"<<std::endl;

        UpdateCheck uc;

        uc.Check(self_workpath);

        std::string main_filename=self_filename.substr(1);     //去掉_

        if(uc.CheckFileUpdate(main_filename))                   //主文件有被更新，，，那就重启吧
        {
            std::cout<<"need restart self"<<std::endl;

            std::string main_fullname=self_workpath+"\\"+main_filename;

            execvp(main_fullname.c_str(),nullptr);
            _exit(0);
        }
    }

    return(true);
}

int main(int,char **)
{
    return AutoUpdate();
}
