#include<hgl/Console.h>
#include<hgl/algorithm/VectorMath.h>
#include<hgl/graph/Camera.h>
#include<hgl/graph/Render.h>

using namespace hgl;
using namespace hgl::graph;

const uint width=1280;
const uint height=720;

const Vector3f  eye(100,70,80),
                center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

void SetCamera(Camera &cam)
{
    cam.fov=45.0f;
    cam.znear=4.0f;
    cam.zfar=1000.0f;

    cam.width=width;
    cam.height=height;

    cam.eye=eye;
    cam.center=center;
    cam.forward_vector=forward_vector;
    cam.up_vector=up_vector;
}

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectCode=OS_TEXT("Frustum");

    app.Init(&sii);

	Camera cam;
	Frustum f;

	Matrix4f c_proj,c_mv;

	SetCamera(cam);

	MakeCameraMatrix(&c_proj,&c_mv,&cam);
	CameraToFrustum(&f,&cam);

	return 0;
}
