#ifndef HGL_GRAPH_RENDER_STATE_INCLUDE
#define HGL_GRAPH_RENDER_STATE_INCLUDE

#include<hgl/graph/Axis.h>
#include<hgl/graph/ColorFormat.h>
#include<hgl/graph/VertexBufferType.h>
#include<hgl/graph/TextureChannels.h>
#include<string.h>
namespace hgl
{
    namespace graph
    {
        struct RenderState
        {
            bool				mvp;                   	///<是否包含MVP矩阵
            Axis				axis;					///<高度轴方向

            bool				vertex_color;          	///<是否包含顶点颜色
            bool				vertex_normal;         	///<是否包含顶点法线
            bool				vertex_tangent;        	///<是否包含顶点切线

            bool				diffuse_map;           	///<是否包含漫反射贴图
            bool				normal_map;            	///<是否包含法线贴图
            bool				tangent_map;           	///<是否包含切线贴图

            bool				color_material;        	///<是否包含颜色材质
            bool				alpha_test;            	///<是否使用Alpha测试
            bool				outside_discard;       	///<是否使用出界放弃

            bool				height_map;            	///<是否是高度图

            ColorFormat			vertex_color_format;  	///<顶点色格式
            uint8				vertex_coord;         	///<顶点坐标维数

            uint8				tex_number;				///<贴图数量
            uint32				tex_type[mtcMax];    	///<通道纹理类型
            uint32				tex_color[mtcMax];		///<通道纹理颜色成份
            uint8				vbc[mtcMax];    		///<通道顶点坐标维数
            VertexBufferType	vbt[mtcMax];			///<通道对应的顶点缓冲区类型

            bool				lighting;				///<是否承接光照

            uint8				direction_light;		///<方向光数量
            uint8				point_light;			///<点光源数量
            uint8				spot_light;				///<聚光灯数量
            uint8				infinite_spot_light;	///<无尽聚光灯数量

            bool				two_side;				///<双面材质

        public:

            CompOperatorMemcmp(const RenderState &);
        };//RenderState
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_STATE_INCLUDE
