#ifndef HGL_GRAPH_RENDER_OBJECT_INCLUDE
#define HGL_GRAPH_RENDER_OBJECT_INCLUDE

namespace hgl
{
	namespace graph
	{
		class Renderable;

        enum RenderMode
        {
            rmNone=0,

            rmNormal,                                       ///<正常渲染
            rmDepth,                                        ///<正常仅深度渲染
            rmShadowMapsDepth,                              ///<ShadowMaps深度纹理渲染
            rmOQDepth,                                      ///<OQ查询用深度纹理渲染
            rmOQShadowMapsDepth,                            ///<ShadowMaps用OQ查询深度纹理渲染
            rmDeferredShadowMaps,                           ///<延迟阴影渲染
            rmDeferredLight,                                ///<延迟光照渲染
            rmDeferredDecal,                                ///<延迟贴花渲染
            rmDeferredFinal,                                ///<延迟渲染最终混合

            rmEnd
        };//

		class RenderObject
		{
		protected:

			Renderable *data;

            struct RenderItem
            {
                uint        vao;
                int *       location;                       ///<shader绑定变量地址
                RenderState state;                          ///<渲染状态
            };

            Map<GLSL *,RenderItem> render_items;            ///<渲染项

		public:

			RenderObject(Renderable *r)
			{
				data = r;
			}

			virtual ~RenderObject();

			virtual bool Add(GLSL *,);																///<刷新数据
			virtual bool Use();																		///<使用这个对象
		};//class RenderObject
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_OBJECT_INCLUDE
