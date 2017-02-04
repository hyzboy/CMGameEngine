#ifndef HGL_GRAPH_TEXTURE_2D_INCLUDE
#define HGL_GRAPH_TEXTURE_2D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
        struct Texture2DData;

		/**
		* 2D贴图
		*/
        class Texture2D:public Texture
		{
		protected:

			uint    width,height;																				///<宽、高

		public:

			int		GetWidth		()const{return width;}														///<取得纹理宽度
			int		GetHeight		()const{return height;}														///<取得纹理高度

		public:

                    Texture2D       ();

			bool	SetImage		(Texture2DData *);											                ///<创建2D贴图数据
			bool	ChangeImage		(uint l,uint t,uint w,uint h,void *,uint size, TSF sf);						///<更改贴图数据

            int		GetImage		(void *data_pointer,TSF fmt,int level=0);									///<取得2D贴图数据
		};//class Texture2D
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_2D_INCLUDE
