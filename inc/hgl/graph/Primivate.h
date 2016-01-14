#ifndef HGL_GRAPH_PRIMIVATE_INCLUDE
#define HGL_GRAPH_PRIMIVATE_INCLUDE

#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        //可绘制的图元
        #define HGL_PRIM_POINTS                     GL_POINTS                   ///<点
        #define HGL_PRIM_LINES                      GL_LINES                    ///<线
        #define HGL_PRIM_LINE_STRIP                 GL_LINE_STRIP               ///<连续线
        #define HGL_PRIM_LINE_LOOP                  GL_LINE_LOOP                ///<线圈
        #define HGL_PRIM_TRIANGLES                  GL_TRIANGLES                ///<三角形
        #define HGL_PRIM_TRIANGLE_STRIP             GL_TRIANGLE_STRIP           ///<三角形条
        #define HGL_PRIM_TRIANGLE_FAN               GL_TRIANGLE_FAN             ///<扇形
        #define HGL_PRIM_LINES_ADJACENCY            GL_LINES_ADJACENCY          ///<代表一個有四個頂點的Primitive,其中第二個點與第三個點會形成線段,而第一個點與第四個點則用來提供2,3鄰近點的資訊.
        #define HGL_PRIM_LINE_STRIP_ADJACENCY       GL_LINE_STRIP_ADJACENCY     ///<與GL_LINES_ADJACENCY類似,第一個點跟最後一個點提供資訊,剩下的點則跟Line Strip一樣形成線段.
        #define HGL_PRIM_TRIANGLES_ADJACENCY        GL_TRIANGLES_ADJACENCY      ///<代表一個有六個頂點的Primitive,其中第1,3,5個點代表一個Triangle,而地2,4,6個點提供鄰近資訊.(由1起算)
        #define HGL_PRIM_TRIANGLE_STRIP_ADJACENCY   GL_TRIANGLE_STRIP_ADJACENCY ///<4+2N個Vertices代表N個Primitive,其中1,3,5,7,9...代表原本的Triangle strip形成Triangle,而2,4,6,8,10...代表鄰近提供資訊的點.(由1起算)
        #define HGL_PRIM_PATCHES                    GL_PATCHES
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_PRIMIVATE_INCLUDE
