#pragma once

namespace filesystem
{
    bool FileCopy(const std::string &,const std::string &);                                         ///<文件复制
    bool FileDelete(const std::string &);                                                           ///<文件删除
    bool FileMove(const std::string &,const std::string &);                                         ///<文件移动
    bool FileRename(const std::string &,const std::string &);                                       ///<文件改名
    bool FileConfirm(const std::string &);                                                          ///<文件确认是否存在

    void *LoadFileToMemory(const std::string &,unsigned int *);                                     ///<加载一个文件到内存
    unsigned int SaveMemoryToFile(const std::string &,const void *,unsigned int);                   ///<保存一块内存成文件

    bool IsDirectory(const std::string &);
    bool MakeDirectory(const std::string &);                                                        ///<创建一个子目录(不处理嵌套)
    bool MakePath(const std::string &);                                                             ///<创建一个路径(处理嵌套)
}

