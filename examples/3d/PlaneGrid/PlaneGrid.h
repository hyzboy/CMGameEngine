#ifndef PLANE_GRID_INCLUDE
#define PLANE_GRID_INCLUDE

#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    namespace graph
    {
        class VertexArray;
        class Material;
        class Renderable;

        class PlaneGrid
        {
            VertexArray *grid_data;
            Material *mtl;
            Renderable *grid;

        public:

            PlaneGrid(int,int);
            ~PlaneGrid();

            void Render(const Matrix4f *,const Matrix4f *);
        };//class PlaneGrid
    }//namespace graph
}//namespace hgl
#endif//PLANE_GRID_INCLUDE
