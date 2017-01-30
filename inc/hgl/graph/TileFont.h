#ifndef HGL_GRAPH_TILE_FONT_INCLUDE
#define HGL_GRAPH_TILE_FONT_INCLUDE

#include<hgl/VectorMath.h>
#include<hgl/type/Color4f.h>
#include<hgl/type/ActiveChain.h>
#include<hgl/graph/TileData.h>
#include<hgl/graph/FontSource.h>
#include<hgl/graph/Makeup.h>
#include<hgl/graph/VertexBuffer.h>
namespace hgl
{
	namespace graph
	{
		struct FontInfo;
		class Renderable;
		class Material;

		/**
		* 使用每个Tile代表一个字符的管理模块<br>
		* 即可以使用系统字体，由程序实时生成字符；也可以使用由美术制作好的字体图片。
		*/
		class TileFont																				///Tile字体
		{
		protected:

			TileData *tile_data;																	///<Tile管理器
			FontSource *chs_source,*eng_source;														///<字体数据源

			ActiveChain<u16char,TileData::Object *> fud;											///<字体缓冲管理

			uint8 *char_bitmap_buffer;																///<字符位图缓冲区
			uint char_bitmap_bytes;																	///<字符位图字节数

			VB2f *vertex2d;																			///<绘制用顶点坐标
			VB2f *tex_coord;																		///<绘制用顶点坐标
//			VB1ui *vertex_color;																	///<绘制用顶点色
			Material *fnt_mtl;																		///<绘制用材质
			Renderable *fnt_draw;																	///<绘制用对象

			int tile_width,tile_height;																///<所使用的Tile宽高
			int font_height;
			int line_distance;

		protected:

			void Clear(const u16char &,TileData::Object *&);										///<清除某个字

			FontSource::Bitmap *GetCharBitmap(const u16char &ch)									///<取得字符位图数据
				{return ((ch<=0xFF?eng_source:chs_source)->GetCharBitmap(ch));}

			FontSource::Bitmap *MakeCharBitmap(const u16char &ch);									///<生成字符位图数据

			TileData::Object *GetCharData(const u16char &);											///<取得字符数据

			bool MakeupText(Makeup &,int,int,const u16char *,int);									///<排版字符串

		public:

			int GetHeight		()const	{return font_height;}										///<取得字符高度
			int GetLineDistance	()const	{return line_distance;}										///<取得行间距

			void SetLineDistance(int n)	{line_distance=n;}

		public:	//属性

			Color4f Color;																			///<颜色

		public:

			TileFont(int,TileData *,FontSource *,FontSource *);
			virtual ~TileFont();

			float CharWidth(u16char);																///<指定字符宽度
			float GetStringWidth(const u16char *,int=-1);											///<求字符串宽度

		public:

			bool MakeupText(Makeup &,const u16char *,int=-1);										///<排版字符串
			bool MakeupText(Makeup &,const u16char *,int,TextAlignment);							///<排版字符串

			void Draw(const Matrix4f *,const Makeup &,int=-1);										///<根据排版进行绘制

			void Draw(float l,float t,const Makeup &makeup,int limit_char=-1)						///<根据排版进行绘制
			{
				const Matrix4f mat=translate(l,t,0);

				Draw(&mat,makeup,limit_char);
			}

			/**
			* 绘制一个字符串,可限制字数,并且处理\n
			* @param mat modelview变换矩阵
			* @param str 字符串
			* @param limit_char 限制的字数,-1表示不限制
			* @return 字符串的象素级宽度
			* @return <0 错误
			*/
			float DrawString(const Matrix4f *mat,const u16char *str,int limit_char=-1)				///<绘制一个字符串,可限制字数,并且处理\n
			{
				Makeup m;

				if(!MakeupText(m,str,limit_char))
					return(-1);

				Draw(mat,m,limit_char);

				return m.Width;
			}

			/**
			* 绘制一个字符串,可限制字数,并且处理\n
			* @param x X坐标
			* @param y Y坐标
			* @param str 字符串
			* @param limit_char 限制的字数,-1表示不限制
			* @return 字符串的象素级宽度
			* @return <0 错误
			*/
			float DrawString(float x,float y,const u16char *str,int limit_char=-1)					///<绘制一个字符串,可限制字数,并且处理\n
			{
				const Matrix4f mat=translate(x,y,0);

				return DrawString(&mat,str,limit_char);
			}

			float DrawFormat(const Matrix4f *mat,const u16char *,...);								///<绘制一个格式化的字符串
			float DrawFormat(float,float,const u16char *,...);										///<绘制一个格式化的字符串
		};//class TileFont

		TileFont *CreateTileFont(const FontInfo &,const FontInfo &,int=-1);							///<创建一个字体,使用系统字体

		/**
		* 通过系统字体创建一个Tile字体
		* @param chs_fontname 中文字体名称
		* @param eng_fontname 英文字体名称
		* @param width 宽,可以为0,表示默认。
		* @param height 高
		* @param bold 加粗,默认false
		* @param italic 斜体,默认false
		* @param anti 是否抗矩齿,默认true
		* @param count 缓冲区内保存的字符个数
		*/
		TileFont *CreateTileFont(const u16char *chs_fontname,const u16char *eng_fontname,int width,int height,bool bold=false,bool italic=false,bool anti=true,int count=-1);

		/**
		* 通过系统字体创建一个Tile字体,中英文字体相同
		* @param fontname 字体名称
		* @param width 宽,可以为0,表示默认。
		* @param height 高
		* @param bold 加粗,默认false
		* @param italic 斜体,默认false
		* @param anti 是否抗矩齿,默认true
		* @param count 缓冲区内保存的字符个数
		*/
		__inline TileFont *CreateTileFont(const u16char *fontname,int width,int height,bool bold=false,bool italic=false,bool anti=true,int count=-1)
		{
			return CreateTileFont(fontname,fontname,width,height,bold,italic,anti,count);
		}

		/**
		* 通过系统字体创建一个Tile字体,无斜体，无加粗
		* @param chs_fontname 中文字体名称
		* @param eng_fontname 英文字体名称
		* @param width 宽,可以为0,表示默认。
		* @param height 高
		* @param count 缓冲区内保存的字符个数
		*/
		__inline TileFont *CreateTileFont(const u16char *chs_fontname,const u16char *eng_fontname,int width,int height,int count=-1)
		{
			return CreateTileFont(chs_fontname,eng_fontname,width,height,false,false,true,count);
		}
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TILE_FONT_INCLUDE
