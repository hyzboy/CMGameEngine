#ifndef HGL_API_TEXTURE_INCLUDE
#define HGL_API_TEXTURE_INCLUDE

#include<hgl/graph/Texture.h>
#include<hgl/graph/TextureData.h>
namespace hgl
{
	namespace graph
	{
		namespace api
		{
			bool InitTextureAPI();

			using HGL_API_CREATE_TEXTURE		=uint (*)(uint format);
			using HGL_API_CREATE_MULTI_TEXTURES	=uint (*)(uint *,const uint format, const uint count);
			using HGL_API_DELETE_TEXTURE		=void (*)(const uint);
			using HGL_API_DELETE_MULTI_TEXTURES	=void (*)(const uint *,const uint);

            using HGL_API_SET_TEX_IMAGE_1D      =bool (*)(uint, Texture1DData *);
            using HGL_API_SET_TEX_IMAGE_2D      =bool (*)(uint, Texture2DData *);
            using HGL_API_SET_TEX_IMAGE_3D      =bool (*)(uint, Texture3DData *);
            using HGL_API_SET_TEX_IMAGE_1D_ARRAY=bool (*)(uint, Texture1DArrayData *);
            using HGL_API_SET_TEX_IMAGE_2D_ARRAY=bool (*)(uint, Texture2DArrayData *);

            using HGL_API_GET_TEX_IMAGE_1D      =int (*)(uint, void *, TSF, int, int);

            using HGL_API_CHANGE_TEX_IMAGE_1D   =bool (*)(uint,uint,uint,void *,uint,TSF);

			HGL_API_CREATE_TEXTURE			CreateTexture=nullptr;									///<创建纹理
			HGL_API_CREATE_MULTI_TEXTURES	CreateMultiTextures = nullptr;							///<创建多个纹理
			HGL_API_DELETE_TEXTURE			DeleteTexture = nullptr;
			HGL_API_DELETE_MULTI_TEXTURES	DeleteMultiTextures = nullptr;

            HGL_API_SET_TEX_IMAGE_1D        SetTexImage1D=nullptr;
            HGL_API_SET_TEX_IMAGE_2D        SetTexImage2D=nullptr;
            HGL_API_SET_TEX_IMAGE_3D        SetTexImage3D=nullptr;
            HGL_API_SET_TEX_IMAGE_1D_ARRAY  SetTexImage1DArray=nullptr;
            HGL_API_SET_TEX_IMAGE_2D_ARRAY  SetTexImage2DArray=nullptr;

            HGL_API_GET_TEX_IMAGE_1D        GetTexImage1D=nullptr;
            HGL_API_CHANGE_TEX_IMAGE_1D     ChangeTexImage1D=nullptr;
		}//namespace api
	}//namespace graph
}//namespace hgl
#endif//HGL_API_TEXTURE_INCLUDE
