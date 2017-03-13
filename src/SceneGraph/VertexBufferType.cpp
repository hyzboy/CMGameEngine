#include<hgl/graph/VertexBufferType.h>
#include<string.h>
namespace hgl
{
    namespace graph
    {
        namespace
        {
            const VERTEX_BUFFER_NAME VertexBufferName[vbtEnd]=
            {
                "Index",

                "Vertex",
                "Color",
                "Normal",
                "Tangent",
                "BiNormal",
                "SecondColor",
                "FogCoord",

                "Ambient",
                "Specular",

                "TexCoord_Diffuse",
                "TexCoord_Alpha",
                "TexCoord_Normal",
                "TexCoord_Tangent",
                "TexCoord_BiNormal",
                "TexCoord_Ambient",
                "TexCoord_Specular",

                "TexCoord_GBuffer",
            };//const VERTEX_BUFFER_NAME VertexBufferName
        }//namespace

        bool GetVertexBufferName(VERTEX_BUFFER_NAME &vbn,int index)                ///<获取顶点缓冲区类型名称字串
        {
            if(index<=vbtNone)return(false);
            if(index>=vbtEnd+100)return(false);

            if(index<vbtEnd)
            {
                memcpy(vbn,VertexBufferName[index],VERTEX_BUFFER_NAME_MAX_SIZE);
            }
            else
            {
                memcpy(vbn,"CusTexCoord_",12);

                index-=vbtEnd;

                vbn[12]='0'+(index/10);
                vbn[13]='0'+(index%10);
                vbn[14]=0;
            }

            return(true);
        }
    }//namespace graph
}//namespace hgl
