#include<hgl/graph/SamplerObject.h>
namespace hgl
{
    namespace graph
    {
        float SamplerObject::max_texture_max_anistropy=0;
        float SamplerObject::default_max_anistropy=0;

        void InitTextureMaxAnistropy(float pos)
        {
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&SamplerObject::max_texture_max_anistropy);

            SamplerObject::default_max_anistropy=pos*SamplerObject::max_texture_max_anistropy;
        }
    }//namespace graph
}//namespace hgl
