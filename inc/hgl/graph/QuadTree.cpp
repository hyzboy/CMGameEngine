#ifndef HGL_GRAPH_QUAD_TREE_SOURCE
#define HGL_GRAPH_QUAD_TREE_SOURCE

#include<hgl/graph/QuadTree.h>
namespace hgl
{
	namespace graph
	{
		template<typename T>
		QuadTree<T>::QuadTree()
		{
			level=0;
			total=0;

			node_list=nullptr;
			level_list=nullptr;
			level_count=nullptr;
		}

		template<typename T>
		QuadTree<T>::~QuadTree()
		{
			Clear();
		}

		template<typename T>
		void QuadTree<T>::Clear()
		{
			SAFE_CLEAR_ARRAY(level_count);
			SAFE_CLEAR_ARRAY(level_list);
			SAFE_CLEAR_ARRAY(node_list);
		}

		template<typename T>
		void QuadTree<T>::Create(int l)
		{
			Clear();

			level=l;
			level_list=new T *[level];
			level_count=new int[level];

			total=1;

			level_count[0]=1;

			for(int i=1;i<level;i++)
			{
				level_count[i]=level_count[i-1]*4;
				total+=level_count[i];
			}

			node_list=new T[total];

			level_list[0]=node_list;

			node_list->prev=nullptr;			//0级的上一级是NULL

			for(int i=1;i<level;i++)
				level_list[i]=level_list[i-1]+level_count[i-1];

			T *cur,*sub;

			for(int i=0;i<level;i++)
			{
				cur=level_list[i];

				if(i<level-1)
					sub=level_list[i+1];
				else
					sub=nullptr;				//最后一级

				for(int j=0;j<level_count[i];j++)
				{
					if(sub)
					{
						cur->sub[0]=sub++;
						cur->sub[1]=sub++;
						cur->sub[2]=sub++;
						cur->sub[3]=sub++;

						cur->sub[0]->prev=cur;
						cur->sub[1]->prev=cur;
						cur->sub[2]->prev=cur;
						cur->sub[3]->prev=cur;
					}
					else
					{
						cur->sub[0]=nullptr;
						cur->sub[1]=nullptr;
						cur->sub[2]=nullptr;
						cur->sub[3]=nullptr;
					}

					cur++;
				}
			}
		}

		template<typename T>
		T *QuadTree<T>::GetLevelNode(int l,int &c)
		{
			if(!level_list||l<0||l>=level)return(nullptr);

			c=level_count[l];
			return level_list[l];
		}
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_QUAD_TREE_SOURCE
