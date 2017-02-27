#include<hgl/graph/VertexArray.h>

namespace hgl
{
    namespace graph
    {
        VertexArray::VertexArray(uint prim)
        {
            primitive=prim;

            vb_pixel_compoment=HGL_PC_NONE;
        }

        /**
        * 设置顶点数据缓冲区
        * @param vbt 缓冲区类型
        * @param vb 数据缓冲区
        * @return 是否设置成功
        */
        bool VertexArray::SetVertexBuffer(VertexBufferType vbt,VertexBufferBase *vb)
        {
            if(!vertex_buffer.Set(vbt,vb))return(false);

            _SetVertexBuffer(vbt,vb);       //各种真实渲染器处理

            return(true);
        }

        /**
        * 取得可绘制数据数量
        * @return 可绘制数量数量
        * @return -1 出错
        */
        int VertexArray::GetDrawCount()
        {
            if(vertex_buffer[vbtIndex])
                return vertex_buffer[vbtIndex]->GetCount();

            if(vertex_buffer[vbtVertex])
                return vertex_buffer[vbtVertex]->GetCount();

            return(-1);
        }
    }//namespace graph
}//namespace hgl
