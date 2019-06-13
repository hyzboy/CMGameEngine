#ifndef HGL_TYPE_PAIR_INCLUDE
#define HGL_TYPE_PAIR_INCLUDE

#include<hgl/platform/Platform.h>
#include<hgl/CompOperator.h>

namespace hgl
{
    /**
     * 成对数据模板
     */
    template<typename L,typename R> struct Pair                                                     ///成对数据模板
    {
        L left;
        R right;

        using SelfClass=Pair<L,R>;

    public:

        Pair()=default;

        Pair(const L &l,const R &r)
        {
            left=l;
            right=r;
        }

        Pair(const SelfClass &p)
        {
            operator=(p);
        }

        Pair(const SelfClass *p)
        {
            operator=(p);
        }

        virtual ~Pair()=default;

        SelfClass &operator=(const SelfClass &p)
        {
            left=p.left;
            right=p.right;

            return(*this);
        }

        SelfClass &operator=(const SelfClass *p)
        {
            if(!p)return(*this);

            left=p->left;
            right=p->right;

            return(*this);
        }

        void Swap(Pair<R,L> &p)
        {
            p.left=right;
            p.right=left;
        }

        //不使用CompOperator是因为某些类型无法做+-处理

        const bool operator == (const SelfClass &p)const
        {
            if(this->left!=p.left)return(false);

            return this->right==p.right;
        }

        const bool operator != (const SelfClass &p)const
        {
            if(this->left==p.left)return(false);

            return this->right!=p.right;
        }
    };//template<typename L,typename R> struct Pair
}//namespace hgl
#endif//HGL_TYPE_PAIR_INCLUDE
