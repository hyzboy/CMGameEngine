#include"UBO.h"
#include<hgl/type/Stack.h>
#include<glew/include/GL/glew.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/VertexBuffer.h>

/**
 * Unifrom Buffer Object 用法
 *
 * shader中定义：
 *
 *          uniform ubo_block
 *          {
 *              ....
 *          };
 *
 * C代码中：
 *
 *          ubo_index=glGetUniformBlockIndex(program,"ubo_block");            //取得ubo块变量索引
 *
 *          uint bind_point=1;                                                  //从1开始，自己编号
 *
 *          glUniformBlockBinding(program,ubo_index,bind_point);
 *
 *          glGenBuffer(1,&buffer_id);
 *          glBindBufferBase(GL_UNIFORM_BUFFER,bind_point,buffer_id);
 *
 */

//注：较大数据请使用SSBO

namespace hgl
{
	namespace graph
	{
        //绑定点是全局不可重用的，所以使用一个全局计数
        static Stack<int> uniform_block_binding_stack;                                               ///<绑定点计数

        static int max_uniform_block_binding=0;
        static int max_uniform_block_size=0;

        bool InitUBO()
        {
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,&max_uniform_block_binding);
            glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&max_uniform_block_size);

            if(max_uniform_block_binding<=0)
                return(false);

            for(int i=1;i<=max_uniform_block_binding;i++)
                uniform_block_binding_stack.Push(i);

            return(true);
        }

        int GetMaxShaderBlockBinding()
        {
            return max_uniform_block_binding;
        }

        int GetMaxShaderBlockSize()
        {
            return max_uniform_block_size;
        }

        int AcquireShaderBlockBinding()
        {
            int result;

            if(uniform_block_binding_stack.Pop(result))
                return result;

            return -1;
        }

        void ReleaseShaderBlockBinding(int bb)
        {
            if(bb<=0)return;

            uniform_block_binding_stack.Push(bb);
        }

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
            if(!name||!(*name))
            {
                LOG_ERROR("block name error");
                return(nullptr);
            }

            if(size>max_uniform_block_size)
            {
                LOG_ERROR(U8_TEXT("block size very large,UBO max ")+UTF8String(max_uniform_block_size)+U8_TEXT(", need ")+UTF8String(size));

                return(nullptr);
            }

			ShaderDataBlock *ubo=new UBO(name,size,level);

			ubo->SetData(data);

			return ubo;
		}

		UBO::UBO(const char *name,int size,int level):ShaderDataBlock(name,size)
		{
			glGenBuffers(1,&buffer_id);

            if(buffer_id==0)return;

			update_level=(level?level:HGL_DYNAMIC_DRAW);
		}

		UBO::~UBO()
		{
            if(!buffer_id)
                return;

			glDeleteBuffers(1,&buffer_id);
			glBindBuffer(GL_UNIFORM_BUFFER,0);
		}

		bool UBO::SetData(void *data)
		{
            if(!buffer_id)return(false);

			glBindBuffer(GL_UNIFORM_BUFFER,buffer_id);
			glBufferData(GL_UNIFORM_BUFFER,block_size,data,update_level);

            return(true);
		}

		bool UBO::ChangeData(void *data,int offset,int size)
		{
            if(!buffer_id)return(false);

			if(offset+size>block_size)
			{
				LOG_ERROR(u8"UBO::ChangeData数据长度出错，block:"+UTF8String(shader_name)+UTF8String("offset=")+UTF8String(offset)+u8"size="+UTF8String(size));
				return(false);
			}

			glBindBuffer(GL_UNIFORM_BUFFER,buffer_id);
			glBufferSubData(GL_UNIFORM_BUFFER,offset,size,data);

            return(true);
		}

        void UBO::Binding(int bb)
        {
            if(!buffer_id)return;

            glBindBufferBase(GL_UNIFORM_BUFFER,bb,buffer_id);
        }
	}//namespace graph
}//namespace hgl
