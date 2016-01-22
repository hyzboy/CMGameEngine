#ifndef HGL_GRAPH_TEXTURE_3D_INCLUDE
#define HGL_GRAPH_TEXTURE_3D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * 2D贴图阵列
		 */
		class Texture3D:public Texture
		{
			int width,height,depth;																						///<宽、高、层数

			uint wrap_s,wrap_t,wrap_r;

		public:

					uint	GetCoordNumber	()const{return 3;}															///<取得坐标数量
					int		GetWidth		(){return width;}
					int		GetHeight		(){return height;}
					int		GetDepth		(){return depth;}

		public:

							Texture3D();
			virtual			~Texture3D();

					bool	SetImage		(uint w,uint h,uint l,void *data,uint,TSF sf,uint vf,uint ltp);				///<设置

					int		GetImage		(void *data_pointer,TSF fmt,int level);										///<取得图片数据

					bool	ChangeImage		(uint l,uint t,uint s,uint w,uint h,uint d,void *data,uint bytes,TSF sf);	///<更改图片数据

			virtual void	SetWrapS		(uint);																		///<设置横向寻址方式
			virtual void	SetWrapT		(uint);																		///<设置纵向寻址方式
			virtual void	SetWrapR		(uint);																		///<设置纵向寻址方式
		};//class Texture3D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_3D_INCLUDE
