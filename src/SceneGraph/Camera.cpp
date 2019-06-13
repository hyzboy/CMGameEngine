#include<hgl/graph/Camera.h>
#include<hgl/algorithm/VectorMath.h>
#include<iostream>
namespace hgl
{
    namespace graph
    {
         void put(const char *name,const Matrix4f &mat)
         {
             for(int r=0;r<4;r++)
             {
                 std::cout<<"mat["<<name<<"]["<<r<<"] ";
                 for(int c=0;c<4;c++)
                     std::cout<<mat[r][c]<<"\t";
                 std::cout<<std::endl;
             }
         }

        inline Matrix4f Perspective(const float fov,const float aspect,const float znear,const float zfar)
        {
            const float bottom=znear*tanf(fov*HGL_PI/360.0f);
            const float top=-bottom;
            const float left=top*aspect;
            const float right=bottom*aspect;

            return Matrix4f(2.0f*znear/(right-left),    0.0f,                       (right+left)/(right-left),          0.0f,
                            0.0f,                       2.0f*znear/(bottom-top),    (bottom+top)/(bottom-top),          0.0f,
                            0.0f,                       0.0f,                       -(zfar+znear)/(zfar-znear),         -(2.0f*zfar*znear)/(zfar-znear),
                            0.0f,                       0.0f,                       -1.0f,                              0.0f);
        }

        inline Matrix4f LookAt(const Vector3f &eye,const Vector3f &target,const Vector3f &up)
        {
            Vector3f forward=target-eye;

            normalize(forward);

            Vector3f side=cross(forward,up);

            normalize(side);

            Vector3f nup=cross(side,forward);

            Matrix4f result(side.x,     side.y,     side.z,     0.0f,
                            nup.x,      nup.y,      nup.z,      0.0f,
                            -forward.x, -forward.y, -forward.z, 0.0f,
                            0.0f,       0.0f,       0.0f,       1.0f);

            return result*translate(-eye);
        }

        void CameraToFrustum(Frustum *f,const Camera *cam)
        {
            if(!f||!cam)return;

            f->SetVerticalFovAndAspectRatio(DegToRad(cam->fov),cam->width/cam->height);
            f->SetViewPlaneDistances(cam->znear,cam->zfar);

            //Matrix4f projection_matrix=f->ProjectionMatrix();     //可以用Frustum来算projection matrix
        }

        void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam)
        {
            *proj=hgl::graph::Perspective(cam->fov,cam->width/cam->height,cam->znear,cam->zfar);
            *mv=hgl::graph::LookAt(cam->eye,cam->center,cam->up_vector);
        }
    }//namespace graph
}//namespace hgl
