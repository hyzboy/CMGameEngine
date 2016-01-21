#include<hgl/graph/RenderToTexture.h>
#include<hgl/graph/Texture2D.h>
#include<hgl/LogInfo.h>

#ifndef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT 0x8CD8		//ATI SDK中有
#endif

#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9					//EXT版有，ARB版没有
#endif

#ifndef GL_FRAMEBUFFER_INCOMPLETE_FORMATS
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS 0x8CDA                    //EXT版有，ARB版没有
#endif

namespace hgl
{
	namespace graph
	{
		RenderToTexture::RenderToTexture()
		{
		}

		RenderToTexture::~RenderToTexture()
		{
		}

		bool RenderToTexture::CheckFrameBufferStatus(uint fbo)
		{
			GLenum status = glCheckNamedFramebufferStatus(fbo,GL_FRAMEBUFFER);

			switch (status)
			{
				case GL_FRAMEBUFFER_COMPLETE:						return true;
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"			);break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"	);break;
				case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT"	);break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:			LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"			);break;
				case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:				LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_FORMATS"				);break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"			);break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"			);break;
				case GL_FRAMEBUFFER_UNSUPPORTED:					LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED"						);break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"			);break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"			);break;
				default:											LOG_ERROR("Unknown FBO error"								);break;
			}

			return(false);
		}
	}//namespace graph

	namespace graph
	{
		RenderToTextureColor::RenderToTextureColor(uint width,uint height,const TextureSourceFormat &tsf,const Color3f &bc,const float id)
		{
			if(!TextureSourceFormatCheck(tsf))
			{
				tex_color=nullptr;
				return;
			}

			{
				tex_color=new Texture2D();

				tex_color->SetImage(width,height,nullptr,0,tsf,0,0);
			}

			back_color=bc;
			init_depth=id;

			draw_buffers=GL_COLOR_ATTACHMENT0;

			glCreateFramebuffers(1,&fbo);
			glCreateRenderbuffers(1,&rb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb_depth);
			glNamedRenderbufferStorage(rb_depth,GL_DEPTH_COMPONENT24,width,height);

			glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex_color->GetID(), 0);
			CheckFrameBufferStatus(fbo);
			glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
			CheckFrameBufferStatus(fbo);
		}

		RenderToTextureColor::~RenderToTextureColor()
		{
			if(tex_color)
			{
				glDeleteRenderbuffers(1,&rb_depth);
				glDeleteFramebuffers(1,&fbo);

				delete tex_color;
			}
		}

		bool RenderToTextureColor::Begin()
		{
			if(!tex_color)return(false);

			glGetIntegerv(GL_VIEWPORT,this->viewport);

			glBindFramebuffer(GL_FRAMEBUFFER,fbo);
			CheckFrameBufferStatus(fbo);
			glDrawBuffers(1,&draw_buffers);
			CheckFrameBufferStatus(fbo);

			glViewport(0,0,tex_color->GetWidth(),tex_color->GetHeight());
			glClearBufferfv(GL_COLOR,0,(float *)&back_color);
			glClearBufferfv(GL_DEPTH,0,&init_depth);
		}

		void RenderToTextureColor::End()
		{
			if(!tex_color)return;

			glBindFramebuffer(GL_FRAMEBUFFER,0);
			glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
		}
	}//namespace graph
}//namespace hgl
