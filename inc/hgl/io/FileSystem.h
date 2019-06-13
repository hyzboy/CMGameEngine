#ifndef HGL_FILE_SYSTEM_INCLUDE
#define HGL_FILE_SYSTEM_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/List.h>
namespace hgl
{
    namespace io
    {
        class InputStream;
    }//namespace io

    namespace filesystem
    {
        template<typename T>
        inline BaseString<T> MergeFilename(const BaseString<T> &pathname,const BaseString<T> &filename,const T directory_separator_char,const T *directory_separator_str)
        {
            BaseString<T> fullname;

            if(pathname.GetEndChar()==directory_separator_char)                 //结尾有分隔符
            {
                if(filename.GetBeginChar()==directory_separator_char)           //开头有分隔符
                {
                    fullname.Set(pathname.c_str(),pathname.Length()-1);         //少取一个字符
                }
                else
                {
                    fullname=pathname;
                }
            }
            else                                                                //结尾没有分隔符
            {
                fullname=pathname;

                if(filename.GetBeginChar()!=directory_separator_char)           //开头没有分隔符
                {
                    fullname.Strcat(directory_separator_str);                   //添加分隔符
                }
            }

            fullname.Strcat(filename);
            return fullname;
        }

        /**
         * 截取完整路径中的文件名
         * @param filename 文件名
         * @param fullname 完整路径文件名
         */
        template<typename T>
        inline BaseString<T> ClipFilename(const BaseString<T> &fullname)
        {
            if(fullname.Length()<=1)
                return(BaseString<T>());

            const T spear_char[] = { '/','\\' };

            const int pos=fullname.FindRightChar(spear_char);

            if(pos==-1)
                return BaseString<T>(fullname);

            return BaseString<T>();
        }

        /**
         * 截取路径最后一个名字
         */
        template<typename T>
        inline BaseString<T> ClipLastPathname(const BaseString<T> &fullname)
        {
            if(fullname.Length()<=1)
                return(BaseString<T>());

            const T gap_char[2]={'\\','/'};

            T *p=nullptr;

            T *s=fullname.c_str();
            T *e=fullname.c_str()+fullname.Length()-1;

            while(e>s)
            {
                if(!p)
                {
                    if(*e==gap_char[0]||*e==gap_char[1])
                    {
                        --e;
                        continue;
                    }

                    p=e;
                    --e;
                }
                else
                {
                    if(*e==gap_char[0]||*e==gap_char[1])
                    {
                        return BaseString<T>(e+1,p-e);
                    }

                    --e;
                }
            }

            return(BaseString<T>());
        }

        inline UTF8String MergeFilename(const UTF8String &pathname,const UTF8String &filename)          ///<组合路径名与文件名
        {return MergeFilename<char>(pathname,filename,HGL_DIRECTORY_SEPARATOR,HGL_DIRECTORY_SEPARATOR_U8STR);}

        inline WideString MergeFilename(const WideString &pathname,const WideString &filename)          ///<组合路径名与文件名
        {return MergeFilename<wchar_t>(pathname,filename,L'\\',L"\\");}

        bool FileCopy(const OSString &,const OSString &);                                               ///<文件复制
        bool FileDelete(const OSString &);                                                              ///<文件删除
        bool FileMove(const OSString &,const OSString &);                                               ///<文件移动
        bool FileRename(const OSString &,const OSString &);                                             ///<文件改名
        bool FileExist(const OSString &);                                                               ///<文件确认是否存在
        bool FileComp(const OSString &,const OSString &);                                               ///<文件比较

        bool FileCanRead(const OSString &);                                                             ///<检测文件是否可读
        bool FileCanWrite(const OSString &);                                                            ///<检测文件是否可写
        bool FileCanExec(const OSString &);                                                             ///<检测文件是否可执行

        int64 LoadFileToMemory(const OSString &,void **);                                               ///<加载一个文件到内存
        int64 SaveMemoryToFile(const OSString &,const void *,const int64 &);                            ///<保存一块内存成文件
        int64 SaveMemoryToFile(const OSString &,void **,const int64 *,const int &);                     ///<保存多块内存成一个文件

