#include<hgl/graph/Renderable.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		Renderable::Renderable(VertexArray *rd,Material *m)
		{
            va=rd;
            shader=nullptr;
            bind_shader=nullptr;

			if(m)
				material=m;
			else
				material=default_material;

			DrawStart=-1;
			DrawCount=-1;

            hgl_set(TextureChannels,vbtEnd,mtcMax);

            Init();
		}

		Renderable::~Renderable()
		{
            Clear();
		}

		/**
		* 设置材质数据
		* @param m 材质指针
		*/
		bool Renderable::SetMaterial(Material *m)
		{
            if(!va)return(false);

			material=m;
            return(true);
		}

		VertexBufferBase *Renderable::GetTexCoord(int mtc,VertexBufferType *vbt)				///<取得贴图坐标对应的缓冲区
		{
            if(!va)return(nullptr);
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

			return va->GetVertexBuffer((VertexBufferType)index);
		}

		bool Renderable::SetTexCoord(int mtc,VertexBufferType vbt)
		{
            if(!va)return(false);

			if(mtc<mtcDiffuse||mtc>=mtcMax
			 ||mtc==mtcNormal					//法线贴图不需要指定坐标
			 ||mtc==mtcTangent                  //切线贴图不需要指定坐标
			 ||mtc==mtcPalette					//调色板不需要指定坐标
			 ||mtc==mtcShadow)					//阴影贴图不需要指定坐标
			 return(false);

			if(vbt<vbtDiffuseTexCoord)	//超出最大贴图数量
			 return(false);

			TextureChannels[mtc]=vbt;

			return(true);
		}

		bool Renderable::SetDrawCount(int start,int count)
		{
            if(!va)return(false);

			if(start<0||count<=0)return(false);

			const int max_count=va->GetDrawCount();

			if(start+count>max_count)return(false);

			DrawStart=start;
			DrawCount=count;

			return(true);
		}

		/**
		* 取得指定的要绘制的数据数量
		*/
		bool Renderable::GetDrawCount(int &start,int &count)
		{
            if(!va)return(false);

			if(DrawStart==-1
			 ||DrawCount==-1)
			{
				start=0;
				count=va->GetDrawCount();

				return(false);
			}

			start=DrawStart;
			count=DrawCount;
            return(true);
		}
	}//namespace graph
}//namespace hgl
