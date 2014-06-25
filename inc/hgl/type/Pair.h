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
		L first;
		R second;

		typedef Pair<L,R> SelfClass;

	public:

		Pair()HGL_DEFAULT_MEMFUNC;

		Pair(const L &l,const R &r)
		{
			first=l;
			second=r;
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
			first=p.first;
			second=p.second;

			return(*this);
		}

		SelfClass &operator=(const SelfClass *p)
		{
			if(!p)return(*this);

			first=p->first;
			second=p->second;

			return(*this);
		}

		void Swap(Pair<R,L> &p)
		{
			p.first=second;
			p.second=first;
		}

		int Comp(const SelfClass &p)const
		{
			if(first==p.first)
				return second-p.second;

			return first-p.first;
		}

		int Comp(const L &l)const
		{
			return first-l;
		}

		CompOperator(const SelfClass &,Comp)
		CompOperator(const L &,Comp)
	};//template<typename L,typename R> struct Pair
}//namespace hgl
#endif//HGL_TYPE_PAIR_INCLUDE
