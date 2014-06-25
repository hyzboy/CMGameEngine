#include<hgl/graph/Camera.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam)
		{
			*proj=perspective_wh(cam->width,cam->height,cam->znear,cam->zfar);
//			*proj=perspective_yfov(cam->yfov,cam->width/cam->height,cam->znear,cam->zfar);

			*mv=lookAt(cam->eye,cam->center,cam->upvector);
		}
	}//namespace graph
}//namespace hgl
