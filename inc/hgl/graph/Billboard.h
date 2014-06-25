#ifndef HGL_GRAPH_BILLBOARD_INCLUDE
#define HGL_GRAPH_BILLBOARD_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		namespace billboard
		{
			/**
			* 取得公告板向上以及向右量
			*/
			void GetBillboardUpRightVector(const Matrix4f &modelview,Vector3f &up,Vector3f &right);

			/**
			* 取得一个公告板的坐标值<br>
			*<br>
			*    d---------c <br>
			*    |         | <br>
			*    |         | <br>
			*    |         | <br>
			*    a----X----b <br>
			*<br>
			* @param center 公告板下方的中间的点坐标（图中的X）
			* @param v 取出来的坐标值（以图中的a,b,c,d为序）
			*/
			void GetUpBillboard(const Matrix4f &modelview,const Vector3f &center,float width,float height,Vector3f *v);
			void GetUpBillboard(const Vector3f &up,const Vector3f &right,const Vector3f &center,float width,float height,Vector3f *v);

			/*
			* 取得一个公告板的坐标值<br>
			*<br>
			*    d---------c <br>
			*    |         | <br>
			*    |    X    | <br>
			*    |         | <br>
			*    a---------b <br>
			*<br>
			* @param center 公告板正中央的点坐标（图中的X）
			* @param v 取出来的坐标值（以图中的a,b,c,d为序）
			*/
			void GetCenterBillboard(const Matrix4f &modelview,const Vector3f &center,float width,float height,Vector3f *v);
			void GetCenterBillboard(const Vector3f &up,const Vector3f &right,const Vector3f &center,float width,float height,Vector3f *v);

			/**
			* 真公告板<br>
			* 根据摄象机和目标数据计算而得，会改变当前矩阵
			*/
			void CylindricalBillboard(Matrix4f &rotate_mat,const Vector3f &cam, const Vector3f &worldPos);

			/**
			* 真公告板<br>
			* 根据摄象机和目标数据计算而得，会改变当前矩阵
			*/
			void SphericalBillboard(Matrix4f &rotate_mat,const Vector3f &cam, const Vector3f &worldPos);
		}

		using namespace billboard;
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_BILLBOARD_INCLUDE
