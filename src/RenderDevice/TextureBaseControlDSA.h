#ifndef HGL_GRAPH_TEXTURE_BASE_CONTROL_DSA_INCLUDE
#define HGL_GRAPH_TEXTURE_BASE_CONTROL_DSA_INCLUDE

#include<hgl/graph/TextureBaseControl.h>
namespace hgl
{
    namespace graph
    {
        class TextureBaseControlDSA:public TextureBaseControl
        {
        public:

            using TextureBaseControl::TextureBaseControl;

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
        };//class TextureBaseControlDSA
        
        inline TextureBaseControl *TextureBaseControlCreateDSA(uint t,uint bt,uint id)
        {
            return(new TextureBaseControlDSA(t,bt,id));
        }
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_BASE_CONTROL_DSA_INCLUDE
