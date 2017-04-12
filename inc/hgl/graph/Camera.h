#ifndef HGL_GRAPH_CAMERA_INCLUDE
#define HGL_GRAPH_CAMERA_INCLUDE

#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    using namespace algorithm;

	namespace graph
	{
		/**
		* 摄像机数据结构
		*/
		struct Camera
		{
			float width;                ///<视图宽
			float height;               ///<视图高

			float fov;		            ///<水平FOV
			float znear,zfar;           ///<Z轴上离眼睛的距离

			Vector3f eye;                   ///<眼睛坐标
			Vector3f center;                ///<视点坐标
            Vector3f world_up_vector;       ///<世界向上量
			Vector3f local_up_vector;       ///<本地向上量
            Vector3f local_forward_vector;  ///<本地向前量
		};//struct Camera

		void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam);

		/**
		* 行走模拟像机
		*/
		class WalkerCamera:public Camera
		{
		public:	//方法

			virtual void Backward(float=0.01);														///<向后
			virtual void Forward(float step=0.01){Backward(-step);}									///<向前

			virtual void Up(float step=0.01);														///<向上
			virtual void Down(float step=0.01){Up(-step);}											///<向下

			virtual void Right(float step=0.01);													///<向右
			virtual void Left(float step=0.01){Right(-step);}										///<向左

		public:	//以自身为中心旋转

			virtual void UpRotate(float=5);															///<以自身为中心向上旋转
			virtual void DownRotate(float ang=5){UpRotate(-ang);}									///<以自身为中心向下旋转

			virtual void RightRotate(float=5);														///<以自身为中心向右旋转
			virtual void LeftRotate(float ang=5){RightRotate(-ang);}								///<以自身为中心向左旋转

		public:	//以目标点为中心旋转

			virtual void CenterUpRotate(float=5);													///<以目标点为中心向上旋转
			virtual void CenterDownRotate(float ang=5){CenterUpRotate(-ang);}						///<以目标点为中心向下旋转

			virtual void CenterRightRotate(float ang=5);											///<以目标点为中心向右旋转
			virtual void CenterLeftRotate(float ang=5){CenterRightRotate(-ang);}					///<以目标点为中心向左旋转

		public:	//距离

			virtual void Distance(float=0.9);														///<调整距离
		};//class WalkerCamera
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_CAMERA_INCLUDE
