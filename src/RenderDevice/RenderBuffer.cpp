#include<hgl/graph/RenderBuffer.h>
#include<hgl/Macro.h>

namespace hgl
{
    namespace graph
    {
        RenderBuffer *PrimaryRenderBuffer=nullptr;                                                    ///<缺省主渲染缓冲区

        void InitPrimaryRenderBuffer()
        {
            PrimaryRenderBuffer=new RenderBuffer(0);
        }

        void ClosePrimaryRenderBuffer()
        {
            SAFE_CLEAR(PrimaryRenderBuffer);
        }

        RenderBuffer::RenderBuffer()
        {
            buffer_id=0;
        }

        RenderBuffer::RenderBuffer(int id)
        {
            if(glIsRenderbuffer(id))
                buffer_id=id;
            else
                buffer_id=0;
        }

        RenderBuffer::~RenderBuffer()
        {
            Close();
        }

        bool RenderBuffer::Create()
        {
            if(buffer_id>0)return(false);

            glCreateRenderbuffers(1,&buffer_id);
            return(true);
        }

        bool RenderBuffer::Close()
        {
            if(buffer_id==0)return(false);

            glDeleteRenderbuffers(1,&buffer_id);
            buffer_id=0;
            return(true);
        }
    }//namespace graph
}//namespace hgl
