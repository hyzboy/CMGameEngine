#ifndef HGL_PLIST_INCLUDE
#define HGL_PLIST_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
namespace hgl
{
	using namespace io;

	/**
	* 属性列表,类似INI的管理类
	*/
    template<typename C,typename T> class PList:public Map<T,T>										///属性列表
	{
	protected:

        void ReadData(const StringList<T> &sl)
        {
            int n=sl.GetCount();

            while(n--)
                Add(sl[n]);
        }

	public:

		virtual ~PList()=default;

        virtual bool Add(const T &key)                                                              ///<向列表中增加一项
        {
            T name;
            C *value;
            int off;

            if(key.Length()<2)return(false);

            if(((off=key.FindChar(C('\t')))==-1)
            && ((off=key.FindChar(C(' '))) ==-1)
            && ((off=key.FindChar(C('='))) ==-1))
            && ((off=key.FindChar(C(':'))) ==-1))
                return(false);

            name.Strcpy(key,off);
            off++;

            value=key.c_str()+off;

            while(true)
            {
                if(*value == C('\t')
                || *value == C('=')
                || *value == C(' ')
                || *value == C(':'))
                    value++;
                else
                {
                    T str=value;

                    Map<T,T>::Add(name,str);

                    return(true);
                }
            }
        }//bool PList::Add

		virtual bool Add(const T &key,const T &value)									            ///<向列表中增加一项
        {
            return Map<T,T>::Add(key,value);
        }

        virtual bool LoadTxt(const OSString &filename)                                              ///<从文件中加载列表
        {
            StringList<T> sl;

            if(LoadStringListFromTextFile(sl,filename)<=0)
                return(false);

            ReadData(sl);

            return(true);
        }

		template<ByteOrderMask BOM>
                bool SaveTxt(const OSString &filename,const T &gap_ch=C('\t'))					    ///<保存列表到文件
		{
			FileOutputStream fos;
			TextOutputStream<BOM> tos(&fos);

			if(!fos.Create(filename))return(false);

			int n=this->data_list.GetCount();

			tos.WriteBOM();

			while(n--)
			{
				T f,s;

				if(Get(n,f,s))
					tos.WriteLine(f+gap_ch+s);
			}

			return(true);
		}
	};//class PList

    using UTF8PList =PList<char,   UTF8String   >;
    using UTF16PList=PList<u16char,UTF16String  >;
    using WidePList =PList<wchar_t,WideString   >;
}//namespace hgl
#endif//HGL_PLIST_INCLUDE
