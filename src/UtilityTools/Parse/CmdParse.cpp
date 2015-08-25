#include<hgl/ut/CmdParse.h>
#include<hgl/Str.h>

namespace hgl
{
    namespace util
    {
        CmdParse::CmdParse(const OSStringList &_args)
        {
            args=_args;
        }

        CmdParse::~CmdParse()
        {
        }

        int CmdParse::Find(const OSString &flag)const
        {
            return args.Find(flag,flag.Length());
        }

        bool CmdParse::GetInteger(const OSString &flag,int *result)const
        {
            int index=Find(flag);

            if(index==-1)return(false);

            const OSString &str=args[index];

            if(str.Length()>flag.Length())
            {
                stoi(str.c_str()+flag.Length(),*result);
            }
            else
            {
                stoi(str.c_str(),*result);
            }

            return(true);
        }

        bool CmdParse::GetString(const OSString &flag,OSString &result)const
        {
            int index=Find(flag);
            os_char *p=nullptr;

            if(index==-1)return(false);

            const OSString &str=args[index];

            if(str.Length()>flag.Length())
            {
                p=str.c_str()+flag.Length();
            }
            else
            {
                p=args[index+1];
            }

            if(*p=='"')    //有引号
            {
                int len=strchr(p+1,L'"')-p;

                if(len>1)
                    result.Strcpy(p+1,len-1);
                else
                    result=p+1;
            }
            else            //无引号
            {
                result=p;
            }

            return(true);
        }
    }//namespace util
}//namespace hgl
