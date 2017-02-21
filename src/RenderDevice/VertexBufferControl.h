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
                glDeleteBuffers(1, &index);
            }

            virtual void Set(int, void *,uint);
            virtual void Change(int, int, void *);
            virtual void Clear();
        };//class VertexBufferControl
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_VERTEX_BUFFER_OBJECT_CONTROL_INCLUDE