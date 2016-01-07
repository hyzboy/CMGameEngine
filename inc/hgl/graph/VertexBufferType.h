#ifndef HGL_GRAPH_VERTEX_BUFFER_TYPE_INCLUDE
#define HGL_GRAPH_VERTEX_BUFFER_TYPE_INCLUDE

#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        //顶点缓冲区认可的几种格式
        #define HGL_BYTE                    GL_BYTE
        #define HGL_UNSIGNED_BYTE           GL_UNSIGNED_BYTE
        #define HGL_SHORT                   GL_SHORT
        #define HGL_UNSIGNED_SHORT          GL_UNSIGNED_SHORT
        #define HGL_INT                     GL_INT
        #define HGL_UNSIGNED_INT            GL_UNSIGNED_INT
        #define HGL_HALF_FLOAT              GL_HALF_FLOAT
        #define HGL_FLOAT                   GL_FLOAT
        #define HGL_DOUBLE                  GL_DOUBLE

        #define HGL_STREAM_DRAW             GL_STREAM_DRAW  //流式绘制，数据很少变化
        #define HGL_STATIC_DRAW             GL_STATIC_DRAW  //静态绘制，数据基本不会变化
        #define HGL_DYNAMIC_DRAW            GL_DYNAMIC_DRAW //动态绘制，数据频繁变化

        /**
        * 顶点缓冲区类型描述
        */
        enum VertexBufferType:int
        {
            vbtNone=-1,

            vbtIndex=0,

            vbtVertex,
            vbtColor,
            vbtNormal,
            vbtTangent,
            vbtSecondColor,
            vbtFogCoord,

            vbtAmbient,             //环境光颜色
            vbtSpecular,            //镜面光颜色

            vbtDiffuseTexCoord,     //漫反射纹理坐标
            vbtAlphaTexCoord,       //透明度纹理坐标
            vbtNormalTexCoord,      //法线纹理坐标
            vbtTangentTexCoord,     //切线纹理坐标
            vbtAmbientTexCoord,     //环境光颜色纹理坐标
            vbtSpecularTexCoord,    //镜面光颜色纹理坐标

            vbtTexCoord0,   vbtTexCoord1,   vbtTexCoord2,   vbtTexCoord3,   vbtTexCoord4,   vbtTexCoord5,   vbtTexCoord6,   vbtTexCoord7,
            vbtTexCoord8,   vbtTexCoord9,   vbtTexCoord10,  vbtTexCoord11,  vbtTexCoord12,  vbtTexCoord13,  vbtTexCoord14,  vbtTexCoord15,
            vbtTexCoord16,  vbtTexCoord17,  vbtTexCoord18,  vbtTexCoord19,  vbtTexCoord20,  vbtTexCoord21,  vbtTexCoord22,  vbtTexCoord23,
            vbtTexCoord24,  vbtTexCoord25,  vbtTexCoord26,  vbtTexCoord27,  vbtTexCoord28,  vbtTexCoord29,  vbtTexCoord30,  vbtTexCoord31,

            vbtEnd
        };//enum VertexBufferType

        extern int HGL_MAX_VERTEX_ATTRIBS;                  ///<最大顶点属性数量

        extern const char VertexBufferName[vbtEnd][32];     ///<顶点缓冲区类型名称字串
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_VERTEX_BUFFER_TYPE_INCLUDE
