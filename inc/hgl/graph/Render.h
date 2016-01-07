#ifndef HGL_GRAPH_RENDER_INCLUDE
#define HGL_GRAPH_RENDER_INCLUDE

#include<hgl/type/List.h>
#include<hgl/type/ResManage.h>
#include<hgl/VectorMath.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/Renderable.h>
#include<hgl/graph/RenderDevice.h>
namespace hgl
{
	namespace graph
	{
		bool SaveScreenToBMP(const u16char * =0,int=-1,int=-1);										///<保存当前屏幕内容到指定BMP文件
		bool SaveScreenToTGA(const u16char * =0,int=-1,int=-1);										///<保存当前屏幕内容到指定TGA文件
	}//namespace graph

	namespace graph
	{
		//视口与裁剪函数
		void GetViewport(int &,int &,int &,int &);													///<取得视口
		void SetViewport(int,int,int,int);															///<设置视口
		void GetScreenSize(int &,int &);															///<取得绘制区尺寸
		void GetScreenSize(float &,float &);														///<取得绘制区尺寸
		int GetScreenWidth();																		///<取得屏幕宽
		int GetScreenHeight();																		///<取得屏幕高
		void OpenScissor(int,int,int,int);															///<开启裁剪区
		void CloseScissor();																		///<关闭裁剪区
		inline void OpenScissor(const int *v){OpenScissor(v[0],v[1],v[2],v[3]);}					///<开启裁剪区

		//清屏函数
		void SetClearColor(float red=0.0f,float green=0.0f,float blue=0.0f);						///<设定清屏颜色
		void SetClearDepth(float clear_depth=0.0f);													///<设定清屏后的深度值
		void SetClearBuffer(bool color=true,bool depth=true,bool stencil=true,bool accum=true);		///<设定清屏缓冲区
		void ClearScreen();																			///<清除缓冲区
		void ClearColorBuffer();																	///<清除颜色缓冲区
		void ClearDepthBuffer();																	///<清除深度缓冲区
		void ClearColorDepthBuffer();																///<清除颜色与深度缓冲区
		void ClearStencilBuffer();																	///<清除腊纸缓冲区
		void ClearAccumBuffer();																	///<清除累积缓冲区

		//深度缓冲区相关函数
		void OpenDepthTest(uint);																	///<开启深度测试
		void CloseDepthTest();																		///<关闭深度测试
		void SetDepthMask(bool);																	///<设置深度遮罩

		//面相关
		void SetPolygonMode(uint,uint);																///<设置多边形绘制模式
		void OpenCullFace(uint);																	///<开启面裁剪
		void CloseCullFace();																		///<关闭面裁剪
		void SetFrontFace(uint);																	///<设置三角面正面绘制方向
	}//namespace graph

	namespace graph
	{
		class Bitmap1D;
		class Bitmap2D;
		class Bitmap3D;
	}//namespace graph

	//贴图函数
	namespace graph
	{
//		Texture1D *CreateTexture1D();							//创建1D贴图
		Texture2D *CreateTexture2D();							//创建2D贴图

		/**
		* 创建2D贴图
		* @param width 宽
		* @param height 高
		* @param video_format 显存格式
		*/
		inline Texture2D *CreateTexture2D(int width,int height,uint video_format)
		{
			Texture2D *tex=CreateTexture2D();

			tex->SetImage(width,height,0,0,HGL_SF_NONE,video_format);

			return tex;
		}

		class Texture2DList:public CusObjectList<Texture2D>
		{
			Texture2D *CreateObject()
			{
				return(CreateTexture2D());
			}

			void DeleteObject(Texture2D *tex)
			{
				delete tex;
			}
		};//class Texture2DList

		//贴图采样器数据结构
		struct TextureSampler
		{
			int id;									//采样器ID

			uint texture_type;						//贴图类型

			uint mag_filter[3];						//放大过滤
			uint min_filter[3];						//缩小过滤
		};//struct TextureSampler

		//材质
		Material *CreateMaterial();					///<创建材质

		class MaterialList:public CusObjectList<Material>
		{
			Material *CreateObject()
			{
				return(CreateMaterial());
			}

			void DeleteObject(Material *mtl)
			{
				delete mtl;
			}

		public:

			virtual ~MaterialList()
			{
				Clear();
			}
		};//class MaterialList
	}//namespace graph

	namespace graph
	{
		//Shader着色程序
		class Shader;

