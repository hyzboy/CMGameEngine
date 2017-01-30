#include"apiTexture.h"

namespace hgl
{
	namespace graph
	{
		namespace api
		{
			namespace texture_bind
			{
				uint CreateTexture(uint format)
				{
					uint id;

					glGenTextures(1, &id);

					return id;
				}

				uint CreateMultiTextures(uint *id_list, const uint format, const uint count)
				{
					glGenTextures(count, id_list);

					return count;
				}

				void DeleteTexture(const uint id)
				{
					glDeleteTextures(1, &id);
				}

				void DeleteMultiTextures(const uint *id_list, const uint count)
				{
					glDeleteTextures(count, id_list);
				}
			}//namespace texture_bind

			void InitTextureBind()
			{
				hgl::graph::api::CreateTexture		= hgl::graph::api::texture_bind::CreateTexture;
				hgl::graph::api::CreateMultiTextures= hgl::graph::api::texture_bind::CreateMultiTextures;
				hgl::graph::api::DeleteTexture		= hgl::graph::api::texture_bind::DeleteTexture;
				hgl::graph::api::DeleteMultiTextures= hgl::graph::api::texture_bind::DeleteMultiTextures;
			}

			namespace texture_bind_storage
			{
			}//namespace texture_bind_storage
			
			void InitTextureBindStorage()
			{
				hgl::graph::api::CreateTexture		= hgl::graph::api::texture_bind::CreateTexture;
				hgl::graph::api::CreateMultiTextures= hgl::graph::api::texture_bind::CreateMultiTextures;
				hgl::graph::api::DeleteTexture		= hgl::graph::api::texture_bind::DeleteTexture;
				hgl::graph::api::DeleteMultiTextures= hgl::graph::api::texture_bind::DeleteMultiTextures;
			}
		}//namespace api
	}//namespace graph
}//namespace hgl