#include"apiTexture.h"
namespace hgl
{
	namespace graph
	{
		namespace api
		{
			namespace texture_dsa_storage
			{
				//void InitDSATextureEXTAPI()		//由于EXT版DSA函数和ARB版不同，所以无支持计划，此函数仅保留做为注释
				//{
				//	glBindTextureUnit				=glBindTextureUnitEXT;
				//	glCompressedTextureSubImage1D	=glCompressedTextureSubImage1DEXT;
				//	glCompressedTextureSubImage2D	=glCompressedTextureSubImage2DEXT;
				//	glCompressedTextureSubImage3D	=glCompressedTextureSubImage3DEXT;
				//	glCopyNamedBufferSubData		=glCopyNamedBufferSubDataEXT;
				//	glCopyTextureSubImage1D			=glCopyTextureSubImage1DEXT;
				//	glCopyTextureSubImage2D			=glCopyTextureSubImage2DEXT;
				//	glCopyTextureSubImage3D			=glCopyTextureSubImage3DEXT;
				//	glCreateTextures				=glCreateTexturesEXT;
				//	glGenerateTextureMipmap			=glGenerateTextureMipmapEXT;
				//	glGetCompressedTextureImage		=glGetCompressedTextureImageEXT;
				//	glGetTextureImage				=glGetTextureImageEXT;
				//	glGetTextureLevelParameterfv	=glGetTextureLevelParameterfvEXT;
				//	glGetTextureLevelParameteriv	=glGetTextureLevelParameterivEXT;
				//	glGetTextureParameterIiv		=glGetTextureParameterIivEXT;
				//	glGetTextureParameterIuiv		=glGetTextureParameterIuivEXT;
				//	glGetTextureParameterfv			=glGetTextureParameterfvEXT;
				//	glGetTextureParameteriv			=glGetTextureParameterivEXT;
				//	glNamedFramebufferTexture		=glNamedFramebufferTextureEXT;
				//	glNamedFramebufferTextureLayer	=glNamedFramebufferTextureLayerEXT;
				//	glTextureBuffer					=glTextureBufferEXT;
				//	glTextureBufferRange			=glTextureBufferRangeEXT;
				//	glTextureParameterIiv			=glTextureParameterIivEXT;
				//	glTextureParameterIuiv			=glTextureParameterIuivEXT;
				//	glTextureParameterf				=glTextureParameterfEXT;
				//	glTextureParameterfv			=glTextureParameterfvEXT;
				//	glTextureParameteri				=glTextureParameteriEXT;
				//	glTextureParameteriv			=glTextureParameterivEXT;
				//	glTextureStorage1D				=glTextureStorage1DEXT;
				//	glTextureStorage2D				=glTextureStorage2DEXT;
				//	glTextureStorage2DMultisample	=glTextureStorage2DMultisampleEXT;
				//	glTextureStorage3D				=glTextureStorage3DEXT;
				//	glTextureStorage3DMultisample	=glTextureStorage3DMultisampleEXT;
				//	glTextureSubImage1D				=glTextureSubImage1DEXT;
				//	glTextureSubImage2D				=glTextureSubImage2DEXT;
				//	glTextureSubImage3D				=glTextureSubImage3DEXT;
				//}

				uint CreateTexture(uint format)
				{
					uint id;

					glCreateTextures(format, 1, &id);

					return id;
				}

				uint CreateMultiTextures(uint *id_list, const uint format, const uint count)
				{
					glCreateTextures(format, count, id_list);

					return count;
				}

				void DeleteTexture(const uint id)
				{
					glDeleteTextures(1, &id);
				}

				void DeleteMultiTextures(const uint *id_list,const uint count)
				{
					glDeleteTextures(count,id_list);
				}
			}//namespace texture_dsa_storage

			void InitTextureDSAStorage()
			{
				//ext版设计和ARB版不同，而且有一定陈旧设计，所以不支持EXT版DSA
//				if (!GLEW_ARB_direct_state_access&&GLEW_EXT_direct_state_access)
//					hgl::graph::api::texture_dsa_storage::InitDSATextureEXTAPI();

				hgl::graph::api::CreateTexture		= hgl::graph::api::texture_dsa_storage::CreateTexture;
				hgl::graph::api::CreateMultiTextures= hgl::graph::api::texture_dsa_storage::CreateMultiTextures;
				hgl::graph::api::DeleteTexture		= hgl::graph::api::texture_dsa_storage::DeleteTexture;
				hgl::graph::api::DeleteMultiTextures= hgl::graph::api::texture_dsa_storage::DeleteMultiTextures;
			}
		}//namespace api
	}//namespace graph
}//namespace hgl
