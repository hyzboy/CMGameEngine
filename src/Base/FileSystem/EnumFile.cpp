#include<hgl/filesystem/EnumFile.h>

namespace hgl
{
    namespace filesystem
    {
        namespace
        {
            class OnlyFileEnum:public EnumFile
            {
            protected:

                List<FileInfo> *fi_list;

            public:

                OnlyFileEnum(List<FileInfo> *lfi)
                {
                    fi_list=lfi;
                }

                void ProcFile(struct EnumFileConfig *efc,hgl::filesystem::FileInfo &fi) override
                {
                    fi_list->Add(fi);
                }
            };//class OnlyFileEnum:public EnumFile
        }//namespace

        int GetFileInfoList(List<FileInfo> &fi_list,const OSString &folder_name,bool proc_folder,bool proc_file,bool sub_folder)
        {
            EnumFileConfig efc(folder_name);

            efc.proc_folder = proc_folder;
            efc.proc_file   = proc_file;
            efc.sub_folder  = sub_folder;

            OnlyFileEnum ofe(&fi_list);

            return ofe.Enum(&efc);
        }
    }//namespace filesystem
}//namespace hgl
