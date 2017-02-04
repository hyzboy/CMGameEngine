#ifndef HGL_GRAPH_TEXTURE_1D_INCLUDE
#define HGL_GRAPH_TEXTURE_1D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
        struct Texture1DData;

		/**
		* 1D贴图
		*/
		class Texture1D:public Texture
		{
		protected:

            uint    length;

		public:	//

			uint	GetLength       ()const{return length;}										    ///<取得纹理长度

		public:

					Texture1D       ();

			bool	SetImage		(Texture1DData *);					                            ///<创建1D贴图数据
			bool	ChangeImage		(uint s,uint l,void *data,uint size,TSF sf);					///<更改1D贴图数据

			int		GetImage		(void *data_pointer,TSF fmt,int level=0);						///<取得1D贴图数据
		};//class Texture1D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_1D_INCLUDE
