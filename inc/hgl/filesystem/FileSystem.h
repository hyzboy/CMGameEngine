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

            return fullname.SubString(pos+1);
        }

        /**
         * 截取一个文件名中的主名称(不能带路径)
         * @param filename 文件名
         * @param split_char 扩展名分隔符,一般为'.'
         */
        template<typename T>
        inline BaseString<T> ClipFileMainname(const BaseString<T> &filename,const T split_char='.')
        {
            if(filename.Length()<=1)
                return(BaseString<T>());

            const int pos=filename.FindRightChar(split_char);

            if(pos==-1)
                return BaseString<T>(filename);

            return filename.SubString(0,pos);
        }

        /**
         * 截取完整文件名中的扩展名
         * @param fullname 完整文件名
         * @param include_dot 是否包括点
         */
        template<typename T>
        inline BaseString<T> ClipFileExtName(const BaseString<T> &fullname,bool include_dot=true)
        {
            int end=fullname.FindChar(T('?'));         //url的文件名，以?为结束

            if(end==-1)
                end=fullname.Length();

            int pos=fullname.FindRightChar(fullname.Length()-end,T('.'));

            if(pos==-1)
                return BaseString<T>();

            return include_dot? fullname.SubString(pos,     end- pos   ):
                                fullname.SubString(pos+1,   end-(pos+1));
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
    }//namespace filesystem
}//namespace hgl
#endif//HGL_FILE_SYSTEM_INCLUDE
