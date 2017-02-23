#include<hgl/graph/TextureChannels.h>
#include<string.h>
namespace hgl
{
    namespace graph
    {
        namespace
        {
            const MATERIAL_TEXTURE_CHANNEL_NAME MaterialTextureName[mtcMax]=
            {
                "Diffuse",
                "Ambient",
                "Specular",
                "Normal",
                "Tangent",
                "Light",
                "Alpha",
                "Depth",
                "Shadow",

                "Palette",

                "Height",
                "NormalHeight",

                "GBuffer",
            };
        }//namespace

        bool GetMaterialTextureName(MATERIAL_TEXTURE_CHANNEL_NAME &mtc_name,int index)
        {
            if(index<=mtcNone)return(false);
            if(index>=mtcMax+100)return(false);

            if(index<mtcMax)
            {
                memcpy(mtc_name,MaterialTextureName[index],MATERIAL_TEXTURE_CHANNEL_NAME_MAX_SIZE);
            }
            else
            {
                memcpy(mtc_name,"CusTexture_",10);

                index-=mtcMax;

                mtc_name[11]='0'+(index/10);
                mtc_name[12]='0'+(index%10);
                mtc_name[13]=0;
            }

            return(true);
        }
    }//namespace graph
}//namespace hgl