		/**
		* 创建一个着色程序
		* @param vertex_shader 顶点程序代码
		* @param control_shader 曲面镶嵌控制程序代码
		* @param evaluation_shader 曲面镶嵌评估程序代码
		* @param geometry_shader 几何程序代码
		* @param fragment_shader 片断程序代码
		* @return 创建成功的GLSL程序指针
		* @return NULL 创建失败
		*/
		Shader *CreateShader(	const char *vertex_shader,
								const char *control_shader,
								const char *evaluation_shader,
								const char *geometry_shader,
								const char *fragment_shader);										///<创建一个着色程序

		/**
		* 创建一个着色程序
		* @param vertex_shader 顶点程序代码
		* @param fragment_shader 片断程序代码
		* @return 创建成功的GLSL程序指针
		* @return NULL 创建失败
		*/
		inline Shader *CreateShader(const char *vertex_shader,const char *fragment_shader)			///<创建一个着色程序
				{return CreateShader(vertex_shader,0,0,0,fragment_shader);}

		/**
		* 创建一个着色程序
		* @param vertex_shader 顶点程序代码
		* @param geometry_shader 几何程序代码
		* @param fragment_shader 片断程序代码
		* @return 创建成功的GLSL程序指针
		* @return NULL 创建失败
		*/
		inline Shader *CreateShader(const char *vertex_shader,const char *geometry_shader,const char *fragment_shader)			///<创建一个着色程序
				{return CreateShader(vertex_shader,0,0,geometry_shader,fragment_shader);}

		//Shader数据块
		class ShaderDataBlock;

		/**
		* 创建一个Shader数据块
		* @param shader_name 数据块在shader程序中的名称
		* @param block_size 数据块长度
		* @param data 数据内容
		* @param update_level 更新级别，默认为HGL_DYNAMIC_DRAW
		*/
		ShaderDataBlock *CreateShaderDataBlock(const char *shader_name,int block_size,void *data=nullptr,int update_level=0);		///<创建数据块
	}//namespace graph

	namespace graph
	{
		//位于SceneOrient.CPP中
		extern Matrix4f Ortho2DMatrix;																///<全局2D视图矩阵

		//位于InlineRenderable.CPP中

		extern Texture2D *BlackWhiteGrid;															///<黑白格贴图
		extern Texture2D *GrayWhiteGrid;                                                            ///<灰白格贴图
		extern Texture2D *BlueWhiteGrid;                                                            ///<蓝白格贴图

		extern Renderable *SolidCube;																///<实心立方体
		extern Renderable *WireCube;																///<白色线框立方体
		extern Renderable *SolidRect;																///<实心块2D矩形
		extern Renderable *WireRect;																///<线框2D矩形
		extern Renderable *TextureRect;																///<带贴图的2D矩形
		extern Renderable *SolidCircle;																///<实心圆
		extern Renderable *WireCircle;																///<空心圆

				bool DirectRender(Renderable *obj,const Matrix4f *proj,const Matrix4f *mv);			///<直接渲染一个可渲染数据

		inline	bool DirectRender(Renderable *obj)													///<直接渲染一个可渲染数据
		{
			return DirectRender(obj,0,0);
		}

		inline	bool DirectRender(Renderable *obj,const Matrix4f *proj)								///<直接渲染一个可渲染数据
		{
			return DirectRender(obj,proj,0);
		}

		inline	bool DirectRender(Renderable *obj,const Matrix4f &proj)								///<直接渲染一个可渲染数据
		{
			return DirectRender(obj,&proj,0);
		}

		inline	bool DirectRender(Renderable *obj,const Matrix4f *proj,const Matrix4f *mv,const Matrix4f *ltw)
		{
			const Matrix4f fin=(*mv)*(*ltw);

			return DirectRender(obj,proj,&fin);
		}

		inline	bool DirectRender(Renderable *obj,const Matrix4f &proj,const Matrix4f &mv)
		{
			return DirectRender(obj,&proj,&mv);
		}

		inline	bool DirectRender(Renderable *obj,const Matrix4f &proj,const Matrix4f &mv,const Matrix4f &ltw)
		{
			const Matrix4f fin=mv*ltw;

			return DirectRender(obj,&proj,&fin);
		}

		inline	bool DirectRender2D(Renderable *obj)
		{
			return DirectRender(obj,&Ortho2DMatrix,0);
		}

		inline	bool DirectRender2D(Renderable *obj,const Matrix4f *mv)
		{
			return DirectRender(obj,&Ortho2DMatrix,mv);
		}

		inline	bool DirectRender2D(Renderable *obj,const Matrix4f *mv,const Matrix4f *ltw)
		{
			const Matrix4f fin=(*mv)*(*ltw);

			return DirectRender(obj,&Ortho2DMatrix,&fin);
		}

