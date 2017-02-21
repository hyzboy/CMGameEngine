#include<hgl/graph/VertexBuffer.h>
#include<glew/include/GL/glew.h>
#include"VertexBufferControl.h"

namespace hgl
{
	namespace graph
	{
		int HGL_MAX_VERTEX_ATTRIBS;		///<最大顶点属性数量

		void InitVertexBuffer()
		{
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&HGL_MAX_VERTEX_ATTRIBS);
		}
	}//namespace graph

    namespace graph
    {
        VertexBufferControl *CreateVertexBufferControlDSA();
        VertexBufferControl *CreateVertexBufferControlBind();

        VertexBufferControl *(*CreateVertexBufferControl)()=nullptr;

        void InitVertexBufferDSA()
        {
            CreateVertexBufferControl=CreateVertexBufferControlDSA;
        }

        void InitVertexBufferBind()
        {
            CreateVertexBufferControl = CreateVertexBufferControlBind;
        }

        bool InitVertexBufferAPI()
        {
            if (GLEW_ARB_direct_state_access)            //4.5
            {
                InitVertexBufferDSA();
                return(true);
            }

            InitVertexBufferBind();
            return(true);
        }
    }//namespace graph

    namespace graph
    {
        void VertexBufferBase::SetDataSize(int size)
        {
            if (bytes == size)return;

            bytes = size;

            if (mem_data)
                mem_data = hgl_realloc(mem_data, size);
            else
                mem_data = hgl_malloc(size);

            mem_end = ((char *)mem_data) + size;
        }

        VertexBufferBase::VertexBufferBase(uint level, uint size)
        {
            dc_num = 0;

            bytes = size;

            mem_data = hgl_malloc(size);			//在很多情况下，hgl_malloc分配的内存是对齐的，这样有效率上的提升
            mem_end = ((char *)mem_data) + size;

            data_level = level;

            vbc=nullptr;
        }

        VertexBufferBase::~VertexBufferBase()
        {
            CloseVertexBuffer();

            hgl_free(mem_data);

            SAFE_CLEAR(vbc);
        }
        void VertexBufferBase::CloseVertexBuffer()
        {
            if(vbc)
                vbc->Clear();
        }

        void VertexBufferBase::ChangeVertexBuffer(int start, int size, void *data)
        {
            if (!vbc)return;

            vbc->Change(start,size,data);
        }

        bool VertexBufferBase::CreateVertexBuffer(uint type)
        {
            SAFE_CLEAR(vbc);

            vbc=CreateVertexBufferControl();
            return vbc;
        }

        // 		void VertexBufferBase::BindVertexBuffer()
        // 		{
        // 			if(!video_buffer_type)return;
        //
        // 			glBindBuffer(video_buffer_type,video_buffer_index);
        // 		}

        int VertexBufferBase::GetBufferIndex()const
        {
            return vbc?vbc->GetIndex():-1;
        }
    }//namespace graph
}//namespace hgl