        void *LoadFileToMemory(const OSString &,int64,void *buf,int64);                                 ///<加载一个文件的一部分到内存
        bool SaveMemoryToFile(const OSString &,int64,const void *,int64);                               ///<保存一块内存到一个文件

        bool IsDirectory(const os_char *);
        inline  bool IsDirectory(const OSString &str){return IsDirectory(str.c_str());}                 ///<判断这个名称是否是目录

#if HGL_OS != HGL_OS_Windows
        bool IsLink(const os_char *);                                                                   ///<判断这个名称是否是链接
#endif//

        bool MakePath(const OSString &);                                                                ///<创建一个路径
        bool DeletePath(const OSString &);                                                              ///<删除一个路径
        void DeleteTree(const OSString &);                                                              ///<删除一个路径(包含所有文件)

        bool GetCurrentPath(OSString &);                                                                ///<取得当前路径
        bool GetCurrentProgram(OSString &);                                                             ///<取得当前程序全路径名称
        bool GetCurrentProgramPath(OSString &);                                                         ///<取得当前程序所在路径
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH]);                                             ///<取得当前用户应用程序数据存放路径

        //使用int64而不是__int64是因为不是所有编译器都支持__int64的写法，必须使用DataType.H中引入的定义

        /**
         * 文件信息数据结构
         */
        struct FileInfo                             ///文件信息
        {
            os_char name[HGL_MAX_PATH];             ///<文件名(不包含路径)
            os_char fullname[HGL_MAX_PATH];         ///<完整名称(包含路径)

            uint64 size;                            ///<文件长度

            union
            {
                uint32 attrib;                      ///<文件属性

                struct
                {
                    bool is_file:1;                 ///<是文件
                    bool is_directory:1;            ///<是目录

                    bool is_hiddle:1;               ///<是否隐藏文件

#if HGL_OS != HGL_OS_Windows
                    bool is_link:1;                 ///<是否是链接
#endif//HGL_OS != HGL_OS_Windows

                    bool can_read:1;                ///<可以读
                    bool can_write:1;               ///<可以写
                };
            };

            uint64 mtime;                           ///<最后修改日期(这个值在win/unix下不通用)
        };//struct FileInfo

        bool GetFileInfo(const os_char *filename,struct FileInfo &);    ///<取得文件信息

        int GetFileInfoList(List<FileInfo> &, const OSString &folder_name, bool proc_folder, bool proc_file, bool sub_folder);

        /**
         * 卷信息数据结构
         */
        struct VolumeInfo
        {
            enum DriverType
            {
                dtNone=0,               //未知类型

                dtRemovable,            //可移动设备
                dtFixed,                //固定设备
                dtRemote,               //远程设备
                dtCDROM,                //光盘驱动器
                dtRamDisk,              //内存虚拟设备

                dtEnd                   //结束定义
            };

            u16char             name[1024];         //卷名称

            u16char             path[1024];         //卷所对应的路径名(注意:不是所有卷都有对应路径)

            DriverType          driver_type;        //驱动器类型(注意:不是所有的卷都对应驱动器)

            uint32              serial;             //卷序列号

            u16char             volume_label[256];  //卷标名称

            u16char             file_system[256];   //文件系统名称

            uint32              filename_max_length;//文件名最大长度

            bool                unicode;            //文件名支持UNICODE

            uint64              available_space;    //有效容量
            uint64              total_space;        //总空量
            uint64              free_space;         //自由容量
        };//struct VolumeInfo

        /**
         * 枚举当前计算机所有卷
         * @param data 用户自定义回传信息
         * @param func 回调函数
         * @param check_removable 检测可移除设备
         * @param check_remote 检测远程驱动器
         * @param check_cd 检测光盘
         * @return 查找到的卷数量，-1表示失败
         */
        //  int EnumVolume(void *data,void (*func)(void *,hgl::VolumeInfo &),bool check_removable=false,bool check_remote=false,bool check_cd=false);
    }//namespace filesystem
}//namespace hgl
#endif//HGL_FILE_SYSTEM_INCLUDE
