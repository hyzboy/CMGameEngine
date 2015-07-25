#include<hgl/graph/Frustum.h>

namespace hgl
{
    namespace graph
    {
        Frustum::Frustum()
        {
        }

        void Frustum::SetMatrix(const Matrix4f &projection,const Matrix4f &modelview)
        {
            float planes[6][4];

            cml::extract_frustum_planes(modelview,projection,planes,cml::z_clip_zero);

            for(int i=0;i<6;i++)
                pl[i].Set(planes[i]);
        }

        int Frustum::PointIn(const Vector3f &p) const
        {
            int result = INSIDE;

            for(int i=0; i < 6; i++)
                if (pl[i].Distance(p) < 0)
                    return OUTSIDE;

            return(result);
        }

        int Frustum::SphereIn(const Vector3f &p, float raio) const
        {
            int result = INSIDE;
            float distance;

            for(int i=0; i < 6; i++)
            {
                distance = pl[i].Distance(p);

                if (distance < -raio)
                    return OUTSIDE;
                else
                if (distance < raio)
                    result =  INTERSECT;
            }

            return(result);
        }

        int Frustum::BoxIn(const AABox &b) const
        {
            int result = INSIDE;

            for(int i=0; i < 6; i++)
            {
                if (pl[i].Distance(b.GetVertexP(pl[i].normal)) < 0)
                    return OUTSIDE;
                else if (pl[i].Distance(b.GetVertexN(pl[i].normal)) < 0)
                    result =  INTERSECT;
            }

            return(result);
        }
    }//namespace graph
}//namespace hgl
