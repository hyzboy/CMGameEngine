#include"UBO.h"
#include<GL/glew.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/VertexBuffer.h>

namespace hgl
{
	namespace graph
	{
		/**
		* 创建一个Shader数据块
		* @param name 数据块在shader中的名称
		* @param size 数据块长度
		* @param data 数据内容
		* @param level 更新级别
		* @return 创建好的数据块
		*/
		ShaderDataBlock *CreateShaderDataBlock(const char *name,int size,void *data,int level)
		{
			ShaderDataBlock *ubo=new UBO(name,size,level);

			ubo->SetData(data);

			return ubo;
		}

		UBO::UBO(const char *name,int size,int level):ShaderDataBlock(name,size)
		{
			glGenBuffers(1,&buffer_id);

			update_level=(level?level:HGL_DYNAMIC_DRAW);
		}

		UBO::~UBO()
		{
			glDeleteBuffers(1,&buffer_id);
			glBindBuffer(GL_UNIFORM_BUFFER,0);
		}

		void UBO::SetData(void *data)
		{
			glBindBuffer(GL_UNIFORM_BUFFER,buffer_id);
			glBufferData(GL_UNIFORM_BUFFER,block_size,data,update_level);
		}

		void UBO::ChangeData(void *data,int offset,int size)
		{
			if(offset+size>block_size)
			{
				LOG_ERROR(u8"UBO::ChangeData数据长度出错，block:"+UTF8String(shader_name)+UTF8String("offset=")+UTF8String(offset)+u8"size="+UTF8String(size));
				return;
			}

			glBindBuffer(GL_UNIFORM_BUFFER,buffer_id);
			glBufferSubData(GL_UNIFORM_BUFFER,offset,size,data);
		}
	}//namespace graph
}//namespace hgl
