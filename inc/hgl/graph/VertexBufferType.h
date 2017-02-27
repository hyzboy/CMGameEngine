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
            vbtBiNormal,vbtBiTangen=vbtBiNormal,
            vbtSecondColor,
            vbtFogCoord,

            vbtAmbient,                                 ///<环境光颜色
            vbtSpecular,                                ///<镜面光颜色

            vbtDiffuseTexCoord,                         ///<漫反射纹理坐标
            vbtAlphaTexCoord,                           ///<透明度纹理坐标
            vbtNormalTexCoord,                          ///<法线纹理坐标
            vbtTangentTexCoord,                         ///<切线纹理坐标
            vbtBiNormalTexCoord,                        ///<副法线纹理坐标
            vbtBiTangentTexCoord=vbtBiNormalTexCoord,   ///<副切线纹理坐标
            vbtAmbientTexCoord,                         ///<环境光颜色纹理坐标
            vbtSpecularTexCoord,                        ///<镜面光颜色纹理坐标

			vbtGBufferTexCoord,		                    ///<延迟渲染最终混合用纹理坐标

            vbtEnd
        };//enum VertexBufferType

        extern int HGL_MAX_VERTEX_ATTRIBS;                  ///<最大顶点属性数量

		const int VERTEX_BUFFER_NAME_MAX_SIZE=32;

		using VERTEX_BUFFER_NAME=char[VERTEX_BUFFER_NAME_MAX_SIZE];

		bool GetVertexBufferName(VERTEX_BUFFER_NAME &,int);		///<获取顶点缓冲区类型名称字串
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_VERTEX_BUFFER_TYPE_INCLUDE
