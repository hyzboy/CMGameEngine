#include<hgl/graph/VertexBuffer.h>

namespace hgl
{
    namespace graph
	{
        VertexBufferBase::VertexBufferBase(uint level, uint size)
        {
            dc_num = 0;

            bytes = size;

            mem_data = hgl_malloc(size);            //在很多情况下，hgl_malloc分配的内存是对齐的，这样有效率上的提升
            mem_end = ((char *)mem_data) + size;

            data_level = level;
        }

        VertexBufferBase::~VertexBufferBase()
        {
            hgl_free(mem_data);
        }
		
        void VertexBufferBase::ChangeVertexBuffer(int, int, void *)
		{
		}

        bool VertexBufferBase::CreateVertexBuffer(uint)
        {
			return(true);
        }
    }//namespace graph
}//namespace hgl
