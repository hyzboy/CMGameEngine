#include<hgl/graph/VertexArray.h>

namespace hgl
{
    namespace graph
    {
        VertexArray::VertexArray(uint prim)
        {
            primitive=prim;

            vb_color_format=HGL_COLOR_NONE;

            NEW_NULL_ARRAY(vertex_buffer,VertexBufferBase *,vbtEnd);
        }

        VertexArray::~VertexArray()
        {
            SAFE_CLEAR_OBJECT_ARRAY(vertex_buffer,vbtEnd);
        }

        /**
        * 设置顶点数据缓冲区
        * @param vbt 缓冲区类型
        * @param vb 数据缓冲区
        * @return 是否设置成功
        */
        bool VertexArray::SetVertexBuffer(VertexBufferType vbt,VertexBufferBase *vb)
        {
            if(vbt<0||vbt>=vbtEnd)
            {
                LOG_ERROR(OS_TEXT("VertexArray::SetVertexBuffer设置的数据类型错误：")+OSString(vbt));
                return(false);
            }

            if(vertex_buffer[vbt])
            {
                if(vertex_buffer[vbt]==vb)return(true); //有重复设的情况

                LOG_ERROR(OS_TEXT("VertexArray::SetVertexBuffer设置的数据缓冲区已有数据"));
                return(false);
            }

            vertex_buffer[vbt]=vb;

            _SetVertexBuffer(vbt,vb);       //各种真实渲染器处理

            return(true);
        }

        /**
        * 清除顶点缓冲区数据
        * @param vbt 要清除的数据缓冲区类型
        * @return 是否清除成功
        */
        bool VertexArray::ClearVertexBuffer(VertexBufferType vbt)
        {
            if(vbt<0||vbt>=vbtEnd)
            {
                LOG_ERROR(OS_TEXT("VertexArray::ClearVertexBuffer要清除的数据类型错误：")+OSString(vbt));
                return(false);
            }

            if(!vertex_buffer[vbt])
            {
                LOG_ERROR(OS_TEXT("VertexArray::ClearVertexBuffer要清除的数据缓冲区没有数据：")+OSString(vbt));
                return(false);
            }

            delete vertex_buffer[vbt];
            vertex_buffer[vbt]=nullptr;

            return(true);
        }

        VertexBufferBase *VertexArray::GetVertexBuffer(VertexBufferType vbt)                     ///<取得顶点缓冲区数据
        {
            if(vbt<0||vbt>=vbtEnd)return(nullptr);

            return(vertex_buffer[vbt]);
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
