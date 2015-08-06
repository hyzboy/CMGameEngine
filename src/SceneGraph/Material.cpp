#include<hgl/graph/Material.h>
#include<hgl/type/BaseString.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace graph
	{
		extern const char MaterialTextureName[mtcMax][32]=
		{
			"Diffuse",
			"Ambient",
			"Specular",
			"Normal",
            "Tangent",
            "Bitangent",
			"Light",
			"Alpha",
			"Shadow",

			"Palette",

			"Height",
			"NormalHeight",
		};

		Material *CreateMaterial()
		{
			return(new Material);
		}
	}//namespace graph

	namespace graph
	{
		Material::Material()
		{
			Color.Set(1.0f,1.0f,1.0f,1.0f);

			draw_face=HGL_FACE_FRONT_AND_BACK;
			fill_mode=HGL_DRAW_FILL;
			cull_face=0;

			depth_test=true;
			depth_func=HGL_DEPTH_LESS;
			depth_mask=true;

			alpha_test=0.0f;

			outside_discard=false;

			blend=false;

			SetBlend(	HGL_BLEND_SRC_ALPHA,
						HGL_BLEND_ONE_MINUS_SRC_ALPHA);

			height_axis=HGL_AXIS_Y;

			color_material=true;

			Light=false;
			LightPosition=Vector3f(0,0,0);

			Emission.Set(0.0f,0.0f,0.0f,1.0f);
			Ambient.Set(0.25f,0.25f,0.25f,1.0f);
			Diffuse.Set(1.0f,1.0f,1.0f,1.0f);
			Specular.Set(0.95f,0.95f,0.95f,1.0f);

			Shininess=40.0f;
			Transparency=1.0f;

			for(int i=0;i<mtcMax;i++)
				texture_list[i]=0;
		}

		Material::~Material()
		{
			ClearTexture();
		}

		/**
		* 设定混合方式(传统方式，类似于glBlendFunc)
		* @param src 源数据混合因子
		* @param dst 目标数据混合因子
		*/
		void Material::SetBlend	(uint src,uint dst)
		{
			BlendMode bm;

			bm.rgb.src=src;
			bm.rgb.dst=dst;
			bm.rgb.func=HGL_BLEND_ADD;

			bm.alpha.src=src;
			bm.alpha.dst=dst;
			bm.alpha.func=HGL_BLEND_ADD;

			SetBlend(bm);
		}

		bool Material::ClearTexture(int n)
		{
			if(n<0||n>=mtcMax)
				return(false);

			if(!texture_list[n])return(false);

			texture_list[n]=nullptr;

			return(true);
		}

		void Material::ClearTexture()
		{
			for(int i=0;i<mtcMax;i++)
				texture_list[i]=nullptr;
		}

		bool Material::SetTexture(int n,Texture *tex)
		{
			if(n<0||n>=mtcMax)return(false);

			if(texture_list[n])
			{
				if(texture_list[n]==tex)
					return(true);

				LOG_ERROR(OS_TEXT("Material::SetTexture(")+OSString(n)+OS_TEXT(")已有贴图，不可再次设置"));
				return(false);
			}

			texture_list[n]=tex;

			return(true);
		}
	}//namespace graph
}//namespace hgl
