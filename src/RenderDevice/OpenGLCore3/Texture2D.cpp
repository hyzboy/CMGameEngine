#include"OpenGLTexture.h"
#include<glew/include/GL/glew.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		bool BindTexture(int active,unsigned int type,unsigned int index);	//绑定贴图

		glTexture2D::glTexture2D()
		{
#ifdef HGL_OPENGL_USE_DSA
			glCreateTextures(GL_TEXTURE_2D,1,&index);
#else
			glGenTextures(1,&index);
#endif//HGL_OPENGL_USE_DSA
		}

		glTexture2D::~glTexture2D()
		{
			glDeleteTextures(1,&index);
		}

		bool glTexture2D::SetImage(unsigned int w,unsigned int h,void *data,unsigned int image_size,TSF sf,unsigned int vf,unsigned int ltp)
		{
			if(!w||!h)
			{
				LOG_ERROR(OS_TEXT("glTexture2D::SetImage,size error,width=")+OSString(w)+OS_TEXT(",height=")+OSString(h));
				return(false);
			}

			if(data)		//无DATA数据时不用关心源格式
			{
				if(sf<=HGL_SF_NONE
				 ||sf>=HGL_SF_END
				 ||sf==HGL_SF_UNCOMPRESSED
				 ||sf==HGL_SF_INDEX
				 ||sf==HGL_SF_COMPRESSED)
				{
					LOG_ERROR(OS_TEXT("glTexture2D::SetImage,sf error =")+OSString(sf));
					return(false);
				}
			}

			type=HGL_TEXTURE_2D;
			width=w;
			height=h;

			const TextureFormat *sfmt=TextureFormatInfoList+sf;		//原始数据格式

			if(vf==0)
				vf=TextureFormatInfoList[sf].internalFormat;

			const bool gen_mip=ltp&ltGenMipmaps;			//取得是否创建mipmaps

			#ifndef HGL_OPENGL_USE_DSA
				BindTexture(0,GL_TEXTURE_2D,index);				//4.5之后提交纹理不用bind
			#endif//HGL_OPENGL_USE_DSA

			if(sfmt->compress)		//原本就是压缩格式
			{
			#ifdef HGL_OPENGL_USE_DSA
				glCompressedTextureSubImage2D(index,0,0,0,w,h,vf,image_size,data);
			#else
				glCompressedTexImage2D(GL_TEXTURE_2D,0,vf,w,h,0,image_size,data);
			#endif//HGL_OPENGL_USE_DSA
			}
			else					//正常非压缩格式
			{
#ifdef HGL_OPENGL_USE_DSA
				glTextureStorage2D(index, 1, vf, w, h);
				glTextureSubImage2D(index, 0, 0, 0, w, h, sfmt->format, sfmt->type, data);
#else
/*				if (use_pbo)
				{
					glGenBuffers(1, &pbo);
					glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
//					glBufferData(GL_PIXEL_UNPACK_BUFFER, w*h*sfmt->source_bytes, 0, GL_STREAM_DRAW);

//					void *ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
//					memcpy(ptr, w*h*sfmt->source_bytes, data);
//					glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
					glBufferDATA(GL_PIXEL_UNPACK_BUFFER, w*h*sfmt->source_bytes, data, GL_STREAM_DRAW);		//此句与上面的对应，是不是可以这么用，有待测试

					glTexImage2D(GL_TEXTURE_2D, 0, vf, w, h, 0, sfmt->format, sfmt->type, nullptr);
				}
				else*/
				{
					glTexImage2D(GL_TEXTURE_2D, 0, vf, w, h, 0, sfmt->format, sfmt->type, data);
				}
			#endif//HGL_OPENGL_USE_DSA
			}

			if(gen_mip)
			{
				#ifdef HGL_OPENGL_USE_DSA
					glGenerateTextureMipmap(index);
				#else
					glGenerateMipmap(GL_TEXTURE_2D);
				#endif//HGL_OPENGL_USE_DSA

//					glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);		//设置LOD偏向,负是更精细，正是更模糊
			}

			{
				min_filter=gen_mip?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR;
				mag_filter=GL_LINEAR;

				wrap_s=GL_REPEAT;
				wrap_t=GL_REPEAT;

				//未来使用Sampler Object，则不再需要以下部分
	#ifdef HGL_OPENGL_USE_DSA
				glTextureParameteri(index,GL_TEXTURE_MIN_FILTER,min_filter);
				glTextureParameteri(index,GL_TEXTURE_MAG_FILTER,mag_filter);
				//glTextureParameteri(index,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				//glTextureParameteri(index,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				glTextureParameteri(index,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTextureParameteri(index,GL_TEXTURE_WRAP_T,GL_REPEAT);
	#else
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min_filter);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag_filter);
				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	#endif//HGL_OPENGL_USE_DSA
			}

			return(true);
		}

		void glTexture2D::GetMipmapLevel(int &base_level,int &max_level)
		{
			#ifdef HGL_OPENGL_USE_DSA
				glGetTextureParameteriv(index,GL_TEXTURE_BASE_LEVEL,&base_level);
				glGetTextureParameteriv(index,GL_TEXTURE_MAX_LEVEL,&max_level);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,&base_level);
				glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,&max_level);
			#endif//HGL_OPENGL_USE_DSA
		}

		int glTexture2D::GetImage(void *data_pointer,TSF fmt,int level)
		{
			if(fmt<=HGL_SF_NONE
			 ||fmt>=HGL_SF_END
			 ||fmt==HGL_SF_UNCOMPRESSED
			 ||fmt==HGL_SF_INDEX
			 ||fmt==HGL_SF_COMPRESSED)
			{
				LOG_ERROR(OS_TEXT("glTexture2D::GetImage,fmt error =")+OSString(fmt));
				return(-1);
			}

			int compress;
			int bytes;

			const TextureFormat *tsf=TextureFormatInfoList+fmt;

			#ifdef HGL_OPENGL_USE_DSA
				glGetTextureLevelParameteriv(index,level,GL_TEXTURE_COMPRESSED,&compress);

				if(compress)
				{
					glGetTextureLevelParameteriv(index,level,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&bytes);

					if(data_pointer)
						glGetCompressedTextureImage(index,level,bytes,data_pointer);
				}
				else
				{
					if(tsf->video_bytes==0)return(-1);

					bytes=width*height*tsf->video_bytes;

					if(data_pointer)
						glGetTextureImage(index,level,tsf->format,tsf->type,bytes,data_pointer);
				}
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glGetTexLevelParameteriv(GL_TEXTURE_2D,level,GL_TEXTURE_COMPRESSED,&compress);

				if(compress)
				{
					glGetTexLevelParameteriv(GL_TEXTURE_2D,level,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&bytes);

					if(data_pointer)
						glGetCompressedTexImage(GL_TEXTURE_2D,level,data_pointer);
				}
				else
				{
					if(tsf->video_bytes==0)return(-1);

					bytes=width*height*tsf->video_bytes;

					if(data_pointer)
						glGetTexImage(GL_TEXTURE_2D,level,tsf->format,tsf->type,data_pointer);
				}
			#endif//HGL_OPENGL_USE_DSA

			return(bytes);
		}

		bool glTexture2D::ChangeImage(uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf)
		{
			if(!w||!h||!data
			 ||sf<=HGL_SF_NONE
			 ||sf>=HGL_SF_END
			 ||sf==HGL_SF_UNCOMPRESSED
			 ||sf==HGL_SF_INDEX
			 ||sf==HGL_SF_COMPRESSED)
			{
				LOG_ERROR(OS_TEXT("glTexture2D::ChangeImage,fmt error =")+OSString(sf));
				return(false);
			}

			const TextureFormat *sfmt=TextureFormatInfoList+sf;		//原始数据格式

			#ifdef HGL_OPENGL_USE_DSA
				if(sfmt->compress)
					glCompressedTextureSubImage2D(index,0,l,t,w,h,sfmt->internalFormat,bytes,data);
				else
					glTextureSubImage2D(index,0,l,t,w,h,sfmt->format,sfmt->type,data);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				if(sfmt->compress)
					glCompressedTexSubImage2D(GL_TEXTURE_2D,0,l,t,w,h,sfmt->internalFormat,bytes,data);
				else
					glTexSubImage2D(GL_TEXTURE_2D,0,l,t,w,h,sfmt->format,sfmt->type,data);
			#endif//HGL_OPENGL_USE_DSA

			return(true);
		}

		void glTexture2D::SetMinFilter(uint mf)
		{
			if(min_filter==mf)return;

			min_filter=mf;

			#ifdef HGL_OPENGL_USE_DSA
				glTextureParameteri(index,GL_TEXTURE_MIN_FILTER,min_filter);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min_filter);
			#endif//HGL_OPENGL_USE_DSA
		}

		void glTexture2D::SetMagFilter(uint mf)
		{
			if(mag_filter==mf)return;

			mag_filter=mf;

			#ifdef HGL_OPENGL_USE_DSA
				glTextureParameteri(index,GL_TEXTURE_MAG_FILTER,mag_filter);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag_filter);
			#endif//HGL_OPENGL_USE_DSA
		}

		void glTexture2D::SetWrapS(uint wrap)
		{
			if(wrap_s==wrap)return;

			wrap_s=wrap;

			#ifdef HGL_OPENGL_USE_DSA
				glTextureParameteri(index,GL_TEXTURE_WRAP_S,wrap_s);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap_s);
			#endif//HGL_OPENGL_USE_DSA
		}

		void glTexture2D::SetWrapT(uint wrap)
		{
			if(wrap_t==wrap)return;

			wrap_t=wrap;

			#ifdef HGL_OPENGL_USE_DSA
				glTextureParameteri(index,GL_TEXTURE_WRAP_T,wrap_t);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap_t);
			#endif//HGL_OPENGL_USE_DSA
		}

		Texture2D *new Texture2D
		{
			return(new glTexture2D);
		}
	}//namespace graph
}//namespace hgl
