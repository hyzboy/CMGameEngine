#include"PlaneGrid.h"
#include<hgl/graph/Render.h>
#include<hgl/graph/Material.h>			//Material
#include<hgl/graph/Renderable.h>		//Renderable
#include<hgl/graph/InlineRenderable.h>	//CreateRenderableCube
namespace hgl
{
	namespace graph
	{
		PlaneGrid::PlaneGrid(int size,int num)
		{
			Vector3f v[4];

            v[0]=Vector3f(-size,-size,0);
            v[1]=Vector3f( size,-size,0);
            v[2]=Vector3f( size, size,0);
            v[3]=Vector3f(-size, size,0);

			grid=CreateRenderablePlaneGrid(v[0],v[1],v[2],v[3],num,num);

			//创建材质
			{
				Material *mtl=grid->GetMaterial();

				mtl->SetColorMaterial(true);					//使用Material中的颜色
				mtl->SetColor(0.75,0.75,0.75,1.0);
			}

	#ifdef _DEBUG	//debug模式下将shader保存成文件
			grid->AutoCreateShader(true,OS_TEXT("PlaneGrid"));			//自动创建shader
	#else
			grid->AutoCreateShader(true);						//自动创建shader
	#endif//_DEBUG
		}

		PlaneGrid::~PlaneGrid()
		{
			delete grid;
		}

		void PlaneGrid::Render(const Matrix4f *proj,const Matrix4f *mv)
		{
			DirectRender(grid,proj,mv);
		}
	}//namespace graph
}//namespace hgl
