#ifndef HGL_GRAPH_TEXTURE_2D_INCLUDE
#define HGL_GRAPH_TEXTURE_2D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 2D贴图
		*/
        class Texture2D:public Texture
		{
		protected:

			uint    width,height;																				///<宽、高

			virtual bool	_SetImage	(Texture2DData *tex)=0;
            virtual int		_GetImage	(void *data_pointer, TSF fmt, int level,int width,int height)=0;
            virtual bool	_ChangeImage(uint l, uint t, uint w, uint h, void *data, uint bytes, TSF sf)=0;

		public:

			int		GetWidth		()const{return width;}														///<取得纹理宽度
			int		GetHeight		()const{return height;}														///<取得纹理高度

		public:

                    Texture2D       (TextureBaseControl *t):Texture(t){width=height=0;}

			bool	SetImage		(Texture2DData *);											                ///<创建2D贴图数据
            bool    SetImage        (uint w, uint h, TSF fmt)
                                    {
                                        Texture2DData tex(w, h, fmt);

                                        return SetImage(&tex);
                                    }
            bool    SetImage        (uint w, uint h, void *data, uint size, TSF sf, uint vf)
                                    {
                                        Texture2DData tex(w,h,data,size,sf,vf);

                                        return SetImage(&tex);
                                    }

			bool	ChangeImage		(uint l,uint t,uint w,uint h,void *,uint size, TSF sf);						///<更改贴图数据

            int		GetImage		(void *data_pointer,TSF fmt,int level=0);									///<取得2D贴图数据
		};//class Texture2D

		Texture2D *CreateTexture2D();
		Texture2D *CreateTexture2D(Texture2DData *);
		Texture2D *CreateTexture2D(uint width,uint height,TSF format);
		Texture2D *CreateTexture2D(uint width,uint height,void *bitmap,uint bitmap_bytes,TSF source_format,uint video_format);
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_2D_INCLUDE
