#include<hgl/graph/AABox.h>

namespace hgl
{
    namespace graph
    {
        AABox::AABox(void)
        {
            corner[0] = 0;
            corner[1] = 0;
            corner[2] = 0;

            length[0] = 1.0f;
            length[1] = 1.0f;
            length[2] = 1.0f;
        }

        Vector3f AABox::GetVertexP(const Vector3f &normal) const
        {
            Vector3f res = corner;

            if (normal[0] > 0)res[0] += length[0];
            if (normal[1] > 0)res[1] += length[1];
            if (normal[2] > 0)res[2] += length[2];

            return(res);
        }

        Vector3f AABox::GetVertexN(const Vector3f &normal) const
        {
            Vector3f res = corner;

            if (normal[0] < 0)res[0] += length[0];
            if (normal[1] < 0)res[1] += length[1];
            if (normal[2] < 0)res[2] += length[2];

            return(res);
        }

        void AABox::operator+=(const AABox &box)
        {
            corner[0]=hgl_min(corner[0],box.corner[0]);
            corner[1]=hgl_min(corner[1],box.corner[1]);
            corner[2]=hgl_min(corner[2],box.corner[2]);

            corner_max[0]=hgl_max(corner_max[0],box.corner_max[0]);
            corner_max[1]=hgl_max(corner_max[1],box.corner_max[1]);
            corner_max[2]=hgl_max(corner_max[2],box.corner_max[2]);

            length=corner_max-corner;

            center=(corner+corner_max)/2.0f;
        }
    }//namespace graph
}//namespace hgl
