#ifndef HGL_GRAPH_FONT_SOURCE_INCLUDE
#define HGL_GRAPH_FONT_SOURCE_INCLUDE

#include<hgl/graph/FontInfo.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 字体数据源抽像基类<br>
		* 主要提供点阵字体的数据产生管理
		*/
		class FontSource
		{
		public:

			/**
			* 字体位图数据
			*/
			struct Bitmap
			{
    			int x,y;		//图像显示偏移
				int w,h;		//图像尺寸

				int adv_x,adv_y;//字符尺寸

				unsigned char *data;
			};//struct Bitmap

		protected:

			FontInfo fnt;

			FontSource::Bitmap char_bitmap[0xFFFF];					///<字符位图数据

		protected:

			virtual bool MakeCharBitmap(u16char)=0;					///<产生字体数据
			virtual int GetLineHeight()const=0;						///<取得行高

		public:

			FontSource(const FontInfo &);
			virtual ~FontSource();

			FontSource::Bitmap *GetCharBitmap(u16char);				///<取得字体数据
		};//class FontSource
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_FONT_SOURCE_INCLUDE
