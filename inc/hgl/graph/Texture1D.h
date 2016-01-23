#ifndef HGL_GRAPH_TEXTURE_1D_INCLUDE
#define HGL_GRAPH_TEXTURE_1D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 1D贴图
		*/
		class Texture1D:public Texture
		{
		protected:

			int length;																									///<长

			uint wrap;

		public:	//

					int		GetLength		()const{return length;}														///<取得纹理长度

		public:

							Texture1D();
			virtual 		~Texture1D();

			virtual bool	SetImage		(uint l,void *data,uint size,TSF sf,uint vf=0,uint ltp=0);					///<创建1D贴图数据
					bool	SetImage		(uint l,TSF sf){return SetImage(l,nullptr,0,sf);}							///<设置空的1D贴图数据
			virtual bool	ChangeImage		(uint s,uint l,void *data,uint size,TSF sf);								///<更改1D贴图数据

			virtual int		GetImage		(void *data_pointer,TSF fmt,int level=0);									///<取得1D贴图数据

			virtual void	SetWrap			(uint);																		///<设置寻址方式
		};//class Texture1D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_1D_INCLUDE
