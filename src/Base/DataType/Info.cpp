#include<hgl/type/DataType.h>
#include<hgl/Info.h>

#define HGL_VERSION_STRING OS_TEXT("201706")

namespace hgl
{
    namespace info
    {
        static OSString hgl_status_string[hfsEnd];

        void InitString()
        {
            hgl_status_string[hfsName    ]=OS_TEXT("CMGDK");
            hgl_status_string[hfsVersion]=HGL_VERSION_STRING OS_TEXT(" for ") HGL_OS_NAME OS_TEXT("(") HGL_CPU_NAME OS_TEXT(") , ") HGL_COMPILER_NAME OS_TEXT(" (ver:") HGL_LIB_COMPILER_VERSION OS_TEXT(") Edition");

            for(int i=hfsCMGDKPath;i<hfsEnd;i++)
                hgl_status_string[i].Clear();
        }

        /**
        * 读取HGL标志字串
        * @param n 标志枚举
        * @return 标志字串
        * @see HGLFlagString
        */
        const OSString &GetString(HGLFlagString n)
        {
            const static OSString null_result=OS_TEXT("null");

            if(n<=hfsNone||n>=hfsEnd)return(null_result);

            return hgl_status_string[n];
        }

        void SetString(int n,const OSString &str)
        {
            if(n<hfsCMGDKPath||n>=hfsEnd)return;

            hgl_status_string[n]=str;
        }
    }//namespace info
}//namespace hgl
