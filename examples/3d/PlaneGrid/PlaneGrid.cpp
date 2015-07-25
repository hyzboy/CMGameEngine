#include"PlaneGrid.h"
#include<hgl/graph/Render.h>
#include<hgl/graph/Material.h>			//Material
#include<hgl/graph/Renderable.h>		//Renderable
#include<hgl/graph/InlineRenderable.h>	//CreateRenderablePlaneGrid
namespace hgl
{
	namespace graph
	{
		PlaneGrid::PlaneGrid(int size,int num,bool view_x,bool view_y,bool view_z)
		{
            x=view_x;
            y=view_y;
            z=view_z;

			Vector3f v[3][4];
            Color4f c[3]={Color4f(1,0,0,1),Color4f(0,1,0,1),Color4f(0,0,1,1)};

            {
                v[0][0]=Vector3f(-size,0,-size);
                v[0][1]=Vector3f( size,0,-size);
                v[0][2]=Vector3f( size,0, size);
                v[0][3]=Vector3f(-size,0, size);
            }
			{
				v[1][0]=Vector3f(-size,-size,0);
				v[1][1]=Vector3f( size,-size,0);
				v[1][2]=Vector3f( size, size,0);
				v[1][3]=Vector3f(-size, size,0);
			}
			{
                v[2][0]=Vector3f(0,-size,-size);
                v[2][1]=Vector3f(0, size,-size);
                v[2][2]=Vector3f(0, size, size);
                v[2][3]=Vector3f(0,-size, size);
            }

            for(int i=0;i<3;i++)
            {
                grid[i]=CreateRenderablePlaneGrid(v[i][0],v[i][1],v[i][2],v[i][3],num,num,c[i]);

        #ifdef _DEBUG	//debug模式下将shader保存成文件
                grid[i]->AutoCreateShader(true,OS_TEXT("PlaneGrid"));			//自动创建shader
        #else
                grid[i]->AutoCreateShader(true);						//自动创建shader
        #endif//_DEBUG
            }
		}

		PlaneGrid::~PlaneGrid()
		{
			delete grid[0];
            delete grid[1];
            delete grid[2];
		}

		void PlaneGrid::Render(const Matrix4f *proj,const Matrix4f *mv)
		{
			if(x)DirectRender(grid[0],proj,mv);
            if(y)DirectRender(grid[1],proj,mv);
            if(z)DirectRender(grid[2],proj,mv);
		}
	}//namespace graph
}//namespace hgl
