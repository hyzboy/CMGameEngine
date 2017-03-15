#ifndef HGL_GRAPH_RENDER_STATE_INCLUDE
#define HGL_GRAPH_RENDER_STATE_INCLUDE

#include<hgl/graph/Axis.h>
#include<hgl/graph/PixelCompoment.h>
#include<hgl/graph/VertexBufferType.h>
#include<hgl/graph/TextureChannels.h>
#include<hgl/graph/LightMode.h>
#include<string.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 渲染模式
        */
        enum RenderMode
        {
            rmNone=0,

            rmForward,              //前向
            rmDeferredLighting,     //前向+延迟灯光

            rmDeferredShading128,   //延迟,128位GBuffer模式(48位RGB,16位Specular,32位法线,32位XY坐标)

            rmEnd
        };

        struct RenderState
        {
            bool				mvp;                   	///<是否包含MVP矩阵
            Axis				up_axis;				///<向上轴方向

            bool				vertex_color;          	///<是否包含顶点颜色
            bool				vertex_normal;         	///<是否包含顶点法线
            bool				vertex_tangent;        	///<是否包含顶点切线

            union
            {
                bool            vertex_bi_normal;       ///<是否包含副法线
                bool            vertex_bi_tangent;      ///<是否包含副切线
            };

            bool				diffuse_map;           	///<是否包含漫反射贴图
            bool				normal_map;            	///<是否包含法线贴图
            bool				tangent_map;           	///<是否包含切线贴图

            bool				color_material;        	///<是否包含颜色材质
            bool				alpha_test;            	///<是否使用Alpha测试
            bool                hsv_clamp_color;        ///<是否使用HSVColor剔除
            bool				outside_discard;       	///<是否使用出界放弃

            bool				height_map;            	///<是否是高度图

            PixelCompoment      vertex_pixel_compoment; ///<顶点象素成份
            uint8				vertex_coord;         	///<顶点坐标维数

            uint8				tex_number;				///<贴图数量
            uint32				tex_type[mtcMax];    	///<通道纹理类型
            uint32				tex_pf[mtcMax];		    ///<通道纹理象素格式
            uint8				vbc[mtcMax];    		///<通道顶点坐标维数
            VertexBufferType	vbt[mtcMax];			///<通道对应的顶点缓冲区类型

            LightMode           light_mode;				///<光照模式
            bool                sun_light;              ///<是否有太阳光

            uint8				direction_light;		///<方向光数量
            uint8				point_light;			///<点光源数量
            uint8				spot_light;				///<聚光灯数量
            uint8				infinite_spot_light;	///<无尽聚光灯数量

            bool				two_side;				///<双面材质

            RenderMode          render_mode;			///<渲染模式

        public:

            CompOperatorMemcmp(const RenderState &);
        };//RenderState
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_STATE_INCLUDE
