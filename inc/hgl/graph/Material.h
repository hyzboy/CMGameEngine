#ifndef HGL_GRAPH_MATERIAL_INCLUDE
#define HGL_GRAPH_MATERIAL_INCLUDE

#include<hgl/VectorMath.h>
#include<hgl/type/DataType.h>
#include<hgl/type/Color4f.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/SceneOrient.h>
#include<GL/glew.h>
namespace hgl
{
	namespace graph
	{
		class Texture;

		/**
		* 贴图通道数据枚举
		*/
		enum MaterialTextureChannels
		{
			mtcDiffuse=0,				///<漫反射
			mtcAmbient,					///<环境光
			mtcSpecular,				///<镜面光
			mtcNormal,					///<法线
			mtcLight,					///<光照贴图
			mtcAlpha,					///<透明度贴图
			mtcShadow,					///<阴影

			mtcPalette,					///<调色板

			mtcHeight,					///<高度图
			mtcNormalHeight,			///<法线+高度

			mtcMax						///<最大贴图通道
		};//enum MaterialTextureChannels

		extern const char MaterialTextureName[mtcMax][32];	//贴图通道名称(用在fragment shader中)

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
		* 材质
		*/
		class Material
		{
			friend Material *CreateMaterial();

		protected:

			Color4f		Color;																		///<颜色

			uint		draw_face;																	///<绘制的面
			uint		fill_mode;																	///<填充方式

			uint		cull_face;																	///<裁剪面,0:不裁,FACE_FRONT:正面,FACE_BACK:背面

			bool		depth_test;																	///<深度测试
			uint		depth_func;																	///<深度处理方式
			bool		depth_mask;																	///<深度遮罩

			float		alpha_test;																	///<alpha测试值,0:全部不显示,1:全部显示

			bool		outside_discard;															///<贴图出界放弃

			bool		smooth;																		///<是否平滑(未用)

			bool		color_material;																///<是否使用颜色追踪材质

			bool		blend;																		///<是否开启混合

			BlendMode	blend_mode;																	///<混合模式

			Axis		height_axis;																///<高度图向上轴

		protected:	//材质本身数据

			Color4f		Emission;
			Color4f		Ambient;
			Color4f		Diffuse;																	///<漫反射
			Color4f		Specular;																	///<反射指数
			float		Shininess;																	///<镜面指数
			float		Transparency;																///<透明度

		protected:

			bool		Light;																		///<承接光照(默认开)

			//测试用
			Vector3f	LightPosition;																///<光源位置
			float		GlobalLightIntensity;														///<全局环境光强度

			Texture	*	texture_list[mtcMax];														///<贴图

		protected:

			double point_size;																		///<点尺寸

		public:

			Material();				//请使用函数CreateMaterial来创建材质，函数在Render.H中定义
			virtual ~Material();

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
			void SetOutsideDiscard	(bool od)							{outside_discard=od;}							///<设置贴图出界放弃

			void SetBlend			(bool enable)						{blend=enable;}									///<设定是否开启混合
			void SetBlend			(const BlendMode &bm)				{blend_mode=bm;}								///<设置混合模式
			void SetBlend			(uint,uint);																		///<设定混合方式(传统方式，类似于glBlendFunc)

			void SetHeightAxis		(Axis axis)							{height_axis=axis;}								///<设置高度图向上轴

			void SetColorMaterial	(bool cm)							{color_material=cm;}							///<设置是否使用颜色追踪材质

			void SetLight			(bool l)							{Light=l;}										///<设置是否承接光照
			void SetLightPosition	(const Vector3f &lp)				{LightPosition=lp;}								///<设置光源位置
			void SetLightPosition	(float x,float y,float z)			{LightPosition.set(x,y,z);}						///<设置光源位置
			void SetGlobalLightIntensity(float gli)						{GlobalLightIntensity=gli;}						///<设置全局环境光强度

		public:

			void SetEmission		(const Color4f &c)					{Emission=c;}
			void SetAmbient			(const Color4f &c)					{Ambient=c;}
			void SetDiffuse			(const Color4f &c)					{Diffuse=c;}
			void SetSpecular		(const Color4f &c)					{Specular=c;}

			void SetEmission		(float r,float g,float b,float a)	{Emission.Set(r,g,b,a);}
			void SetAmbient			(float r,float g,float b,float a)	{Ambient.Set(r,g,b,a);}
			void SetDiffuse			(float r,float g,float b,float a)	{Diffuse.Set(r,g,b,a);}
			void SetSpecular		(float r,float g,float b,float a)	{Specular.Set(r,g,b,a);}

			void SetEmission		(const float c[4])					{Emission.Set(c[0],c[1],c[2],c[3]);}
			void SetAmbient			(const float c[4])					{Ambient.Set(c[0],c[1],c[2],c[3]);}
			void SetDiffuse			(const float c[4])					{Diffuse.Set(c[0],c[1],c[2],c[3]);}
			void SetSpecular		(const float c[4])					{Specular.Set(c[0],c[1],c[2],c[3]);}

			void SetShininess		(float s)							{Shininess=s;}
			void SetTransparency	(float t)							{Transparency=t;}

			bool SetTexture			(int mtc,Texture *);

		public:	//读取方法

			const Color4f &	GetColor()const						{return Color;}

			void			GetDrawMode(uint &d,uint &f)const	{d=draw_face;f=fill_mode;}			///<取得绘制模式
			uint			GetFillMode()const					{return fill_mode;}					///<取得填充模式
			uint			GetCullFace()const					{return cull_face;}					///<取得面裁剪信息

			bool			GetDepthTest()const					{return depth_test;}				///<取得是否做深度测试
			bool			GetDepthTest(uint &df)const			{df=depth_func;return depth_test;}	///<取得是否做深度测试以及处理方式
			uint			GetDepthFunc()const					{return depth_func;}				///<取得深度测试方式
			bool			GetDepthMask()const					{return depth_mask;}				///<取得是否做深度遮罩

			float			GetAlphaTest()const					{return alpha_test;}				///<取得Alpha测试值
			bool			GetOutsideDiscard()const			{return outside_discard;}			///<取得贴图是否出界放弃

			bool			GetBlend(BlendMode &bm)const		{bm=blend_mode;return blend;}		///<取得混合数据

			const Axis		GetHeightAxis()const				{return height_axis;}				///<设置高度图向上轴

			bool			GetColorMaterial()const				{return color_material;}			///<读取是否使用颜色追踪材质

			bool			GetLight()const						{return Light;}						///<读取是否承接光照
			const Vector3f &GetLightPosition()const				{return LightPosition;}				///<读取光源位置
			float			GetGlobalLightIntensity()const		{return GlobalLightIntensity;}		///<读取全局环境光强度

		public:

			const Color4f &	GetEmission()const					{return Emission;}
			const Color4f &	GetAmbient()const					{return Ambient;}
			const Color4f &	GetDiffuse()const					{return Diffuse;}
			const Color4f &	GetSpecular()const					{return Specular;}

			const float	&	GetShininess()const					{return Shininess;}
			const float	&	GetTransparency()const				{return Transparency;}

			int GetTextureNumber()const
			{
				int count=0;

				for(int i=mtcDiffuse;i<mtcMax;i++)
					if(texture_list[i])
						count++;

				return(count);
			}

			Texture *GetTexture(int mtc)const
			{
				if(mtc<mtcDiffuse||mtc>=mtcMax)
					return(0);

				return(texture_list[mtc]);
			}

			bool ClearTexture(int mtc);																///<清除指定贴图
			void ClearTexture();																	///<清除所有贴图
		};//class Material
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_MATERIAL_INCLUDE
