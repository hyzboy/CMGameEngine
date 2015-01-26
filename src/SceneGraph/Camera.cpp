#include<hgl/graph/Camera.h>
#include<hgl/VectorMath.h>
#include<Geometry/Frustum.h>
namespace hgl
{
	namespace graph
	{
		void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam)
		{
			Frustum f;

			f.SetKind(FrustumSpaceGL,FrustumRightHanded);

			f.SetViewPlaneDistances(cam->znear, cam->zfar);
			f.SetFrame(	POINT_VEC(cam->eye),
						DIR_VEC(cam->forward_vector),
						DIR_VEC(cam->up_vector));

			f.SetPerspective(HGL_PI/2.f,HGL_PI/2.f);

			*proj=f.ComputeProjectionMatrix();
			*mv=f.ComputeViewMatrix();

//			*proj=perspective_wh(cam->width,cam->height,cam->znear,cam->zfar);
////			*proj=perspective_yfov(cam->yfov,cam->width/cam->height,cam->znear,cam->zfar);
//
//			*mv=lookAt(cam->eye,cam->center,cam->forward_vector,cam->up_vector);
		}
	}//namespace graph
}//namespace hgl
