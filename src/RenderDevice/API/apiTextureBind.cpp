#include"apiTexture.h"

namespace hgl
{
	namespace graph
	{
		namespace api
		{
			namespace texture_bind
			{
				Texture *CreateTexture(uint format)
				{
				}
			}//namespace texture_bind

			void InitTextureBind()
			{
				hgl::graph::api::CreateTexture = hgl::graph::api::texture_bind::CreateTexture;
			}

			namespace texture_bind_storage
			{
			}//namespace texture_bind_storage
			
			void InitTextureBindStorage()
			{
				hgl::graph::api::CreateTexture = hgl::graph::api::texture_bind_storage::CreateTexture;
			}
		}//namespace api
	}//namespace graph
}//namespace hgl