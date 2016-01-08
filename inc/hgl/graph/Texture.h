#ifndef HGL_GRAPH_TEXTURE_INCLUDE
#define HGL_GRAPH_TEXTURE_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
		class Bitmap2D;

        extern int HGL_MAX_VS_TEXTURE_UNITS;        ///<Vertex Shader中的最大纹理单元
        extern int HGL_MAX_FS_TEXTURE_UNITS;        ///<Fragment Shader中的最大纹理单元
        extern int HGL_MAX_GS_TEXTURE_UNITS;        ///<Geometry Shader中的最大纹理单元
        extern int HGL_MAX_TCS_TEXTURE_UNITS;       ///<Tess Control Shader中的最大纹理单元
        extern int HGL_MAX_TES_TEXTURE_UNITS;       ///<Tess Evaluation Shader中的最大纹理单元
        extern int HGL_MAX_CS_TEXTURE_UNITS;        ///<Compute Shader中的最大纹理单元

        extern int HGL_MAX_TEXTURE_SIZE;            ///<最大纹理尺寸
        extern int HGL_MAX_CUBE_MAP_SIZE;           ///<最大CubeMap纹理尺寸

		/**
		* 贴图类
		*/
		class Texture
		{
		public:

			/**
			* 贴图加载预处理
			*/
			enum LoadTextureProcess:uint
			{
				ltGenMipmaps=1<<0,		//生成mipmaps
			};//enum LoadTextureProcess

		protected:

			uint texture_id;																		///<纹理ID

			uint type;																				///<贴图类型

			uint video_format;																		///<显存格式

			uint min_filter,mag_filter;																///<放大和缩小过滤方式

			void *bitmap_data;																		///<原始点阵数据(不一定存在)

		public:

							Texture();																//请使用CreateTexture()来创建贴图,函数在Render.h中定义
			virtual			~Texture(){if(bitmap_data)hgl_free(bitmap_data);}

		public:

					int		GetID		    ()const{return texture_id;}								///<取得纹理ID
					uint	GetType			()const{return type;}									///<取得类型
			virtual uint	GetCoordNumber	()const=0;												///<取得坐标数量
					uint	GetVideoFormat	()const{return video_format;}							///<取得显存中的颜色格式

		public:

			virtual void	SetMinFilter	(uint f){min_filter=f;}									///<设置缩小过滤
			virtual void	SetMagFilter	(uint f){mag_filter=f;}									///<设置放大过滤
		};//class Texture

// 		/**
// 		* 1D贴图
// 		*/
// 		class Texture1D:public Texture
// 		{
// 			int length;																									///<长
//
// 		public:	//
//
// 					uint	GetCoordNumber	()const{return 1;}
// 					int		GetLength		()const{return length;}
//
// 		public:
//
// 					//bool	LoadFile		(const u16char *,uint vf=0,uint ltp=0);										///<从文件加载一个贴图
//
// 			virtual bool	SetImage		(uint l,void *data,uint size,uint sf,uint vf=0);							///<创建1D贴图数据
// 		};//class Texture1D

		/**
		* 2D贴图
		*/
		class Texture2D:public Texture
		{
		protected:

			int width,height;																							///<长、宽

			uint wrap_s,wrap_t;

		public:
					uint	GetCoordNumber	()const{return 2;}															///<取得坐标数量
					int		GetWidth		()const{return width;}
					int		GetHeight		()const{return height;}

		public:

							Texture2D();
							~Texture2D();

					bool	LoadFile		(const OSString &,uint vf=0,uint ltp=0);									///<从文件加载一个贴图

			virtual bool	SetImage		(uint w,uint h,void *data,uint size,TSF sf,uint vf=0,uint ltp=0)=0;			///<创建2D贴图数据
			virtual bool	ChangeImage		(uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf)=0;				///<更改2D贴图数据

					bool	SetImage		(Bitmap2D *,uint vf=0,uint ltp=0);											///<从位图创建一个贴图
					bool	ChangeImage		(Bitmap2D *,uint l,uint t);													///<更改贴图数据

			virtual void	GetMipmapLevel	(int &,int &)=0;															///<取得贴图mipmaps级别

			virtual int		GetImage		(void *data_pointer,TSF fmt,int level=0)=0;									///<取得2D贴图数据

			virtual void	SetMinFilter	(uint)=0;																	///<设置缩小过滤
			virtual void	SetMagFilter	(uint)=0;																	///<设置放大过滤

			virtual void	SetWrapS		(uint)=0;																	///<设置横向寻址方式
			virtual void	SetWrapT		(uint)=0;																	///<设置纵向寻址方式
		};//class Texture2D

		class Texture1DArray:public Texture{};
        class Texture2DArray:public Texture{};
        class Texture3D:public Texture{};
        class TextureCubeMap:public Texture{};
        class TextureCubeMapArray:public Texture{};

// 		class Texture3D:public Texture
// 		{
// 			int width,height,length;																					///<长、宽、高
//
// 		public:
//
// 					int		GetWidth		(){return width;}
// 					int		GetHeight		(){return height;}
// 					int		GetLength		(){return length;}
//
// 		public:
//
// 					bool	LoadFile		(const u16char *,uint vf=0,uint ltp=0);										///<从文件加载一个贴图
//
// 			virtual bool	SetImageData3D	(uint w,uint h,uint l,void *data,uint size,uint sf,uint vf=0);				///<创建3D贴图数据
// 		};//class Texture3D
//
// 		class Texture2DArray:public Texture
// 		{
// 			int width,height,layer;																						///<宽、高、层数
//
// 		public:
//
// 					int		GetWidth		(){return width;}
// 					int		GetHeight		(){return height;}
// 					int		GetLayer		(){return layer;}
//
// 		public:
//
// 					bool	LoadFile			(const u16char *,uint vf=0,uint ltp=0);									///<从文件加载一个贴图
//
// 			virtual bool	SetImageData2DArray	(uint w,uint h,uint l,void *data,uint size,uint sf,uint vf=0);			///<创建2D贴图阵列数据
// 		};//class Texture2DArray
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_INCLUDE
