#ifndef PLANE_GRID_INCLUDE
#define PLANE_GRID_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		class Renderable;

		class PlaneGrid
		{
            bool x,y,z;
			Renderable *grid[3];
            Renderable *axis;

		public:

			PlaneGrid(int,int,bool,bool,bool);
			~PlaneGrid();

			void Render(const Matrix4f *,const Matrix4f *);
		};//class PlaneGrid
	}//namespace graph
}//namespace hgl
#endif//PLANE_GRID_INCLUDE
