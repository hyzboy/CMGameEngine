#ifndef HGL_COLOR_TYPE_INCLUDE
#define HGL_COLOR_TYPE_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
	/**
	* 这个枚举用来声明在程序中所使用到的色彩类型
	*/
	enum_uint(ColorType)        ///色彩类型
	{
		ctNone          =0,     ///<起始定义，无意义。如使用代表未知色彩

		ctLuminance,            ///<亮度
		ctLuminanceAlpha,       ///<亮度与透明度
		ctRGB,                  ///<RGB三原色
		ctRGBA,                 ///<RGB三原色与透明度
		ctAlpha,                ///<Alpha透明度

		ctBGR,					///<BGR三原色
		ctBGRA,					///<BGR三原色与透明度

		ctEnd,                  ///<结束定义,无意义
	};//enum_uint(ColorType)

	const unsigned int 	ColorTypeCount		=ctEnd+1;

	extern const u16char		ColorTypeName		[ColorTypeCount][32];	///色彩类型名称
	extern const unsigned int	GLFormatByColorType	[ColorTypeCount]; 		///针对色彩类型而对应的OpenGL格式
	extern const unsigned int	BytesByColorType	[ColorTypeCount];  		///针对色彩类型而对应的每象素字节数
	extern const bool			BlendByColorType	[ColorTypeCount];    	///这一种格式是否需要混合

	unsigned int ColorNameToFormat(const u16char *);						///<将色彩类型名称转成OpenGL格式
}//namespace hgl
#endif//HGL_COLOR_TYPE_INCLUDE
