#ifndef HGL_GRAPH_TERRAIN_INCLUDE
#define HGL_GRAPH_TERRAIN_INCLUDE

#include<hgl/graph/SceneOrient.h>
namespace hgl
{
	class VSLum;

	template<typename T> class MemBlockPool;

	namespace graph
	{
		class Renderable;

		struct HeightMapTerrainBlock:public SceneOrient
		{
			float *data;									///<高度数据

			Renderable *able;								///<可渲染数据
		};//struct HeightMapTerrainBlock

		class HeightMapTerrain:public SceneOrient
		{
			MemBlockPool<float> *block_pool;		///<高度图数据块用池

			HeightMapTerrainBlock *block;			///<高度图数据块

		public:

			HeightMapTerrain();
			HeightMapTerrain(float *,int block_width,int block_height,float height=10.0f);
			virtual ~HeightMapTerrain();

			HeightMapTerrainBlock *GetBlock(){return block;}
		};//class Terrain
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TERRAIN_INCLUDE
