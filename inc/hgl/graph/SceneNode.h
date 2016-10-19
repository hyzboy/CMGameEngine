#ifndef HGL_GRAPH_SCENE_NODE_INCLUDE
#define HGL_GRAPH_SCENE_NODE_INCLUDE

#include<hgl/type/List.h>
#include<hgl/graph/SceneOrient.h>
#include<hgl/graph/Renderable.h>
namespace hgl
{
	namespace graph
	{
		class SceneNode;
		struct Camera;
		class Frustum;

		typedef List<const SceneNode *> RenderList;													///<渲染列表类型重定义

		typedef float (*RenderListCompFunc)(Camera *,SceneNode *,SceneNode *);						///<渲染列表排序比较函数

		float CameraLengthComp(Camera *,SceneNode *,SceneNode *);									///<摄像机距离比较函数

		typedef bool (*FilterSceneNodeFunc)(const SceneNode *,void *);								///<场景节点过滤函数重定义

		bool FrustumClipFilter(const SceneNode *,void *);											///<平截头截减过滤函数

		/**
		* 场景节点数据类<br>
		* 从场景坐标变换(SceneOrient)类继承
		*/
		class SceneNode:public SceneOrient																				///场景节点类
		{
		protected:

			AABB BoundingBox;																							///<绑定盒
//			AABB LocalBoundingBox;																						///<本地坐标绑定盒
//			AABB WorldBoundingBox;																						///<世界坐标绑定盒

			Vector3f Center;																							///<中心点
			Vector3f LocalCenter;																						///<本地坐标中心点
			Vector3f WorldCenter;																						///<世界坐标中心点

		public:

			List<Renderable *> SubData;																					///<可渲染数据
			ObjectList<SceneNode> SubNode;																				///<子节点

		public:

			SceneNode()=default;
			virtual ~SceneNode()
			{
				ClearSubNode();
				ClearRenderable();
			}

			void		Add(Renderable *r){if(r)SubData.Add(r);}														///<增加一个可渲染数据
			void		ClearRenderable(){SubData.Clear();}																///<清除可渲染数据

			void		AddSubNode(SceneNode *n){if(n)SubNode.Add(n);}													///<增加一个子节点
			SceneNode *	CreateSubNode()																					///<创建一个子节点
			{
				SceneNode *sn=new SceneNode();
				SubNode.Add(sn);
				return sn;
			}

			SceneNode *	AddSubNode(Renderable *r,const Matrix4f &m)
			{
				if(!r)return(nullptr);

				SceneNode *sn=CreateSubNode();
				sn->Add(r);
				sn->SetLocalMatrix(m);
				return sn;
			}

			void		ClearSubNode(){SubNode.Clear();}																///<清除子节点

		public:	//坐标相关方法

			virtual			void		RefreshMatrix		(const Matrix4f *mat=nullptr);								///<刷新世界变换矩阵
			virtual			void		RefreshBoundingBox	();															///<刷新绑定盒

			virtual const	AABB &		GetBoundingBox		()const{return BoundingBox;}								///<取得绑定盒
//			virtual	const	AABB &		GetLocalBoundingBox	()const{return LocalBoundingBox;}							///<取得本地坐标绑定盒
//			virtual	const	AABB &		GetWorldBoundingBox	()const{return WorldBoundingBox;}							///<取得世界坐标绑定盒

			virtual const	Vector3f &	GetCenter			()const{return Center;}										///<取得中心点
			virtual const	Vector3f &	GetLocalCenter		()const{return LocalCenter;}								///<取得本地坐标中心点
			virtual const	Vector3f &	GetWorldCenter		()const{return WorldCenter;}								///<取得世界坐标中心点

		public:	//渲染列表相关方法

			virtual	bool ExpendToList(RenderList *,FilterSceneNodeFunc func=nullptr,void *func_data=nullptr)const;		///<展开到渲染列表
					bool ExpendToList(RenderList *rl,Frustum *f)const													///<展开到渲染列表(使用平截头裁剪)
						{return ExpendToList(rl,FrustumClipFilter,f);}

					bool ExpendToList(RenderList *,const Matrix4f &,const Matrix4f &,RenderListCompFunc=nullptr)const;	///<展开到渲染列表(使用平截头裁剪并排序)
					bool ExpendToList(RenderList *,Camera *,RenderListCompFunc=nullptr)const;							///<展开到渲染列表(使用摄像机平截头裁剪并排序)
		};//class SceneNode
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SCENE_NODE_INCLUDE
