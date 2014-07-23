#ifndef HGL_GRAPH_QUAD_TREE_INCLUDE
#define HGL_GRAPH_QUAD_TREE_INCLUDE

#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 四叉树节点
		*/
		template<typename T> struct QuadTreeNode
		{
			AABB box;

			T *prev;	//上一级
			T *sub[4];	//子节点
		};//struct QuadTreeNode

		/**
		* 四叉树基本结构
		*/
		template<typename T> class QuadTree
		{
			int level;																				///<最大级别

			int total;																				///<总节点数

			T *node_list;																			///<节点列表

			T **level_list;																			///<分级列表
			int *level_count;																		///<每一级的节点数

		public:

			QuadTree();
			virtual ~QuadTree();

			void	Create(int);																	///<创建四叉树
			void	Clear();																		///<清除四叉树

			T *		GetRoot		()				{return(node_list?node_list:0);}					///<取得根节点
			int		GetLevel	()const			{return level;}										///<取得节点级数
			int		GetTotalNode()const			{return total;}										///<取得总节点数
			T *		GetLevelNode(int l,int &c);														///<取得指定级节点的列表
		};//class QuadTree
	}//namespace graph
}//namespace hgl
#include<hgl/graph/QuadTree.cpp>
#endif//HGL_GRAPH_QUAD_TREE_INCLUDE
