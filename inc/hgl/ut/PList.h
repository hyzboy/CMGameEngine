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
	class PList:public Map<UTF16String,UTF16String>												///属性列表
	{
	protected:

		void ReadData(const UTF16StringList &);

	public:

		virtual ~PList()HGL_DEFAULT_MEMFUNC;

		virtual bool Add(const UTF16String &);														///<向列表中增加一项
		virtual bool Add(const UTF16String &,const UTF16String &);									///<向列表中增加一项

		virtual bool LoadTxt(const OSString &);														///<从文件中加载列表

		template<ByteOrderMask BOM>
				bool SaveTxt(const OSString &filename,const u16char gap_ch=u'\t')					///<保存列表到文件
		{
			FileOutputStream fos;
			TextOutputStream<BOM> tos(&fos);

			if(!fos.Create(filename))return(false);

			int n=data_list.GetCount();

			tos.WriteBOM();

			while(n--)
			{
				UTF16String f,s;

				if(Get(n,f,s))
					tos.WriteLine(f+gap_ch+s);
			}

			return(true);
		}

		const UTF16String operator[](const UTF16String &)const;
	};//class PList
}//namespace hgl
#endif//HGL_PLIST_INCLUDE
