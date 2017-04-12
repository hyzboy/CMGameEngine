#ifndef HGL_GRAPH_MATERIAL_INCLUDE
#define HGL_GRAPH_MATERIAL_INCLUDE

#include<hgl/algorithm/VectorMath.h>
#include<hgl/type/DataType.h>
#include<hgl/type/FixedArray.h>
#include<hgl/type/Color4f.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/TextureChannels.h>
#include<hgl/graph/SceneOrient.h>
#include<hgl/graph/LightMode.h>
#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        class Texture;

        //面显示
        #define HGL_FACE_FRONT						GL_FRONT
        #define HGL_FACE_BACK						GL_BACK
        #define HGL_FACE_FRONT_AND_BACK				GL_FRONT_AND_BACK

        //三角形绘制方式
        #define HGL_DRAW_FILL						GL_FILL
        #define HGL_DRAW_LINE						GL_LINE
        #define HGL_DRAW_POINT						GL_POINT

        //深度比较方式
        #define HGL_DEPTH_NEVER						GL_NEVER
        #define HGL_DEPTH_LESS						GL_LESS
        #define HGL_DEPTH_EQUAL						GL_EQUAL
        #define HGL_DEPTH_LEQUAL					GL_LEQUAL
        #define HGL_DEPTH_GREATER					GL_GREATER
        #define HGL_DEPTH_NOTEQUAL					GL_NOTEQUAL
        #define HGL_DEPTH_GEQUAL					GL_GEQUAL
        #define HGL_DEPTH_ALWAYS					GL_ALWAYS

        //混合方式
        #define HGL_BLEND_ADD						GL_FUNC_ADD
        #define HGL_BLEND_SUBTRACT					GL_FUNC_SUBTRACT
        #define HGL_BLEND_REVERSE_SUBTRACT			GL_FUNC_REVERSE_SUBTRACT
        #define HGL_BLEND_MIN						GL_MIN
        #define HGL_BLEND_MAX						GL_MAX

        //混合因子
        #define HGL_BLEND_ZERO						GL_ZERO
        #define HGL_BLEND_ONE						GL_ONE

        #define HGL_BLEND_SRC_COLOR					GL_SRC_COLOR
        #define HGL_BLEND_ONE_MINUS_SRC_COLOR		GL_ONE_MINUS_SRC_COLOR
        #define HGL_BLEND_DST_COLOR					GL_DST_COLOR
        #define HGL_BLEND_ONE_MINUS_DST_COLOR		GL_ONE_MINUS_DST_COLOR

        #define HGL_BLEND_SRC_ALPHA					GL_SRC_ALPHA
        #define HGL_BLEND_ONE_MINUS_SRC_ALPHA		GL_ONE_MINUS_SRC_ALPHA
        #define HGL_BLEND_DST_ALPHA					GL_DST_ALPHA
        #define HGL_BLEND_ONE_MINUS_DST_ALPHA		GL_ONE_MINUS_DST_ALPHA

        #define HGL_BLEND_CONSTANT_COLOR			GL_CONSTANT_COLOR
        #define HGL_BLEND_ONE_MINUS_CONSTANT_COLOR	GL_ONE_MINUS_CONSTANT_COLOR
        #define HGL_BLEND_CONSTANT_ALPHA			GL_CONSTANT_ALPHA
        #define HGL_BLEND_ONE_MINUS_CONSTANT_ALPHA	GL_ONE_MINUS_CONSTANT_ALPHA
        #define HGL_BLEND_SRC_ALPHA_SATURATE		GL_SRC_ALPHA_SATURATE
        #define HGL_BLEND_SRC1_COLOR				GL_SRC1_COLOR
        #define HGL_BLEND_ONE_MINUS_SRC1_COLOR		GL_ONE_MINUS_SRC1_COLOR
        #define HGL_BLEND_SRC1_ALPHA				GL_SRC1_ALPHA
        #define HGL_BLEND_ONE_MINUS_SRC1_ALPHA		GL_ONE_MINUS_SRC1_ALPHA

        /**
        * 混合模式数据结构
        */
        struct BlendMode			///混合模式
        {
            struct
            {
                uint src,dst;		///<混合因子
                uint func;			///<混合方式
            }rgb,alpha;
        };//struct BlendMode

        /**
         * 纹理材质项
         */
        struct MaterialTexture
        {
            Texture *tex;			///<纹理

            Color4f color;			///<颜色强弱系数
            uint8 blend[2];			///<混合方式(同photoshop图层一样的各种混合指定)
            float scale;			///<缩放系数
            float rotate;			///<旋转系数
            float blur[4];			///<模糊系数(平面模糊，径向模糊，运动模糊,.....等等)
            uint8 mosic;			///<马赛克化
        };//struct MaterialTexture

        /**
        * 材质
        */
        class Material
        {
        protected:

            Color4f		Color;																		///<颜色

            uint		draw_face;																	///<绘制的面
            uint		fill_mode;																	///<填充方式

            uint		cull_face;																	///<裁剪面,0:不裁,FACE_FRONT:正面,FACE_BACK:背面

            bool		depth_test;																	///<深度测试
            uint		depth_func;																	///<深度处理方式
            bool		depth_mask;																	///<深度遮罩

            float		alpha_test;																	///<alpha测试值,0:全部不显示,1:全部显示

            Color4f     hsv_clamp_color;                                                            ///<HSV颜色剔除(保存格式为H,V,H误差,V误差)

            bool		outside_discard;															///<贴图出界放弃

            bool		smooth;																		///<是否平滑(未用)

            bool		color_material;																///<是否使用颜色追踪材质

            bool		blend;																		///<是否开启混合

            BlendMode	blend_mode;																	///<混合模式

            Axis		up_axis;																    ///<高度图向上轴

        protected:	//材质本身数据

            struct
            {
                Color4f		Emission;																///<散射光
                Color4f		Ambient;																///<环境光
                Color4f		Diffuse;																///<漫反射
                Color4f		Specular;																///<镜面光
                float		Shininess;																///<镜面指数
            }Front,Back;

            bool		two_side;																	///<启用双面材质

            float		Transparency;																///<透明度

        protected:

            LightMode	light_mode;																	///<光照默式(0:关,1:顶点,2:象素)

            FixedArray<Texture *,mtcMax,nullptr> TextureList;										///<贴图,(未来将会使用MaterialTexture代替Texture，以支持每一层独立变换)

        protected:

            double		point_size;																	///<点尺寸

        public:

            Material();
            virtual ~Material()=default;

        public:

            void SetColor			(const Color4f &c)					{Color=c;}										///<设置全局颜色
            void SetColor			(float r,float g,float b,float a)	{Color.Set(r,g,b,a);}							///<设置全局颜色

            void SetDrawMode		(uint face,uint fm)					{draw_face=face;fill_mode=fm;}					///<设置绘制模式
            void SetFillMode		(uint fm)							{fill_mode=fm;}									///<设置填充模式
            void SetCullFace		(uint cf)							{cull_face=cf;}									///<设置面裁剪信息

            void SetDepthTest		(bool dt)							{depth_test=dt;}								///<设置深度测试
            void SetDepthFunc		(uint df)							{depth_func=df;}								///<设置深度测试处理函数
            void SetDepthTest		(bool dt,uint df)					{depth_test=dt;depth_func=df;}					///<设置深度测试
            void SetDepthMask		(bool dm)							{depth_mask=dm;}								///<设置深度遮罩

            void SetAlphaTest		(float at)							{alpha_test=at;}								///<设置Alpha测试
            void SetHSVClampColor   (const Color4f &f)                  {hsv_clamp_color=f;}                            ///<设置HSV颜色剔除数据
            void SetOutsideDiscard	(bool od)							{outside_discard=od;}							///<设置贴图出界放弃

            void SetBlend			(bool enable)						{blend=enable;}									///<设定是否开启混合
            void SetBlend			(const BlendMode &bm)				{blend_mode=bm;}								///<设置混合模式
            void SetBlend			(uint,uint);																		///<设定混合方式(传统方式，类似于glBlendFunc)

            void SetUptAxis		    (const Axis &axis)					{up_axis=axis;}								    ///<设置高度图向上轴

            void SetColorMaterial	(bool cm)							{color_material=cm;}							///<设置是否使用颜色追踪材质

            void SetLightMode		(const LightMode &lm)				{light_mode=lm;}								///<设置光照模式
            void SetTwoSide			(bool ts)							{two_side=ts;}									///<设置量启用双面材质

        public:

            void SetEmission		(const Color4f &c,bool b=false)		{if(b)Back.Emission	=c;else Front.Emission	=c;}
            void SetAmbient			(const Color4f &c,bool b=false)		{if(b)Back.Ambient	=c;else Front.Ambient	=c;}
            void SetDiffuse			(const Color4f &c,bool b=false)		{if(b)Back.Diffuse	=c;else Front.Diffuse	=c;}
            void SetSpecular		(const Color4f &c,bool b=false)		{if(b)Back.Specular	=c;else Front.Specular	=c;}

            void SetEmission		(const float c[4],bool b=false)		{if(b)Back.Emission	.Set(c[0],c[1],c[2],c[3]);else Front.Emission	.Set(c[0],c[1],c[2],c[3]);}
            void SetAmbient			(const float c[4],bool b=false)		{if(b)Back.Ambient	.Set(c[0],c[1],c[2],c[3]);else Front.Ambient	.Set(c[0],c[1],c[2],c[3]);}
            void SetDiffuse			(const float c[4],bool b=false)		{if(b)Back.Diffuse	.Set(c[0],c[1],c[2],c[3]);else Front.Diffuse	.Set(c[0],c[1],c[2],c[3]);}
            void SetSpecular		(const float c[4],bool b=false)		{if(b)Back.Specular	.Set(c[0],c[1],c[2],c[3]);else Front.Specular	.Set(c[0],c[1],c[2],c[3]);}

            void SetShininess		(float s,bool b=false)				{if(b)Back.Shininess=s;else Front.Shininess	=s;}

            void SetTransparency	(float t)							{Transparency=t;}

            bool SetTexture			(int mtc,Texture *tex)				{return TextureList.Set(mtc,tex);}				///<在设定通道设定纹理

        public:	//读取方法

            const Color4f &	GetColor()const						{return Color;}

            void			GetDrawMode(uint &d,uint &f)const	{d=draw_face;f=fill_mode;}			///<取得绘制模式
            uint			GetFillMode()const					{return fill_mode;}					///<取得填充模式
            uint			GetCullFace()const					{return cull_face;}					///<取得面裁剪信息

            bool			GetDepthTest()const					{return depth_test;}				///<取得是否做深度测试
            bool			GetDepthTest(uint &df)const			{df=depth_func;return depth_test;}	///<取得是否做深度测试以及处理方式
            uint			GetDepthFunc()const					{return depth_func;}				///<取得深度测试方式
            bool			GetDepthMask()const					{return depth_mask;}				///<取得是否做深度遮罩

            float			GetAlphaTest()const					{return alpha_test;}                ///<取得Alpha测试值

            const Color4f & GetHSVClampColor()const             {return hsv_clamp_color;}           ///<获取HSV颜色剔除的配置

            bool			GetOutsideDiscard()const			{return outside_discard;}			///<取得贴图是否出界放弃

            bool			GetBlend(BlendMode &bm)const		{bm=blend_mode;return blend;}		///<取得混合数据

            const Axis &	GetUpAxis()const				    {return up_axis;}				    ///<取得高度图向上轴

            bool			GetColorMaterial()const				{return color_material;}			///<读取是否使用颜色追踪材质

            LightMode       GetLightMode()const					{return light_mode;}				///<读取是否承接光照
            bool			GetTwoSide()const					{return two_side;}					///<读取是否启用双面材质

        public:

            const Color4f &	GetEmission	(bool b=false)const		{return b?Back.Emission	:Front.Emission	;}
            const Color4f &	GetAmbient	(bool b=false)const		{return b?Back.Ambient	:Front.Ambient	;}
            const Color4f &	GetDiffuse	(bool b=false)const		{return b?Back.Diffuse	:Front.Diffuse	;}
            const Color4f &	GetSpecular	(bool b=false)const		{return b?Back.Specular	:Front.Specular	;}

            const float	&	GetShininess(bool b=false)const		{return b?Back.Shininess:Front.Shininess;}

            const float	&	GetTransparency()const				{return Transparency;}

            const size_t	GetTextureNumber()const				{return TextureList.GetCount();}

            Texture *		GetTexture(int mtc)const			{return TextureList[mtc];}

            bool			ClearTexture(int mtc)				{return TextureList.Clear(mtc);}	///<清除指定贴图
            void			ClearAllTexture()					{return TextureList.ClearAll();}	///<清除所有贴图
        };//class Material

        extern Material *default_material;					///<缺省材质
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_MATERIAL_INCLUDE
