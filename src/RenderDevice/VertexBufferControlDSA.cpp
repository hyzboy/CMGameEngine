#include"VertexBufferControl.h"
#include<hgl/graph/GL/glew.h>

namespace hgl
{
    namespace graph
    {
        class VertexBufferControlDSA:public VertexBufferControl
        {
        public:

            using VertexBufferControl::VertexBufferControl;

            void Set(int size, void *data,uint data_level)
            {
                glNamedBufferData(this->index, size, data, data_level);
            }

            void Change(int start, int size, void *data)
            {
                glNamedBufferSubData(this->index, start, size, data);                
            }
        };//class VertexBufferControlDSA

        VertexBufferControl *CreateVertexBufferControlDSA(uint type)
        {
            uint index;

            glCreateBuffers(1,&index);
            return(new VertexBufferControlDSA(type,index));
        }
    }//namespace graph
}//namespace hgl
