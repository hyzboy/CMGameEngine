#ifndef HGL_GRAPH_TEXTURE_INSTANCE_DSA_INCLUDE
#define HGL_GRAPH_TEXTURE_INSTANCE_DSA_INCLUDE

#include"TextureInstace.h"
namespace hgl
{
    namespace graph
    {
        class TextureInstaceDSA:public TextureInstace
        {
        public:

            using TextureInstance::TextureInstance;

		public:     //mipmaps

            void GenMipmaps()                                                                       ///<生成mipmaps
            {
                glGenerateTextureMipmap(texture_id);
            }

			void GetMipmapLevel(int &base_level,int &max_level)									    ///<取得贴图mipmaps级别
            {
                glGetTextureParameteriv(texture_id,GL_TEXTURE_BASE_LEVEL,&base_level);
                glGetTextureParameteriv(texture_id,GL_TEXTURE_MAX_LEVEL,&max_level);
            }
        };//class TextureInstaceDSA
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_INSTANCE_DSA_INCLUDE
