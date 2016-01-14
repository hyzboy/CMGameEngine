#include<hgl/graph/Renderable.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		Renderable::Renderable(RenderableData *rd,Shader *s)
		{
            data=rd;
            shader=s;
            bind_shader=nullptr;

			material=nullptr;

			DrawStart=-1;
			DrawCount=-1;

            hgl_set(TextureChannels,vbtEnd,mtcMax);

            Init();
		}

		Renderable::~Renderable()
		{
			if(material&&mtl_private)
				delete material;

			material=nullptr;

            Clear();
		}

		/**
		* 设置材质数据
		* @param m 材质指针
		* @param mp 是否为私有(即不会在其它地方使用，本对像被清除时会连带清除这个材质)
		*/
		bool Renderable::SetMaterial(Material *m,bool mp)
		{
            if(!data)return(false);

			if(material&&mtl_private)
				delete material;

			material=m;
			mtl_private=mp;
            return(true);
		}

		VertexBufferBase *Renderable::GetTexCoord(int mtc,VertexBufferType *vbt)				///<取得贴图坐标对应的缓冲区
		{
            if(!data)return(nullptr);
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
            if(!data)return(false);

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
            if(!data)return(false);

			if(start<0||count<=0)return(false);

			const int max_count=data->GetDrawCount();

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
            if(!data)return(false);

			if(DrawStart==-1
			 ||DrawCount==-1)
			{
				start=0;
				count=GetDrawCount();

				return;
			}

			start=DrawStart;
			count=DrawCount;
            return(true);
		}
	}//namespace graph
}//namespace hgl
