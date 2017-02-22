#ifndef HGL_GRAPH_VERTEX_BUFFER_OBJECT_CONTROL_INCLUDE
#define HGL_GRAPH_VERTEX_BUFFER_OBJECT_CONTROL_INCLUDE

#include<hgl/type/DataType.h>
#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        class VertexBufferControl
        {
        protected:

            uint type;
            uint index;

        public:

            uint GetIndex()const { return index; }

        public:

            VertexBufferControl(uint t, uint i) { type = t; index = i; }
            ~VertexBufferControl()
            {
                Clear();
            }

            virtual void Set(int, void *,uint)=0;
            virtual void Change(int, int, void *)=0;
            void Clear()
            {
                if(!type||!index)return;

                glDeleteBuffers(1, &index);
                type = index = 0;
            }
        };//class VertexBufferControl
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_VERTEX_BUFFER_OBJECT_CONTROL_INCLUDE