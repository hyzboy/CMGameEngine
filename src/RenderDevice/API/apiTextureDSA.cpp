#include"apiTexture.h"
#include<hgl/graph/TextureData.h>
#include<hgl/LogInfo.h>
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

                bool SetTexImage1D(uint id,Texture1DData *tex)
                {
                    if (tex->source_format->compress)      //原本就是压缩格式
                    {
                        if(tex->bitmap)
                            glCompressedTextureSubImage1D(id, 0, 0, tex->length, tex->video_format, tex->bitmap_bytes, tex->bitmap);
                    }
                    else                    //正常非压缩格式
                    {
                        glTextureStorage1D(id, 1, tex->video_format, tex->length);

                        if(tex->bitmap)
                            glTextureSubImage1D(id, 0, 0, tex->length, tex->source_format->color_format, tex->source_format->data_type, tex->bitmap);
                    }

                    if (tex->gen_mipmaps)
                    {
                        glGenerateTextureMipmap(id);

                        //                  glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);     //设置LOD偏向,负是更精细，正是更模糊
                    }

                    return(true);
                }

                bool SetTexImage2D(uint id,Texture2DData *tex)
                {
                    if (tex->source_format->compress)      //原本就是压缩格式
                    {
                        if (tex->bitmap)
                            glCompressedTextureSubImage2D(id, 0, 0, 0, tex->width, tex->height, tex->video_format, tex->bitmap_bytes, tex->bitmap);
                    }
                    else                    //正常非压缩格式
                    {
                        glTextureStorage2D(id, 1, tex->video_format, tex->width, tex->height);

                        if (tex->bitmap)
                            glTextureSubImage2D(id, 0, 0, 0, tex->width, tex->height, tex->source_format->color_format, tex->source_format->data_type, tex->bitmap);
                    }

                    if (tex->gen_mipmaps)
                    {
                        glGenerateTextureMipmap(id);

                        //                  glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);     //设置LOD偏向,负是更精细，正是更模糊
                    }

                    return(true);
                }

                int GetTexImage1D(uint texture_id,void *data_pointer, TSF fmt, int level,int length)
                {
                    if (!TextureSourceFormatCheck(fmt))
                    {
                        LOG_ERROR(OS_TEXT("GetTexImage1D,fmt error =") + OSString(fmt));
                        return(-1);
                    }

                    int compress;
                    int bytes;

                    const TextureFormat *tsf = TextureFormatInfoList + fmt;

                    glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED, &compress);

                    if (compress)
                    {
                        glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);

                        if (data_pointer)
                            glGetCompressedTextureImage(texture_id, level, bytes, data_pointer);
                    }
                    else
                    {
                        if (tsf->video_bytes == 0)return(-1);

                        bytes = length*tsf->video_bytes;

                        if (data_pointer)
                            glGetTextureImage(texture_id, level, tsf->color_format, tsf->data_type, bytes, data_pointer);
                    }

                    return(bytes);
                }


                int GetTexImage2D(uint texture_id,void *data_pointer, TSF fmt, int level,int width,int height)
                {
                    if (!TextureSourceFormatCheck(fmt))
                    {
                        LOG_ERROR(OS_TEXT("GetTexImage2D,fmt error =") + OSString(fmt));
                        return(-1);
                    }

                    int compress;
                    int bytes;

                    const TextureFormat *tsf = TextureFormatInfoList + fmt;

                    glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED, &compress);

                    if (compress)
                    {
                        glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);

                        if (data_pointer)
                            glGetCompressedTextureImage(texture_id, level, bytes, data_pointer);
                    }
                    else
                    {
                        if (tsf->video_bytes == 0)return(-1);

                        bytes = width*height*tsf->video_bytes;

                        if (data_pointer)
                            glGetTextureImage(texture_id, level, tsf->color_format, tsf->data_type, bytes, data_pointer);
                    }

                    return(bytes);
                }

                bool ChangeTexImage1D(uint texture_id,uint s, uint l, void *data, uint bytes, TSF sf)
                {
                    const TextureFormat *sfmt = TextureFormatInfoList + sf;       //原始数据格式

                    if (sfmt->compress)
                        glCompressedTextureSubImage1D(texture_id, 0, s, l, sfmt->video_format, bytes, data);
                    else
                        glTextureSubImage1D(texture_id, 0, s, l, sfmt->color_format, sfmt->data_type, data);

                    return(true);
                }


                bool ChangeTexImage2D(uint texture_id,uint l, uint t, uint w, uint h, void *data, uint bytes, TSF sf)
                {
                    const TextureFormat *sfmt = TextureFormatInfoList + sf;       //原始数据格式

                    if (sfmt->compress)
                        glCompressedTextureSubImage2D(texture_id, 0, l, t, w, h, sfmt->video_format, bytes, data);
                    else
                        glTextureSubImage2D(texture_id, 0, l, t, w, h, sfmt->color_format, sfmt->data_type, data);

                    return(true);
                }

//                 bool SetTexImage3D(Texture3DData *);
//                 bool SetTexImage1DArray(Texture1DArrayData *);
//                 bool SetTexImage2DArray(Texture2DArrayData *);
			}//namespace texture_dsa_storage

			void InitTextureDSAStorage()
			{
				//ext版设计和ARB版不同，而且有一定陈旧设计，所以不支持EXT版DSA
//				if (!GLEW_ARB_direct_state_access&&GLEW_EXT_direct_state_access)
//					hgl::graph::api::texture_dsa_storage::InitDSATextureEXTAPI();

#define DSA_STORAGE_TEXTURE_API(name)	hgl::graph::api::name=hgl::graph::api::texture_dsa_storage::name;

				DSA_STORAGE_TEXTURE_API(CreateTexture)
				DSA_STORAGE_TEXTURE_API(CreateMultiTextures)
				DSA_STORAGE_TEXTURE_API(DeleteTexture)
				DSA_STORAGE_TEXTURE_API(DeleteMultiTextures)

                DSA_STORAGE_TEXTURE_API(SetTexImage1D)

#undef DSA_STORAGE_TEXTURE_API
			}
		}//namespace api
	}//namespace graph
}//namespace hgl
