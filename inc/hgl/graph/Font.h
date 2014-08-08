#ifndef HGL_FONT_INCLUDE
#define HGL_FONT_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/Color4f.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
    /**
    * 字体数据结构<br>
    * 用于记录字体名称，粗体，斜体，下划线等等信息
    */
    class Font																						///字体数据结构
	{
    	#include<hgl/graph/Font.Attrib.h>

	public:	//属性

		Property<UTF16String> 	Name;																///<字体名称

		Property<int>			Width;																///<平均字体宽度
		Property<int>			Height;																///<字体高度

		Property<bool>			Bold;																///<是否粗体
		Property<bool>			Italic;																///<是否斜体

	public:	//事件

		DefEvent(void,OnChange,(Font *));															///<字体改变事件

    public:	//方法

        Font();
        Font(const Font &);
		Font(const UTF16String &,int,int,bool=false,bool=false);

        static void SetDefaultFont(const UTF16String &,int,int,bool,bool);							///<设置缺省字体
		static void SetDefaultFont(const Font &);													///<设置缺省字体
		static void ClearDefaultFont();																///<清除缺省字体

		void Set(const UTF16String &,int,int,bool=false,bool=false);								///<设置字体

	public:	//操作符重载

		bool operator == (const Font &);
		bool operator != (const Font &);

        void operator = (const Font &);
	};//class Font
}//namespace hgl
#endif//HGL_FONT_INCLUDE
