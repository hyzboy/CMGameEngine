#include"TextureBaseControl.h"
#include<hgl/graph/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        class TextureBind
        {
            uint type;
            int old_id;            

        public:

            TextureBind(uint t, uint binding_type, int id):type(t)
            {
                glGetIntegerv(binding_type,&old_id);
                glBindTexture(type,id);
            }

            ~TextureBind()
            {
                glBindTexture(type,old_id);
            }
        };//class TextureBind

        class TextureBaseControlBind:public TextureBaseControl
        {
        public:

            using TextureBaseControl::TextureBaseControl;

        public:     //mipmaps

            void GenMipmaps()                                                                       ///<生成mipmaps
            {
                TextureBind tb(type,binding_type,texture_id);

                glGenerateMipmap(type);
            }

            void GetMipmapLevel(int &base_level,int &max_level)                                        ///<取得贴图mipmaps级别
            {
                TextureBind tb(type,binding_type,texture_id);

                glGetTexParameteriv(type,GL_TEXTURE_BASE_LEVEL,&base_level);
                glGetTexParameteriv(type,GL_TEXTURE_MAX_LEVEL,&max_level);
            }
        };//class TextureBaseControlBind
        
        TextureBaseControl *TextureBaseControlCreateBind(uint t,uint bt)
        {
            uint id;

            glGenTextures(1,&id);

            return(new TextureBaseControlBind(t,bt,id));
        }
    }//namespace graph
}//namespace hgl
