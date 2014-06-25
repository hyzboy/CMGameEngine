#ifndef HGL_TREE_INCLUDE
#define HGL_TREE_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
	/**
	* 树型数据节点，一个节点以下可以有任意个子节点
	*
	* 注：这个类还在测试中，请不要修改这个类的源代码，如有修改意见，请致电作者。
	*/
	template<typename T> class TreeNode                                                             ///树型数据节点类
	{
	public: //属性

		T Data;                                                                                     ///<节点数据

		ObjectList<TreeNode<T> > Node;                                                              ///<子节点

	public: //方法

		TreeNode();                                                                                 ///<本类构造函数
		virtual ~TreeNode();                                                                        ///<本类析构函数
	};//template<typename T> class TreeNode
}//namespace hgl
#include<hgl/type/Tree.cpp>
#endif//HGL_TREE_INCLUDE
