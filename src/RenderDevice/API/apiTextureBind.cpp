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

#define BIND_TEXTURE_API(name)	hgl::graph::api::name=hgl::graph::api::texture_bind::name;

			void InitTextureBind()
			{
				BIND_TEXTURE_API(CreateTexture)
				BIND_TEXTURE_API(CreateMultiTextures)
				BIND_TEXTURE_API(DeleteTexture)
				BIND_TEXTURE_API(DeleteMultiTextures)
			}

			namespace texture_bind_storage
			{
			}//namespace texture_bind_storage
			
			void InitTextureBindStorage()
			{
				BIND_TEXTURE_API(CreateTexture)
				BIND_TEXTURE_API(CreateMultiTextures)
				BIND_TEXTURE_API(DeleteTexture)
				BIND_TEXTURE_API(DeleteMultiTextures)
			}

#undef DSA_STORAGE_TEXTURE_API
		}//namespace api
	}//namespace graph
}//namespace hgl