#include<hgl/graph/SSBO.h>

#include<hgl/type/Stack.h>
#include<glew/include/GL/glew.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/VertexBuffer.h>

namespace hgl
{
    namespace graph
    {
        namespace
        {
            //绑定点是全局不可重用的，所以使用一个全局计数
            static Stack<int> ssbo_binding_stack;                                               ///<绑定点计数

            static int max_ssbo_binding=0;
            static int max_ssbo_size=0;
        }//namespace

        namespace ssbo
        {
            bool Init()
            {
                glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,&max_ssbo_binding);
                glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE,&max_ssbo_size);

                if(max_ssbo_binding<=0)
                    return(false);

                for(int i=0;i<max_ssbo_binding;i++)
                    ssbo_binding_stack.Push(i);

                return(true);
            }

            int GetMaxBinding()
            {
                return max_ssbo_binding;
            }

            int GetMaxSize()
            {
                return max_ssbo_size;
            }

            int AcquireBindingPoint()
            {
                int result;

                if(ssbo_binding_stack.Pop(result))
                    return result;

                return -1;
            }

            void ReleaseBindingPoint(int bb)
            {
                if(bb==-1)return;

                ssbo_binding_stack.Push(bb);
            }
        }//namespace ssbo

        SSBO::SSBO(const UTF8String &n,uint p,uint i,uint l)
        {
            block_name=n;
            program=p;
            block_index=i;
            binding_point=ssbo::AcquireBindingPoint();
            level=l;

            use_map=0;

            glShaderStorageBlockBinding(program,block_index,binding_point);

            {
                glCreateBuffers(1,&ssbo);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo);

                buffer=nullptr;
                size=0;
            }
        }

        SSBO::~SSBO()
        {
            ssbo::ReleaseBindingPoint(binding_point);
            glDeleteBuffers(1,&ssbo);
        }

        /**
         * 只读访问一块数据
         * @param start 起始字节
         * @param access_size 要访问的长度
         */
        void *SSBO::ReadMap(int start,int access_size)
        {
            if(use_map!=0)
                return(nullptr);

            if(access_size<=0)
                return(nullptr);

            use_map=GL_MAP_READ_BIT;

//            glBindBufferBase(GL_SHADER_STORAGE_BUFFER,binding_point,ssbo);
            
            buffer=glMapNamedBufferRange(ssbo,0,size,GL_MAP_READ_BIT);

            return(buffer);
        }

        /**
         * 只写访问一块数据
         * @param start 起始字节
         * @param access_size 要访问的长度
         */
        void *SSBO::WriteMap(int start,int access_size)
        {
            if(use_map!=0)
                return(nullptr);

            use_map=GL_MAP_WRITE_BIT;

//            glBindBufferBase(GL_SHADER_STORAGE_BUFFER,binding_point,ssbo);

            map_start=start;
            map_size=access_size;

            glNamedBufferStorage(ssbo, map_size, nullptr, GL_MAP_WRITE_BIT);
            return glMapNamedBufferRange(ssbo,map_start,map_size,GL_MAP_WRITE_BIT|GL_MAP_FLUSH_EXPLICIT_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
        }

        void SSBO::Unmap()
        {
            if(use_map==GL_MAP_WRITE_BIT)
                glFlushMappedNamedBufferRange(ssbo,map_start,map_size);

            use_map=0;
            buffer=nullptr;
            glUnmapNamedBuffer(ssbo);
        }

        bool SSBO::Commit(void *buf,int buf_size)
        {
            if(use_map!=0)
                return(false);

            if(!buf||buf_size<=0)
                return(false);

            if(buf_size>max_ssbo_size)
                return(false);

            size=buf_size;

            glNamedBufferData(ssbo,size,buf,level);
            return(true);
        }
    }//namespace graph
}//namespace hgl
