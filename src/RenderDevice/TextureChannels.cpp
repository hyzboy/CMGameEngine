#include<hgl/graph/TextureChannels.h>
namespace hgl
{
    namespace graph
    {
        extern const char MaterialTextureName[mtcMax][32]=
        {
            "Diffuse",
            "Ambient",
            "Specular",
            "Normal",
            "Tangent",
            "Light",
            "Alpha",
            "Shadow",

            "Palette",

            "Height",
            "NormalHeight",
        };
    }//namespace graph
}//namespace hgl
