#include<hgl/graph/VertexBuffer.h>
#include<glew/include/GL/glew.h>

namespace hgl
{
	namespace graph
	{
		int HGL_MAX_VERTEX_ATTRIBS;		///<最大顶点属性数量

		void InitVertexBuffer()
		{
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&HGL_MAX_VERTEX_ATTRIBS);
		}
	}

	namespace graph
	{
		void VertexBufferBase::CloseVertexBuffer()
		{
			if(video_buffer_type)
				glDeleteBuffers(1,&video_buffer_index);
		}

		bool VertexBufferBase::CreateVertexBuffer(uint type)
		{
			if(video_buffer_type)	//不为0，已经创建过一次了
			{
				LOG_ERROR(OS_TEXT("VertexBuffer::CreateVertexBuffer重复被调用"));
				return(false);
			}

			video_buffer_type=type;

// 			glGenBuffers(1,&video_buffer_index);
//
// 			glBindBuffer(video_buffer_type,video_buffer_index);
// 			glBufferData(video_buffer_type,bytes,mem_data,data_level);

            glCreateBuffers(1,&video_buffer_index);
            glNamedBufferData(video_buffer_index,bytes,mem_data,data_level);
            //glNamedBufferStorage(video_buffer_index,bytes,mem_data,0);

			return(true);
		}

		void VertexBufferBase::ChangeVertexBuffer(int start,int size,void *data)
		{
			if(!video_buffer_type)return;

// 			glBindBuffer(video_buffer_type,video_buffer_index);
// 			glBufferSubData(video_buffer_type,start,size,data);

            glNamedBufferSubData(video_buffer_index,start,size,data);
		}

		void VertexBufferBase::BindVertexBuffer()
		{
			if(!video_buffer_type)return;

			glBindBuffer(video_buffer_type,video_buffer_index);
		}
	}//namespace graph
}//namespace hgl
