#ifndef HGL_GRAPH_VERTEX_ARRAY_INCLUDE
#define HGL_GRAPH_VERTEX_ARRAY_INCLUDE

#include<hgl/type/FixedArray.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/PixelCompoment.h>
#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 顶点阵列数据
         */
        class VertexArray
        {
        protected:

            uint primitive;                                                                                                         ///<绘制的图元类型

            ObjectFixedArray<VertexBufferBase,vbtEnd> vertex_buffer;                                                              	///<顶点数据缓冲区

			AABB aabb;                                                                                                      		///<AABB绑定盒
			OBB obb;																												///<OBB绑定盒

            PixelCompoment vb_pixel_compoment;                                                                                      ///<颜色顶点属性格式

        private:

            bool                        _SetVertexBuffer     (VertexBufferType,VertexBufferBase *);                                 ///<真实设置顶点缓冲区数据

        public:

            VertexArray(uint prim);
            ~VertexArray()=default;

            uint                        GetPrimitive        ()const{return primitive;}                                              ///<取得要绘制的图元类型

        public: //通用顶点缓冲区设置

            bool                        SetVertexBuffer     (VertexBufferType,VertexBufferBase *);                                  ///<设置顶点缓冲区数据
            VertexBufferBase *          GetVertexBuffer     (VertexBufferType vbt){return vertex_buffer[vbt];}                      ///<取得顶点缓冲区数据
            bool                        ClearVertexBuffer   (VertexBufferType vbt){return vertex_buffer.Clear(vbt);}				///<清除顶点缓冲区数据
            void						ClearAllVertexBuffer(){vertex_buffer.ClearAll();}											///<清除所有顶点缓冲区数据

        public: //顶点格式相关

            int                         GetVertexCompoment  ()const                                                                 ///<取得顶点数据成分数量
            {
                return vertex_buffer[vbtVertex]?vertex_buffer[vbtVertex]->GetComponent():0;
            }

        public: //颜色格式相关

            void                        SetVertexPixelCompoment(PixelCompoment fmt){vb_pixel_compoment=fmt;}                        ///<设置顶点属性格式
            PixelCompoment              GetVertexPixelCompoment()const             {return vb_pixel_compoment;}                     ///<取得顶点属性格式

        public: //专项顶点缓冲区设置

            template<typename T>
            bool                        SetVertex           (VertexBuffer3<T> *vb)                                                  ///<设定顶点数据
            {
                if(!vb)return(false);

                vec min_v,max_v,len;

                vb->GetBoundingBox(min_v,max_v);

				aabb.minPoint=min_v;
				aabb.maxPoint=max_v;

				obb.SetFrom(aabb);

                return SetVertexBuffer(vbtVertex,vb);
            }

            template<typename T>
            bool                        SetVertex           (VertexBuffer2<T> *vb){return SetVertexBuffer(vbtVertex,vb);}           ///<设定顶点数据

            bool                        SetIndex            (VertexBufferBase *vb){return SetVertexBuffer(vbtIndex,vb);}            ///<设置渲染数据索引
            bool                        SetColor            (VertexBufferBase *vb,PixelCompoment cf)                                ///<设置顶点颜色数据
            {
                if(!SetVertexBuffer(vbtColor,vb))
                    return(false);

                SetVertexPixelCompoment(cf);
                return(true);
            }
            bool                        SetNormal           (VertexBufferBase *vb){return SetVertexBuffer(vbtNormal,vb);}           ///<设置渲染顶点法线数据
            bool                        SetTangent          (VertexBufferBase *vb){return SetVertexBuffer(vbtTangent,vb);}          ///<设置渲染顶点切线数据
            bool                        SetSecondColor      (VertexBufferBase *vb){return SetVertexBuffer(vbtSecondColor,vb);}      ///<设置顶点第二颜色数据
            bool                        SetFogCoord         (VertexBufferBase *vb){return SetVertexBuffer(vbtFogCoord,vb);}         ///<设置顶点雾数据

        public: //绑定盒相关

            const AABB &				GetAABB				()const{return aabb;}													///<取得AABB绑定盒
            const OBB &					GetOBB				()const{return obb;}													///<取得OBB绑定盒

            const Vector3f              GetCenter           ()const                                                                 ///<取得中心点
            {
				return POINT_TO_FLOAT3(obb.CenterPoint());
            }

            void                        GetMinMaxVertex     (Vector3f &min_v,Vector3f &max_v)                                       ///<取得最小顶点和最大顶点
            {
				min_v=POINT_TO_FLOAT3(aabb.minPoint);
				max_v=POINT_TO_FLOAT3(aabb.maxPoint);
            }

        public:

            int                         GetDrawCount     ();                                                                        ///<取得可绘制的数据总数量
        };//class VertexArray
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_VERTEX_ARRAY_INCLUDE
