#include"OpenGLTexture.h"
#include<glew/include/GL/glew.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		namespace
		{
			struct OpenGLCoreTextureFormat
			{
				TextureSourceFormat tsf;	//数据源格式枚举

				char name[16];				//简写名称

				bool compress;				//是否压缩格式
				uint index;					//索引色数量

				uint internalFormat;		//显存格式

				uint format;				//格式
				uint type;					//类型

				uint source_bytes;			//原始格式字节数
				uint video_bytes;			//显存格式字节数
			};

			//贴图数据源格式
			const OpenGLCoreTextureFormat tf_list[]=
			{
				{HGL_SF_NONE,			"",				false,	0,	0,												GL_RGBA,		GL_UNSIGNED_BYTE			,0,0},

				{HGL_SF_R8,				"R8",			false,	0,	GL_R8,											GL_RED,			GL_UNSIGNED_BYTE			,1,1},
				{HGL_SF_RG8,			"RG8",			false,	0,	GL_RG8,											GL_RG,			GL_UNSIGNED_BYTE			,2,2},
				{HGL_SF_RGB8,			"RGB8",			false,	0,	GL_RGB8,										GL_RGB,			GL_UNSIGNED_BYTE			,3,3},
				{HGL_SF_RGBA8,			"RGBA8",		false,	0,	GL_RGBA8,										GL_RGBA,		GL_UNSIGNED_BYTE			,4,4},

				{HGL_SF_SRGB8,			"SRGB8",		false,	0,	GL_SRGB8,										GL_SRGB,		GL_UNSIGNED_BYTE			,4,4},
				{HGL_SF_SRGBA8,			"SRGBA8",		false,	0,	GL_SRGB8_ALPHA8,								GL_SRGB_ALPHA,	GL_UNSIGNED_BYTE			,5,5},

				{HGL_SF_R16,			"R16",			false,	0,	GL_R16,											GL_RED,			GL_UNSIGNED_SHORT			,2,2},
				{HGL_SF_RG16,			"RG16",			false,	0,	GL_RG16,										GL_RG,			GL_UNSIGNED_SHORT			,4,4},
				{HGL_SF_RGB16,			"RGB16",		false,	0,	GL_RGB16,										GL_RGB,			GL_UNSIGNED_SHORT			,6,6},
				{HGL_SF_RGBA16,			"RGBA16",		false,	0,	GL_RGBA16,										GL_RGBA,		GL_UNSIGNED_SHORT			,8,8},

				{HGL_SF_R16F,			"R16F",			false,	0,	GL_R16F,										GL_RED,			GL_FLOAT					,2,2},
				{HGL_SF_RG16F,			"RG16F",		false,	0,	GL_RG16F,										GL_RG,			GL_FLOAT					,4,4},
				{HGL_SF_RGB16F,			"RGB16F",		false,	0,	GL_RGB16F,										GL_RGB,			GL_FLOAT					,6,6},
				{HGL_SF_RGBA16F,		"RGBA16F",		false,	0,	GL_RGBA16F,										GL_RGBA,		GL_FLOAT					,8,8},

				{HGL_SF_R32F,			"R32F",			false,	0,	GL_R32F,										GL_RED,			GL_FLOAT					,4,4},
				{HGL_SF_RG32F,			"RG32F",		false,	0,	GL_RG32F,										GL_RG,			GL_FLOAT					,8,8},
				{HGL_SF_RGB32F,			"RGB32F",		false,	0,	GL_RGB32F,										GL_RGB,			GL_FLOAT					,12,12},
				{HGL_SF_RGBA32F,		"RGBA32F",		false,	0,	GL_RGBA32F,										GL_RGBA,		GL_FLOAT					,16,16},

				{HGL_SF_UNCOMPRESSED,	"",				false,	0,	0,												0,				0							,0,0},

				{HGL_SF_R3_G3_B2,		"R3_G3_B2",		false,	0,	GL_R3_G3_B2,									GL_RGB,			GL_UNSIGNED_BYTE_3_3_2		,1,1},
				{HGL_SF_RGB5,			"RGB5",			false,	0,	GL_RGB5,										GL_RGB,			GL_UNSIGNED_SHORT_5_5_5_1	,2,2},

				{HGL_SF_RGBA4,			"RGBA4",		false,	0,	GL_RGBA4,										GL_RGBA,		GL_UNSIGNED_SHORT_4_4_4_4	,2,2},
				{HGL_SF_RGB5_A1,		"RGB5_A1",		false,	0,	GL_RGB5_A1,										GL_RGBA,		GL_UNSIGNED_SHORT_5_5_5_1	,2,2},
				{HGL_SF_RGB10_A2,		"RGB10_A2",		false,	0,	GL_RGB10_A2,									GL_RGBA,		GL_UNSIGNED_INT_10_10_10_2	,4,4},

				{HGL_SF_RG11F_B10F,		"RG11F_B10F",	false,	0,	GL_R11F_G11F_B10F,								GL_RGB,			GL_UNSIGNED_INT_10F_11F_11F_REV	,4,4},
				{HGL_SF_RGB9_E5,		"RGB9_E5",		false,	0,	GL_RGB9_E5,										GL_RGB,			GL_UNSIGNED_INT_5_9_9_9_REV	,4,4},

				{HGL_SF_INDEX,			"",				false,	0,	0,												0,				0							,0,0},

				{HGL_SF_INDEX_16_RGB,	"16RGB",		false,	16,	GL_LUMINANCE4,									GL_RGB,			GL_UNSIGNED_BYTE			,0,0},
				{HGL_SF_INDEX_16_RGBA,	"16RGBA",		false,	16,	GL_LUMINANCE4,									GL_RGBA,		GL_UNSIGNED_BYTE			,0,0},
				{HGL_SF_INDEX_256_RGB,	"256RGB",		false,	256,GL_LUMINANCE8,									GL_RGB,			GL_UNSIGNED_BYTE			,0,0},
				{HGL_SF_INDEX_256_RGBA,	"256RGBA",		false,	256,GL_LUMINANCE8,									GL_RGBA,		GL_UNSIGNED_BYTE			,0,0},

				{HGL_SF_COMPRESSED,		"",				false,	0,	0,												0,				0							,0,0},
	/*
				{HGL_SF_CR,				"CR",			true,	0,	GL_COMPRESSED_RED,								GL_RED,			0,0,0},				//注：GL_COMPRESSED RED/RG/RGB/RGBA/SRGB/SRGBA这6个格式
				{HGL_SF_CRG,			"CRG",			true,	0,	GL_COMPRESSED_RG,								GL_RG,			0,0,0},				//只能用于glTexImage，由于显卡并不一定确定最终用什么算法，
				{HGL_SF_CRGB,			"CRGB",			true,	0,	GL_COMPRESSED_RGB,								GL_RGB,			0,0,0},				//所以用glGetCompressTexImage取出的数据，不能在glCompressTexImage中用
				{HGL_SF_CRGBA,			"CRGBA",		true,	0,	GL_COMPRESSED_RGBA,								GL_RGBA,		0,0,0},

				{HGL_SF_CSRGB,			"CSRGB",		true,	0,	GL_COMPRESSED_SRGB,								GL_SRGB,		0,0,0},
				{HGL_SF_CSRGBA,			"CSRGBA",		true,	0,	GL_COMPRESSED_SRGB_ALPHA,						GL_SRGB_ALPHA,	0,0,0},
	*/
				{HGL_SF_DXT1RGB,		"DXT1RGB",		true,	0,	GL_COMPRESSED_RGB_S3TC_DXT1_EXT,				GL_RGB,			0,0,0},
				{HGL_SF_DXT1RGBA,		"DXT1RGBA",		true,	0,	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,				GL_RGBA,		0,0,0},
				{HGL_SF_DXT3RGBA,		"DXT3RGBA",		true,	0,	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,				GL_RGBA,		0,0,0},
				{HGL_SF_DXT5RGBA,		"DXT5RGBA",		true,	0,	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,				GL_RGBA,		0,0,0},

				{HGL_SF_LATC1,			"LATC1",		true,	0,	GL_COMPRESSED_LUMINANCE_LATC1_EXT,				GL_RED,			0,0,0},
				{HGL_SF_LATC1s,			"LATC1s",		true,	0,	GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT,		GL_RED,			0,0,0},
				{HGL_SF_LATC2,			"LATC2",		true,	0,	GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,		GL_RG,			0,0,0},
				{HGL_SF_LATC2s,			"LATC2s",		true,	0,	GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT,	GL_RG,			0,0,0},

				{HGL_SF_RGTC1,			"RGTC1",		true,	0,	GL_COMPRESSED_RED_RGTC1,						GL_RED,			0,0,0},
				{HGL_SF_RGTC1s,			"RGTC1s",		true,	0,	GL_COMPRESSED_SIGNED_RED_RGTC1,					GL_RED,			0,0,0},
				{HGL_SF_RGTC2,			"RGTC2",		true,	0,	GL_COMPRESSED_RG_RGTC2,							GL_RG,			0,0,0},
				{HGL_SF_RGTC2s,			"RGTC2s",		true,	0,	GL_COMPRESSED_SIGNED_RG_RGTC2,					GL_RG,			0,0,0},

				{HGL_SF_BPTC_RGBf,		"BPTC_RGBf",	true,	0,	GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,		GL_RGB,			0,0,0},
				{HGL_SF_BPTC_RGBuf,		"BPTC_RGBuf",	true,	0,	GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,		GL_RGB,			0,0,0},
				{HGL_SF_BPTC_RGBA,		"BPTC_RGBA",	true,	0,	GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,				GL_RGBA,		0,0,0},
				{HGL_SF_BPTC_SRGBA,		"BPTC_SRGBA",	true,	0,	GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB,		GL_SRGB_ALPHA,	0,0,0},
			};//const OpenGLCoreTextureFormat tf_list[]

			const int tf_count=sizeof(tf_list)/sizeof(OpenGLCoreTextureFormat);
		}//namespace

		TSF GetColorFormat(const char *str)
		{
			char fmt[17];

			memcpy(fmt,str,16);
			fmt[16]=0;

			for(int i=0;i<tf_count;i++)
				if(::strcmp(fmt,tf_list[i].name)==0)
					return(tf_list[i].tsf);

			return(HGL_SF_NONE);
		}

		bool BindTexture(int active,unsigned int type,unsigned int index);	//绑定贴图

		glTexture2D::glTexture2D()
		{
#ifdef HGL_USE_OPENGL_CORE_45
			glCreateTextures(GL_TEXTURE_2D,1,&index);
#else
			glGenTextures(1,&index);
#endif//HGL_USE_OPENGL_CORE_45
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

			const OpenGLCoreTextureFormat *sfmt=tf_list+sf;		//原始数据格式

			if(vf==0)
				vf=tf_list[sf].internalFormat;

			const bool gen_mip=ltp&ltGenMipmaps;			//取得是否创建mipmaps

			glGetError();			//清除错误

			#ifndef HGL_USE_OPENGL_CORE_45
				BindTexture(0,GL_TEXTURE_2D,index);				//4.5之后提交纹理不用bind
			#endif//HGL_USE_OPENGL_CORE_45

			if(sfmt->compress)		//原本就是压缩格式
			{
			#ifdef HGL_USE_OPENGL_CORE_45
				glCompressedTextureSubImage2D(index,0,0,0,w,h,vf,image_size,data);
			#else
				glCompressedTexImage2D(GL_TEXTURE_2D,0,vf,w,h,0,image_size,data);
			#endif//HGL_USE_OPENGL_CORE_45
			}
			else					//正常非压缩格式
			{
			#ifdef HGL_USE_OPENGL_CORE_45
				glTextureStorage2D(index,1,vf,w,h);
				glTextureSubImage2D(index,0,0,0,w,h,sfmt->format,sfmt->type,data);
			#else
				glTexImage2D(GL_TEXTURE_2D,0,vf,w,h,0,sfmt->format,sfmt->type,data);
			#endif//HGL_USE_OPENGL_CORE_45
			}

			const GLenum gl_error=glGetError();
			if(gl_error!=GL_NO_ERROR)
			{
				LOG_ERROR(OS_TEXT("创建贴图出错,GL_ERROR=")+OSString(gl_error));
				return(false);
			}

			if(gen_mip)
			{
				#ifdef HGL_USE_OPENGL_CORE_45
					glGenerateTextureMipmap(index);
				#else
					glGenerateMipmap(GL_TEXTURE_2D);
				#endif//HGL_USE_OPENGL_CORE_45

//					glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);		//设置LOD偏向,负是更精细，正是更模糊
			}

			{
				min_filter=gen_mip?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR;
				mag_filter=GL_LINEAR;

				wrap_s=GL_REPEAT;
				wrap_t=GL_REPEAT;

				//未来使用Sampler Object，则不再需要以下部分
	#ifdef HGL_USE_OPENGL_CORE_45
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
	#endif//HGL_USE_OPENGL_CORE_45
			}

			return(true);
		}

		void glTexture2D::GetMipmapLevel(int &base_level,int &max_level)
		{
			#ifdef HGL_USE_OPENGL_CORE_45
				glGetTextureParameteriv(index,GL_TEXTURE_BASE_LEVEL,&base_level);
				glGetTextureParameteriv(index,GL_TEXTURE_MAX_LEVEL,&max_level);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,&base_level);
				glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,&max_level);
			#endif//HGL_USE_OPENGL_CORE_45
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

			const OpenGLCoreTextureFormat *tsf=tf_list+fmt;

			#ifdef HGL_USE_OPENGL_CORE_45
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
			#endif//HGL_USE_OPENGL_CORE_45

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

			const OpenGLCoreTextureFormat *sfmt=tf_list+sf;		//原始数据格式

			#ifdef HGL_USE_OPENGL_CORE_45
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
			#endif//HGL_USE_OPENGL_CORE_45

			return(true);
		}

		void glTexture2D::SetMinFilter(uint mf)
		{
			if(min_filter==mf)return;

			min_filter=mf;

			#ifdef HGL_USE_OPENGL_CORE_45
				glTextureParameteri(index,GL_TEXTURE_MIN_FILTER,min_filter);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min_filter);
			#endif//HGL_USE_OPENGL_CORE_45
		}

		void glTexture2D::SetMagFilter(uint mf)
		{
			if(mag_filter==mf)return;

			mag_filter=mf;

			#ifdef HGL_USE_OPENGL_CORE_45
				glTextureParameteri(index,GL_TEXTURE_MAG_FILTER,mag_filter);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag_filter);
			#endif//HGL_USE_OPENGL_CORE_45
		}

		void glTexture2D::SetWrapS(uint wrap)
		{
			if(wrap_s==wrap)return;

			wrap_s=wrap;

			#ifdef HGL_USE_OPENGL_CORE_45
				glTextureParameteri(index,GL_TEXTURE_WRAP_S,wrap_s);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap_s);
			#endif//HGL_USE_OPENGL_CORE_45
		}

		void glTexture2D::SetWrapT(uint wrap)
		{
			if(wrap_t==wrap)return;

			wrap_t=wrap;

			#ifdef HGL_USE_OPENGL_CORE_45
				glTextureParameteri(index,GL_TEXTURE_WRAP_T,wrap_t);
			#else
				BindTexture(0,GL_TEXTURE_2D,index);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap_t);
			#endif//HGL_USE_OPENGL_CORE_45
		}

		Texture2D *CreateTexture2D()
		{
			return(new glTexture2D);
		}
	}//namespace graph
}//namespace hgl
