#include<hgl/graph/Camera.h>
#include<hgl/algorithm/VectorMath.h>
#include<iostream>
namespace hgl
{
    namespace graph
    {
//         void put(const char *name,const Vector3f &v)
//         {
//             std::cout<<name<<": "<<v.x<<","<<v.y<<","<<v.z<<std::endl;
//         }
//
//         void put(const char *name,const Vector4f &v)
//         {
//             std::cout<<name<<": "<<v.x<<","<<v.y<<","<<v.z<<","<<v.w<<std::endl;
//         }
//
//         void put(const char *name,const Matrix4f &mat)
//         {
//             for(int r=0;r<4;r++)
//             {
//                 std::cout<<"mat["<<name<<"]["<<r<<"] ";
//                 for(int c=0;c<4;c++)
// #ifdef MATH_USE_MGL
//                     std::cout<<mat[r][c]<<"\t";
// #else
//                     std::cout<<mat[c][r]<<"\t";
// #endif//MATH_USE_MGL
//                 std::cout<<std::endl;
//             }
//         }

        inline Matrix4f Perspective(const float fov,const float aspect,const float znear,const float zfar)
        {
            const float bottom=znear*tanf(fov*HGL_PI/360.0f);
            const float top=-bottom;
            const float left=top*aspect;
            const float right=bottom*aspect;

#ifdef MATH_USE_MGL
            return Matrix4f(2.0f*znear/(right-left),    0.0f,                       (right+left)/(right-left),          0.0f,
                            0.0f,                       2.0f*znear/(bottom-top),    (bottom+top)/(bottom-top),          0.0f,
                            0.0f,                       0.0f,                       -(zfar+znear)/(zfar-znear),         -(2.0f*zfar*znear)/(zfar-znear),
                            0.0f,                       0.0f,                       -1.0f,                              0.0f);

#else
            return Matrix4f(2.0f*znear/(right-left),    0.0f,                       0.0f,                               0.0f,
                            0.0f,                       2.0f*znear/(bottom-top),    0.0f,                               0.0f,
                            (right+left)/(right-left),  (bottom+top)/(bottom-top),  -(zfar+znear)/(zfar-znear),         -1.0f,
                            0.0f,                       0.0f,                       -(2.0f*zfar*znear)/(zfar-znear),    0.0f);
#endif//MATH_USE_MGL
        }

        inline Matrix4f LookAt(const Vector3f &eye,const Vector3f &target,const Vector3f &up)
        {
            Vector3f forward=target-eye;

            normalize(forward);

//             put("forward",forward);

            Vector3f side=cross(forward,up);

            normalize(side);

//             put("side",side);

            Vector3f nup=cross(side,forward);

//             put("nup",nup);

#ifdef MATH_USE_MGL
            Matrix4f result(side.x,     side.y,     side.z,     0.0f,
                            nup.x,      nup.y,      nup.z,      0.0f,
                            -forward.x, -forward.y, -forward.z, 0.0f,
                            0.0f,       0.0f,       0.0f,       1.0f);
#else
            Matrix4f result(side.x, nup.x, -forward.x, 0.0f,
                            side.y, nup.y,  -forward.y, 0.0f,
                            side.z, nup.z,  -forward.z, 0.0f,
                            0.0f,   0.0f,   0.0f,       1.0f);

#endif//MATH_USE_MGL

            return result*translate(-eye);
        }

        void MakeCameraMatrix(Matrix4f *proj,Matrix4f *mv,const Camera *cam)
        {
// #ifdef MATH_USE_MGL
//             *proj=perspective_wh(cam->width,cam->height,cam->znear,cam->zfar);
//
//             *mv=lookAt(cam->eye,cam->center,cam->local_forward_vector,cam->local_up_vector,cam->world_up_vector);
// #else
//              *proj=perspective_yfov(cam->fov,cam->width/cam->height,cam->znear,cam->zfar);
//
//              *mv=lookAt(cam->eye,cam->center,cam->world_up_vector);
// #endif//MATH_USE_MGL

            //其实GLM/CML自带的结果也是正确的，只有MGL的产生的特殊，为统一，所以参考各方代码，自行实现一套，以保证统一

            *proj=hgl::graph::Perspective(cam->fov,cam->width/cam->height,cam->znear,cam->zfar);
            *mv=hgl::graph::LookAt(cam->eye,cam->center,cam->world_up_vector);
        }
    }//namespace graph
}//namespace hgl
