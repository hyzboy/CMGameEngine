#include"apiTexture.h"

namespace hgl
{
	namespace graph
	{
		namespace api
		{
			namespace texture_dsa_storage
			{
				Texture *CreateTexture(uint format)
				{
				}
			}//namespace texture_dsa_storage

			void InitTextureDSAStorage()
			{
				hgl::graph::api::CreateTexture = hgl::graph::api::texture_dsa_storage::CreateTexture;
			}
		}//namespace api
	}//namespace graph
}//namespace hgl