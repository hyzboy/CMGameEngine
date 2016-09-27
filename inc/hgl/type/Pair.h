#ifndef HGL_TYPE_PAIR_INCLUDE
#define HGL_TYPE_PAIR_INCLUDE

#include<hgl/CompOperator.h>

namespace hgl
{
	/**
	 * 成对数据模板
	 */
	template<typename L,typename R> struct Pair														///成对数据模板
	{
		L left;
		R right;

		typedef Pair<L,R> SelfClass;

	public:

		Pair()HGL_DEFAULT_MEMFUNC;

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

		virtual ~Pair()HGL_DEFAULT_MEMFUNC;

		SelfClass &operator=(const SelfClass &p)
		{
			     left=p.first;
			     right=p.second;

			return(*this);
		}

		SelfClass &operator=(const SelfClass *p)
		{
			if(!p)return(*this);

			     left=p->first;
			     right=p->second;

			return(*this);
		}

		void Swap(Pair<R,L> &p)
		{
			p.first=right;
			p.second=left;
		}
	};//template<typename L,typename R> struct Pair

	/**
     * 左值比较数据对
     */
	template<typename L,typename R> struct LeftIndexPair:public Pair<L,R>
	{
        virtual int Comp(const L &l)
        {
            return l-this->left;
        }

        CompOperator(const L &,Comp);
    };//template<typename L,typename R> struct LeftIndexPair:public Pair<L,R>

    /**
     * 右值比较数据对
     */
    template<typename L,typename R> struct RightIndexPair:public Pair<L,R>
	{
        virtual int Comp(const R &r)
        {
            return r-this->right;
        }

        CompOperator(const R &,Comp);
    };//template<typename L,typename R> struct RightIndexPair:public Pair<L,R>
}//namespace hgl
#endif//HGL_TYPE_PAIR_INCLUDE
