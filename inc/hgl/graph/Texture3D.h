#ifndef HGL_GRAPH_TEXTURE_3D_INCLUDE
#define HGL_GRAPH_TEXTURE_3D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * 3D贴图
		 */
		class Texture3D:public Texture
		{
			uint width,height,depth;																					///<宽、高、层数

			uint wrap_s,wrap_t,wrap_r;

		public:

					uint	GetType         ()const{return HGL_TEXTURE_3D;}override										///<取得类型
					uint	GetBindingType  ()const{return HGL_TEX_BIND_3D;}override								    ///<取得绑定类型
					int		GetWidth		(){return width;}
					int		GetHeight		(){return height;}
					int		GetDepth		(){return depth;}

		public:

							Texture3D();
			virtual			~Texture3D();

					bool	SetImage		(uint w,uint h,uint l,void *data,uint,TSF sf,uint vf=0,uint ltp=0);			///<设置贴图数据
					bool	SetImage		(uint w,uint h,uint l,TSF sf){return SetImage(w,h,l,nullptr,0,sf);}			///<设置空的贴图数据

					int		GetImage		(void *data_pointer,TSF fmt,int level);										///<取得贴图数据

					bool	ChangeImage		(uint l,uint t,uint s,uint w,uint h,uint d,void *data,uint bytes,TSF sf);	///<更改贴图数据

			virtual void	SetWrapS		(uint);																		///<设置横向寻址方式
			virtual void	SetWrapT		(uint);																		///<设置纵向寻址方式
			virtual void	SetWrapR		(uint);																		///<设置纵向寻址方式
		};//class Texture3D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_3D_INCLUDE
