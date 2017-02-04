#ifndef HGL_GRAPH_TEXTURE_BIND_INCLUDE
#define HGL_GRAPH_TEXTURE_BIND_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
    namespace graph
    {
        class TextureBind
        {
            uint type;
            GLint old_id;            

        public:

            TextureBind(uint t, uint binding_type, GLint id):type(t)
            {
                glGetIntegerv(binding_type,&old_id);
                glBindTexture(type,id);
            }

            ~TextureBind()
            {
                glBindTexture(type,old_id);
            }
        };//class TextureBind
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_BIND_INCLUDE
