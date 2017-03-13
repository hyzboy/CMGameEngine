#include<hgl/graph/Transform.h>

namespace hgl
{
    namespace graph
    {
        void TransformTranslate::UpdateToMatrix(Matrix4f &mat)const
        {
            mat=mat*translate(x,y,z);
        }

        void TransformRotate::UpdateToMatrix(Matrix4f &mat)const
        {
            //mat*=rotate(angle,x,y,z);
        }

        void TransformScale::UpdateToMatrix(Matrix4f &mat)const
        {
            mat=mat*scale(x,y,z);
        }

        void TransformMatrix::UpdateToMatrix(Matrix4f &mat)const
        {
            mat=mat*m;
        }
    }//namespace graph
}//namespace hgl
