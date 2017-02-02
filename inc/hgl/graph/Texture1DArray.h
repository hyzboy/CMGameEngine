#ifndef HGL_GRAPH_TEXTURE_1D_ARRAY_INCLUDE
#define HGL_GRAPH_TEXTURE_1D_ARRAY_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 1D贴图阵列
		*/
		class Texture1DArray:public Texture
		{
		protected:

			uint length,count;																							///<长度、数量

			uint wrap;

		public:

					uint	GetType         ()const{return HGL_TEXTURE_1D_ARRAY;}override								///<取得类型
					uint	GetBindingType  ()const{return HGL_TEX_BIND_1D_ARRAY;}override							    ///<取得绑定类型
					int		GetLength		()const{return length;}														///<取得纹理长度
					int		GetCount		()const{return count;}														///<取得纹理数量

		public:

							Texture1DArray();
			virtual			~Texture1DArray();

					bool	LoadFile		(const OSString &,uint vf=0,uint ltp=0);									///<从文件加载一个贴图

			virtual bool	SetImage		(uint,uint,void *,uint,TSF,uint vf=0,uint ltp=0);							///<创建1D贴图阵列数据
					bool	SetImage		(uint l,uint c,TSF tsf){return SetImage(l,c,nullptr,0,tsf);}				///<创建1D贴图阵列数据
			virtual bool	ChangeImage		(uint,uint,uint,uint,void *,uint,TSF);										///<更改1D贴图阵列数据

			virtual int		GetImage		(void *,TSF,int level=0);													///<取得1D贴图阵列数据

			virtual void	SetWrap			(uint);																		///<设置寻址方式
		};//class Texture1DArray
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_1D_ARRAY_INCLUDE
