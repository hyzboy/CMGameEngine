#ifndef HGL_TREE_CPP
#define HGL_TREE_CPP

#include<hgl/type/Tree.h>
namespace hgl
{
    /**
    * 本类构造函数
    */
    template<typename T>
    TreeNode<T>::TreeNode()
    {
    }

    /**
    * 本类析构函数
    */
    template<typename T>
    TreeNode<T>::~TreeNode()
    {
        Node.Clear();
    }
}//namespace hgl
#endif//HGL_TREE_CPP>
