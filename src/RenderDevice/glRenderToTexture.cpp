#include<hgl/graph/RenderToTexture.h>
#include<hgl/graph/Texture2D.h>
//#include<hgl/graph/Texture2DArray.h>
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
			glCreateFramebuffers(1,&fbo);
		}

		RenderToTexture::~RenderToTexture()
		{
			glDeleteFramebuffers(1,&fbo);
		}

		bool RenderToTexture::Begin()
		{
			glGetIntegerv(GL_VIEWPORT,viewport);

			glBindFramebuffer(GL_FRAMEBUFFER,fbo);
			if(!CheckFrameBufferStatus(fbo,__FILE__,__LINE__))
				return(false);

			return Use();
		}

		void RenderToTexture::End()
		{
			glBindFramebuffer(GL_FRAMEBUFFER,0);
			glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
		}

		bool RenderToTexture::CheckFrameBufferStatus(uint fbo,const char *src_file,const int src_line)
		{
			GLenum status = glCheckNamedFramebufferStatus(fbo,GL_FRAMEBUFFER);

            if(status== GL_FRAMEBUFFER_COMPLETE)return(true);

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

			LOG_ERROR("FBO Error "+UTF8String(status)+" in ("+UTF8String(src_line)+UTF8String(") ")+UTF8String(src_file));
            return(false);
		}
	}//namespace graph

	namespace graph
	{
		RenderToTextureColor::RenderToTextureColor(uint width,uint height,const TextureSourceFormat &tsf,const Color4f &bc,const float id)
		{
			if(width<=0||height<=0||!TextureSourceFormatCheck(tsf))
			{
				tex_color=nullptr;
				return;
			}

			{
				tex_color=CreateTexture2D(width,height,tsf);
			}

			back_color=bc;
			init_depth=id;

			draw_buffers=GL_COLOR_ATTACHMENT0;

			glCreateRenderbuffers(1,&rb_depth);
			glNamedRenderbufferStorage(rb_depth,GL_DEPTH_COMPONENT32,width,height);

			glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex_color->GetID(), 0);
			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
			glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_depth);
			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
		}

		RenderToTextureColor::~RenderToTextureColor()
		{
			if(tex_color)
			{
				glDeleteRenderbuffers(1,&rb_depth);

				delete tex_color;
			}
		}

		bool RenderToTextureColor::Use()
		{
			if(!tex_color)return(false);

            glNamedFramebufferDrawBuffers(fbo,1,&draw_buffers);

			glViewport(0,0,tex_color->GetWidth(),tex_color->GetHeight());
            glClearNamedFramebufferfv(fbo,GL_COLOR,0,(float *)&back_color);
            glClearNamedFramebufferfv(fbo,GL_DEPTH,0,&init_depth);
			return(true);
		}
	}//namespace graph

	namespace graph
	{
		RenderToTextureDepth::RenderToTextureDepth(uint width,uint height,const TextureSourceFormat &tsf,const float id)
		{
			if(width<=0||height<=0||!TextureSourceFormatDepthCheck(tsf))
			{
				tex_depth=nullptr;
				return;
			}

			{
				tex_depth=CreateTexture2D(width,height,tsf);
			}

			init_depth=id;

			glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, tex_depth->GetID(), 0);
			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
		}

		RenderToTextureDepth::~RenderToTextureDepth()
		{
			SAFE_CLEAR(tex_depth);
		}

		bool RenderToTextureDepth::Use()
		{
			if(!tex_depth)return(false);

            glNamedFramebufferDrawBuffer(fbo,GL_NONE);
            glNamedFramebufferReadBuffer(fbo,GL_NONE);

			glViewport(0,0,tex_depth->GetWidth(),tex_depth->GetHeight());
            glClearNamedFramebufferfv(fbo,GL_DEPTH,0,&init_depth);
			return(true);
		}
	}//namespace graph

	namespace graph
	{
		RenderToTextureColorDepth::RenderToTextureColorDepth(uint width,uint height,const TextureSourceFormat &color_tsf,const TextureSourceFormat &depth_tsf,const Color4f &bc,const float id)
		{
			if(width<=0||height<=0||!TextureSourceFormatCheck(color_tsf)||!TextureSourceFormatCheck(depth_tsf))
			{
				tex_color=nullptr;
				tex_depth=nullptr;
				return;
			}

			{
				tex_color=CreateTexture2D(width,height,color_tsf);
			}

			{
				tex_depth=CreateTexture2D(width,height,depth_tsf);
			}

			back_color=bc;
			init_depth=id;

			draw_buffers=GL_COLOR_ATTACHMENT0;

			glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex_color->GetID(), 0);
			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
			glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, tex_depth->GetID(), 0);
			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
		}

		RenderToTextureColorDepth::~RenderToTextureColorDepth()
		{
			if(tex_color)
			{
				delete tex_depth;
				delete tex_color;
			}
		}

		bool RenderToTextureColorDepth::Use()
		{
			if(!tex_color)return(false);

            glNamedFramebufferDrawBuffers(fbo,1,&draw_buffers);

			glViewport(0,0,tex_color->GetWidth(),tex_color->GetHeight());
            glClearNamedFramebufferfv(fbo,GL_COLOR,0,(float *)&back_color);
            glClearNamedFramebufferfv(fbo,GL_DEPTH,0,&init_depth);
			return(true);
		}
	}//namespace graph

    namespace graph
	{
		RenderToTextureMultiChannel::RenderToTextureMultiChannel(uint w,uint h,const Color4f &bc,const float id)
		{
            attachments=nullptr;

			if(w<=0||h<=0)
				return;

            width=w;
            height=h;

            tex_depth=nullptr;
            init_depth=id;
            rb_depth=0;

            back_color=bc;
		}

        Texture2D *RenderToTextureMultiChannel::AddDepth(const TextureSourceFormat &depth_tsf)
        {
            if(width<=0||height<=0)
				return(nullptr);

            if(tex_depth)
                return(tex_depth);
            
			tex_depth=CreateTexture2D(width,height,depth_tsf);
        }

        Texture2D *RenderToTextureMultiChannel::AddColor(const TextureSourceFormat &color_tsf)
        {
            if(!tex_depth)
                return(nullptr);

            if(!TextureSourceFormatCheck(color_tsf))
                return(nullptr);

            Texture2D *tex=CreateTexture2D(width,height,color_tsf);

            if(!tex)
                return(nullptr);

            tex_list.Add(tex);

            return tex;
        }

		RenderToTextureMultiChannel::~RenderToTextureMultiChannel()
		{
            delete[] attachments;       //delete[] nullptr不是个错误

			if(tex_depth)
				delete tex_depth;
            else
				glDeleteRenderbuffers(1,&rb_depth);
		}

        bool RenderToTextureMultiChannel::BindComplete()
        {
            const uint tex_count=tex_list.GetCount();

            if(!tex_depth&&tex_count==0)
                return(false);

            if(tex_depth)
            {
			    glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, tex_depth->GetID(), 0);
			    CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
            }
            else
            {
			    glCreateRenderbuffers(1,&rb_depth);
			    glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb_depth);
			    glNamedRenderbufferStorage(rb_depth,GL_DEPTH_COMPONENT32,width,height);
            }

            if(tex_count<=0)return(true);

            attachments=new uint[tex_count];

            Texture2D **tex=tex_list.GetData();

            uint buffers=GL_COLOR_ATTACHMENT0;

            for(uint i=0;i<tex_count;i++)
            {
			    glNamedFramebufferTexture(fbo, buffers, (*tex)->GetID(), 0);
			    CheckFrameBufferStatus(fbo,__FILE__,__LINE__);

                attachments[i]=buffers;

                ++buffers;
                ++tex;
            }

            return(true);
        }

		bool RenderToTextureMultiChannel::Use()
		{
            const uint tex_count=tex_list.GetCount();

            if(!tex_depth&&tex_count==0)
                return(false);

            if(tex_count>0)
            {
                glNamedFramebufferDrawBuffers(fbo,tex_count,attachments);
            }

			glViewport(0,0,tex_depth->GetWidth(),tex_depth->GetHeight());
//			glClearBufferfv(GL_COLOR,0,(float *)&back_color);
//			glClearBufferfv(GL_DEPTH,0,&init_depth);
            glClearNamedFramebufferfv(fbo,GL_DEPTH,0,&init_depth);
			return(true);
		}
	}//namespace graph

	//namespace graph
	//{
	//	RenderToTextureLayer::RenderToTextureLayer(uint width,uint height,uint depth,const TextureSourceFormat &color_tsf,const TextureSourceFormat &depth_tsf,const Color3f &bc,const float id)
	//	{
	//		if(width<=0||height<=0||depth<=0||!TextureSourceFormatCheck(color_tsf)||!TextureSourceFormatDepthCheck(depth_tsf))
	//		{
	//			tex_color=nullptr;
	//			tex_depth=nullptr;
	//			draw_buffers=nullptr;
	//			return;
	//		}

	//		{
	//			tex_color=new Texture2DArray();
	//			tex_color->SetImage(width,height,power_to_2(depth),color_tsf);
	//		}

	//		{
	//			tex_depth=CreateTexture2D();

	//			tex_depth->SetImage(width,height,depth_tsf);
	//		}

	//		back_color=bc;
	//		init_depth=id;

	//		draw_buffers=new GLenum[depth-1];
	//		for(uint i=0;i<depth;i++)
	//		{
	//			draw_buffers[i]=GL_COLOR_ATTACHMENT0+i;

	//			glNamedFramebufferTextureLayer(fbo,draw_buffers[i],tex_color->GetID(),0,i);
	//			CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
	//		}

	//		glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, tex_depth->GetID(), 0);
	//		CheckFrameBufferStatus(fbo,__FILE__,__LINE__);
	//	}

	//	RenderToTextureLayer::~RenderToTextureLayer()
	//	{
	//		delete[] draw_buffers;		//delete[] nullptr不是BUG

	//		SAFE_CLEAR(tex_depth);
	//		SAFE_CLEAR(tex_color);
	//	}

	//	bool RenderToTextureLayer::Use()
	//	{
	//		if(!tex_color)return(false);

	//		glDrawBuffers(tex_color->GetLayer(),draw_buffers);

	//		glViewport(0,0,tex_color->GetWidth(),tex_color->GetHeight());
	//		glClearBufferfv(GL_COLOR,0,(float *)&back_color);
	//		glClearBufferfv(GL_DEPTH,0,&init_depth);
	//		return(true);
	//	}
	//}//namespace graph
}//namespace hgl
