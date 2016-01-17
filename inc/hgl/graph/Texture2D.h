#ifndef HGL_GRAPH_TEXTURE_2D_INCLUDE
#define HGL_GRAPH_TEXTURE_2D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		class Bitmap2D;

		/**
		* 2D贴图
		*/
		class Texture2D:public Texture
		{
		protected:

			int width,height;																							///<长、宽

			uint wrap_s,wrap_t;

		public:
					uint	GetCoordNumber	()const{return 2;}															///<取得坐标数量
					int		GetWidth		()const{return width;}
					int		GetHeight		()const{return height;}

		public:

							Texture2D();
							~Texture2D();

					bool	LoadFile		(const OSString &,uint vf=0,uint ltp=0);									///<从文件加载一个贴图

			virtual bool	SetImage		(uint w,uint h,void *data,uint size,TSF sf,uint vf=0,uint ltp=0);			///<创建2D贴图数据
			virtual bool	ChangeImage		(uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf);					///<更改2D贴图数据

					bool	SetImage		(Bitmap2D *,uint vf=0,uint ltp=0);											///<从位图创建一个贴图
					bool	ChangeImage		(Bitmap2D *,uint l,uint t);													///<更改贴图数据

			virtual void	GetMipmapLevel	(int &,int &);																///<取得贴图mipmaps级别

			virtual int		GetImage		(void *data_pointer,TSF fmt,int level=0);									///<取得2D贴图数据

			virtual void	SetMinFilter	(uint);																		///<设置缩小过滤
			virtual void	SetMagFilter	(uint);																		///<设置放大过滤

			virtual void	SetWrapS		(uint);																		///<设置横向寻址方式
			virtual void	SetWrapT		(uint);																		///<设置纵向寻址方式
		};//class Texture2D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_2D_INCLUDE
