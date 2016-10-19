#ifndef HGL_GRAPH_TRANSFORM_INCLUDE
#define HGL_GRAPH_TRANSFORM_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 坐标变换处理
		*/
		class Transform
		{
		public:	//方法

			Transform()=default;
			virtual ~Transform()=default;

			virtual void UpdateToMatrix(Matrix4f &mat)const{};										///<使用这个变换更新矩阵
		};//class Transform

		/**
		* 移动变换
		*/
		class TransformTranslate:public Transform
		{
		public:	//属性

			float x,y,z;																			///<偏移值属性

		public:	//方法

			TransformTranslate(){x=y=z=0;}
			virtual ~TransformTranslate()=default;

			void UpdateToMatrix(Matrix4f &mat)const;
		};//class TransformTranslate

		/**
		* 旋转变换
		*/
		class TransformRotate:public Transform
		{
		public: //属性

			float angle;																			///<旋转角度
			float x,y,z;																			///<旋转轴

		public:	//方法

			TransformRotate(){angle=x=y=z=0;}
			virtual ~TransformRotate()=default;

			void UpdateToMatrix(Matrix4f &mat)const;
		};//class TransformRotate

		/**
		* 缩放变换
		*/
		class TransformScale:public Transform
		{
		public:	//属性

			float x,y,z;																			///<偏移值属性

		public:	//方法

			TransformScale(){x=y=z=1.0f;}
			virtual ~TransformScale()=default;

			void UpdateToMatrix(Matrix4f &mat)const;
		};//class TransformScale

		/**
		* 矩阵变换
		*/
		class TransformMatrix:public Transform
		{
		public:	//属性

			Matrix4f m;																			///<变换矩阵

		public:

			TransformMatrix();
			virtual ~TransformMatrix()=default;

			void UpdateToMatrix(Matrix4f &mat)const;
		};//class TransformMatrix
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TRANSFORM_INCLUDE
