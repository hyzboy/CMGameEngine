#include<hgl/filesystem/FileSystem.h>

namespace hgl
{
    namespace filesystem
    {
        struct EnumFileConfig
        {
            OSString            folder_name;                ///<要枚举的目录名称

#if HGL_OS == HGL_OS_Windows                                ///<通配符过滤是Windows平台独有
            OSString            find_name;                  ///<要枚举的文件名称
#endif//HGL_OS == HGL_OS_Windows

            bool                proc_folder;                ///<是否处理目录
            bool                proc_file;                  ///<是否处理文件
            bool                sub_folder;                 ///<是否查找子目录

        public:

            EnumFileConfig(const OSString &fn)
            {
                folder_name=fn;

#if HGL_OS == HGL_OS_Windows
                find_name=OS_TEXT("*.*");
#endif//HGL_OS == HGL_OS_Windows

                proc_folder=true;
                proc_file=true;
                sub_folder=false;
            }

            EnumFileConfig(const EnumFileConfig *efc,const OSString &sub_folder_name)
            {
                folder_name =sub_folder_name;

#if HGL_OS == HGL_OS_Windows
                find_name   =efc->find_name;
#endif//HGL_OS == HGL_OS_Windows

                proc_folder =efc->proc_folder;
                proc_file   =efc->proc_file;
                sub_folder  =efc->sub_folder;
            }

            virtual ~EnumFileConfig()=default;
        };//struct EnumFileConfig

        /**
         * 枚举文件系统内的文件
         */
        class EnumFile
        {
        protected:

            virtual void ProcFolder(struct EnumFileConfig *parent_efc,struct EnumFileConfig *cur_efc,FileInfo &fi){}
            virtual void ProcFile(struct EnumFileConfig *,FileInfo &fi){}

            virtual EnumFileConfig *CreateSubConfig(struct EnumFileConfig *up_efc,const FileInfo &fi)
            {
                const OSString full_sub_folder_name=MergeFilename(up_efc->folder_name,fi.name);

                return(new EnumFileConfig(up_efc,full_sub_folder_name));
            }

        public:

            EnumFile()=default;
            virtual ~EnumFile()=default;

            virtual int Enum(EnumFileConfig *);
        };//class EnumFile
    }//namespace filesystem
}//namespace hgl
