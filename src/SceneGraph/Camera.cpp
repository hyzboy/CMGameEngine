#include<hgl/graph/Camera.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam)
		{
#ifdef MATH_USE_MGL
            *proj=perspective_wh(cam->width,cam->height,cam->znear,cam->zfar);

            *mv=lookAt(cam->eye,cam->center,cam->forward_vector,cam->local_up_vector,cam->world_up_vector);
#else
            *proj=perspective_yfov(cam->fov,cam->width/cam->height,cam->znear,cam->zfar);

            *mv=lookAt(cam->eye,cam->center,cam->world_up_vector);
#endif//MATH_USE_MGL
		}
	}//namespace graph
}//namespace hgl
