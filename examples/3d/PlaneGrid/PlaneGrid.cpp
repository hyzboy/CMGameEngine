#include"PlaneGrid.h"
#include<hgl/graph/Render.h>
#include<hgl/graph/Material.h>			//Material
#include<hgl/graph/Renderable.h>		//Renderable
#include<hgl/graph/InlineRenderable.h>	//CreateRenderablePlaneGrid
namespace hgl
{
	namespace graph
	{
		PlaneGrid::PlaneGrid(int size,int num)
		{
			//创建顶点数据
			{
				Vector3f v[4];

				{
					v[0]=Vector3f(-size,-size,0);
					v[1]=Vector3f( size,-size,0);
					v[2]=Vector3f( size, size,0);
					v[3]=Vector3f(-size, size,0);
				}

				grid_data=CreateRenderablePlaneGrid(v[0],v[1],v[2],v[3],num,num);
			}

			//创建材质数据
			{
				mtl=new Material;

				mtl->SetColorMaterial(true);
				mtl->SetColor(1,1,1,1);
			}

			//创建可渲染对象
			grid=new Renderable(grid_data,mtl);

	#ifdef _DEBUG	//debug模式下将shader保存成文件
			grid->AutoCreateShader(true,nullptr,OS_TEXT("PlaneGrid"));			//自动创建shader
	#else
			grid->AutoCreateShader();						//自动创建shader
	#endif//_DEBUG
		}

		PlaneGrid::~PlaneGrid()
		{
			delete grid;
			delete mtl;
			delete grid_data;
		}

		void PlaneGrid::Render(const Matrix4f *proj,const Matrix4f *mv)
		{
			DirectRender(grid,proj,mv);
		}
	}//namespace graph
}//namespace hgl
