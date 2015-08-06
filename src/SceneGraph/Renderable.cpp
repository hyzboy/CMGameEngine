#include<hgl/graph/Renderable.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		const char VertexBufferName[vbtEnd][32]=
		{
			"Index",

			"Vertex",
			"Color",
			"Normal",
            "Tangent",
			"SecondColor",
			"FogCoord",

			"Ambient",
			"Specular",

			"TexCoord_Diffuse",
			"TexCoord_Alpha",
			"TexCoord_Normal",
            "TexCoord_Tangent",
			"TexCoord_Ambient",
			"TexCoord_Specular",

			"TexCoord0",	"TexCoord1",	"TexCoord2",	"TexCoord3",	"TexCoord4",	"TexCoord5",	"TexCoord6",	"TexCoord7",
			"TexCoord8",	"TexCoord9",	"TexCoord10",	"TexCoord11",	"TexCoord12",	"TexCoord13",	"TexCoord14",	"TexCoord15",
			"TexCoord16",	"TexCoord17",	"TexCoord18",	"TexCoord19",	"TexCoord20",	"TexCoord21",	"TexCoord22",	"TexCoord23",
			"TexCoord24",	"TexCoord25",	"TexCoord26",	"TexCoord27",	"TexCoord28",	"TexCoord29",	"TexCoord30",	"TexCoord31",

		};//const char VertexBufferName

		Renderable::Renderable()
		{
			vb_color_format=HGL_COLOR_NONE;

			material=nullptr;
			shader=nullptr;

			DrawStart=-1;
			DrawCount=-1;

			for(int i=0;i<vbtEnd;i++)
				vertex_buffer[i]=nullptr;

			for(int i=0;i<mtcMax;i++)
				TextureChannels[i]=vbtEnd;

			prim=0xFFFFFFFF;
		}

		Renderable::~Renderable()
		{
			if(material&&mtl_private)
				delete material;

			for(int i=0;i<vbtEnd;i++)
				if(vertex_buffer[i])
				{
					delete vertex_buffer[i];
					vertex_buffer[i]=nullptr;
				}

			material=nullptr;
		}

		/**
		* 设置材质数据
		* @param m 材质指针
		* @param mp 是否为私有(即不会在其它地方使用，本对像被清除时会连带清除这个材质)
		*/
		void Renderable::SetMaterial(Material *m,bool mp)
		{
			if(material&&mtl_private)
				delete material;

			material=m;
			mtl_private=mp;
		}

		/**
		* 设置顶点数据缓冲区
		* @param vbt 缓冲区类型
		* @param vb 数据缓冲区
		* @return 是否设置成功
		*/
		bool Renderable::SetVertexBuffer(VertexBufferType vbt,VertexBufferBase *vb)
		{
			if(vbt<0||vbt>=vbtEnd)
			{
				LOG_ERROR(OS_TEXT("Renderable::SetVertexBuffer设置的数据类型错误：")+OSString(vbt));
				return(false);
			}

			if(vertex_buffer[vbt])
			{
				if(vertex_buffer[vbt]==vb)return(true);	//有重复设的情况

				LOG_ERROR(OS_TEXT("Renderable::SetVertexBuffer设置的数据缓冲区已有数据"));
				return(false);
			}

			vertex_buffer[vbt]=vb;

			return(true);
		}

		/**
		* 清除顶点缓冲区数据
		* @param vbt 要清除的数据缓冲区类型
		* @return 是否清除成功
		*/
		bool Renderable::ClearVertexBuffer(VertexBufferType vbt)
		{
			if(vbt<0||vbt>=vbtEnd)
			{
				LOG_ERROR(OS_TEXT("Renderable::ClearVertexBuffer要清除的数据类型错误：")+OSString(vbt));
				return(false);
			}

			if(!vertex_buffer[vbt])
			{
				LOG_ERROR(OS_TEXT("Renderable::ClearVertexBuffer要清除的数据缓冲区没有数据：")+OSString(vbt));
				return(false);
			}

			delete vertex_buffer[vbt];
			vertex_buffer[vbt]=nullptr;

			return(true);
		}

		VertexBufferBase *Renderable::GetVertexBuffer(VertexBufferType vbt)						///<取得顶点缓冲区数据
		{
			if(vbt<0||vbt>=vbtEnd)return(nullptr);

			return(vertex_buffer[vbt]);
		}

		VertexBufferBase *Renderable::GetTexCoord(int mtc,VertexBufferType *vbt)				///<取得贴图坐标对应的缓冲区
		{
			if(mtc<mtcDiffuse||mtc>=mtcMax)return(nullptr);

			int index=TextureChannels[mtc];

			if(index<vbtDiffuseTexCoord||index>=vbtEnd)
			{
				if(vbt)
					*vbt=vbtEnd;

				return(nullptr);
			}

			if(vbt)
				*vbt=(VertexBufferType)index;

			return(vertex_buffer[index]);
		}

		bool Renderable::SetTexCoord(int mtc,VertexBufferType vbt)
		{
			if(mtc<mtcDiffuse||mtc>=mtcMax
			 ||mtc==mtcNormal					//法线贴图不需要指定坐标
			 ||mtc==mtcTangent                  //切线贴图不需要指定坐标
			 ||mtc==mtcPalette					//调色板不需要指定坐标
			 ||mtc==mtcShadow)					//阴影贴图不需要指定坐标
			 return(false);

			if(vbt<vbtDiffuseTexCoord
			 ||vbt>=vbtDiffuseTexCoord+HGL_MAX_TEXTURE_UNITS)	//超出最大贴图数量
			 return(false);

			TextureChannels[mtc]=vbt;

			return(true);
		}

		bool Renderable::SetDrawCount(int start,int count)
		{
			if(start<0||count<=0)return(false);

			const int max_count=GetDrawCount();

			if(start+count>max_count)return(false);

			DrawStart=start;
			DrawCount=count;

			return(true);
		}

		/**
		* 取得指定的要绘制的数据数量
		*/
		void Renderable::GetDrawCount(int &start,int &count)
		{
			if(DrawStart==-1
			 ||DrawCount==-1)
			{
				start=0;
				count=GetDrawCount();

				return;
			}

			start=DrawStart;
			count=DrawCount;
		}

		/**
		* 取得可绘制数据数量
		* @return 可绘制数量数量
		* @return -1 出错
		*/
		int Renderable::GetDrawCount()
		{
			if(vertex_buffer[vbtIndex])
				return vertex_buffer[vbtIndex]->GetCount();

			if(vertex_buffer[vbtVertex])
				return vertex_buffer[vbtVertex]->GetCount();

			return(-1);
		}
	}//namespace graph
}//namespace hgl