		inline	bool DirectRender2D(Renderable *obj,const Matrix4f &mv)
		{
			return DirectRender(obj,&Ortho2DMatrix,&mv);
		}

		inline	bool DirectRender2D(Renderable *obj,const Matrix4f &mv,const Matrix4f &ltw)
		{
			const Matrix4f fin=mv*ltw;

			return DirectRender(obj,&Ortho2DMatrix,&fin);
		}

		/**
		* 渲染一个贴图在2D空间中
		* @param l 左边界
		* @param t 上边界
		* @param w 宽度
		* @param h 高度
		* @param color 颜色系数
		* @return 是否成功
		*/
		inline bool Render2DRGBTextureRect	(Texture2D *tex,float l,float t,float w,float h,const Color4f &color=Color4f(1,1,1,1))				///<渲染一个2DRGB贴图
		{
			if(!TextureRect||!tex||!w||!h)return(false);

			Material *mtl=TextureRect->GetMaterial();

			mtl->SetTexture(mtcDiffuse,tex);

			if(color.a>=1.0f)
				mtl->SetBlend(false);
			else
			{
				mtl->SetBlend(true);
				mtl->SetBlend(HGL_BLEND_SRC_ALPHA,HGL_BLEND_ONE_MINUS_SRC_ALPHA);
			}

			mtl->SetColor(color);

			TextureRect->AutoCreateShader(true);

			const Matrix4f mat=translate(l,t,0)*scale(w,h,1.0f);

			const bool result=DirectRender2D(TextureRect,mat);

			mtl->ClearTexture(mtcDiffuse);

			return result;
		}

		/**
		* 渲染一个贴图在2D空间中
		* @param l 左边界
		* @param t 上边界
		* @param w 宽度
		* @param h 高度
		* @param color 颜色系数
		* @return 是否成功
		*/
		inline bool Render2DRGBATextureRect(Texture2D *tex,float l,float t,float w,float h,const Color4f &color=Color4f(1,1,1,1))				///<渲染一个2DRGBA贴图
		{
			if(!TextureRect||!tex||!w||!h)return(false);

			Material *mtl=TextureRect->GetMaterial();

			mtl->SetTexture(mtcDiffuse,tex);

			mtl->SetBlend(true);
			mtl->SetBlend(HGL_BLEND_SRC_ALPHA,HGL_BLEND_ONE_MINUS_SRC_ALPHA);
			mtl->SetColor(color);

			TextureRect->AutoCreateShader(true);

			const Matrix4f mat=translate(l,t,0)*scale(w,h,1.0f);

			const bool result=DirectRender2D(TextureRect,mat);

			mtl->ClearTexture(mtcDiffuse);

			return result;
		}

		/**
		* 渲染一个贴图在2D空间中
		* @param l 左边界
		* @param t 上边界
		* @param sw 宽度缩放比
		* @param sh 高度缩放比
		* @param color 颜色系数
		* @return 是否成功
		*/
		inline bool Render2DTextureRGB		(Texture2D *tex,float l,float t,float sw=1.0f,float sh=1.0f,const Color4f &color=Color4f(1,1,1,1))				///<渲染一个2DRGB贴图
		{
			if(!tex||!sw||!sh)return(false);

			return Render2DRGBTextureRect(tex,l,t,float(tex->GetWidth())*sw,float(tex->GetHeight())*sh,color);
		}

		/**
		* 渲染一个贴图在2D空间中
		* @param l 左边界
		* @param t 上边界
		* @param sw 宽度缩放比
		* @param sh 高度缩放比
		* @param color 颜色系数
		* @return 是否成功
		*/
		inline bool Render2DTextureRGBA	(Texture2D *tex,float l,float t,float sw=1.0f,float sh=1.0f,const Color4f &color=Color4f(1,1,1,1))				///<渲染一个2DRGBA贴图
		{
			if(!tex||!sw||!sh)return(false);

			return Render2DRGBATextureRect(tex,l,t,float(tex->GetWidth())*sw,float(tex->GetHeight())*sh,color);
		}

		//渲染列表渲染
		class SceneNode;
		struct Camera;

		typedef List<const SceneNode *> RenderList;													///<渲染列表类型重定义

		bool Render(const RenderList *,const Matrix4f *,const Matrix4f *);							///<使用指定矩阵渲染一个渲染列表
		bool Render(const RenderList *,const Camera *);												///<渲染一个渲染列表
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_INCLUDE
