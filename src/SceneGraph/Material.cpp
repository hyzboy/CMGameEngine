#include<hgl/graph/Material.h>
#include<hgl/type/BaseString.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace graph
	{
		Material *default_material=nullptr;

		void InitDefaultMaterial()
		{
			default_material=new Material;
		}

		void ClearDefaultMaterial()
		{
			SAFE_CLEAR(default_material);
		}

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

			color_material=false;

			Light=false;

			Front.Emission.Set(0.0f,0.0f,0.0f,1.0f);
			Front.Ambient.Set(0.25f,0.25f,0.25f,1.0f);
			Front.Diffuse.Set(1.0f,1.0f,1.0f,1.0f);
			Front.Specular.Set(0.95f,0.95f,0.95f,1.0f);
			Front.Shininess=40.0f;

			Back=Front;

			Transparency=1.0f;
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
	}//namespace graph
}//namespace hgl
