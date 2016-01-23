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

			int width,height;																							///<宽、高

			uint wrap_s,wrap_t;

		public:
					int		GetWidth		()const{return width;}														///<取得纹理宽度
					int		GetHeight		()const{return height;}														///<取得纹理高度

		public:

							Texture2D();
			virtual			~Texture2D();

					bool	LoadFile		(const OSString &,uint vf=0,uint ltp=0);									///<从文件加载一个贴图

			virtual bool	SetImage		(uint w,uint h,void *data,uint size,TSF sf,uint vf=0,uint ltp=0);			///<创建2D贴图数据
					bool	SetImage		(uint w,uint h,TSF sf){return SetImage(w,h,nullptr,0,sf);}					///<创建空的2D贴图数据
			virtual bool	ChangeImage		(uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf);					///<更改2D贴图数据

					bool	SetImage		(Bitmap2D *,uint vf=0,uint ltp=0);											///<从位图创建一个贴图
					bool	ChangeImage		(Bitmap2D *,uint l,uint t);													///<更改贴图数据

			virtual int		GetImage		(void *data_pointer,TSF fmt,int level=0);									///<取得2D贴图数据

			virtual void	SetWrapS		(uint);																		///<设置横向寻址方式
			virtual void	SetWrapT		(uint);																		///<设置纵向寻址方式
		};//class Texture2D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_2D_INCLUDE
