#ifndef HGL_TYPE_PAIR_INCLUDE
#define HGL_TYPE_PAIR_INCLUDE

#include<hgl/platform/Platform.h>
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

		using SelfClass=Pair<L,R>;

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
	};//template<typename L,typename R> struct Pair

	///**
 //    * 左值比较数据对
 //    */
	//template<typename L,typename R> struct LeftIndexPair:public Pair<L,R>
	//{
 //       using SuperClass=Pair<L,R>;
 //       using LIPCalss=struct LeftIndexPair<L,R>;

 //       virtual int Comp(const L &l)
 //       {
 //           return this->left-l;
 //       }

 //       virtual int Comp(const SuperClass &p)
 //       {
 //           return this->left-p.left;
 //       }

 //       virtual int Comp(const LIPCalss &p)
 //       {
 //           return this->left-p.left;
 //       }

 //       CompOperator(const L &,Comp);
 //       CompOperator(const SuperClass &,Comp);
 //       CompOperator(const LIPCalss &,Comp);
 //   };//template<typename L,typename R> struct LeftIndexPair:public Pair<L,R>

 //   /**
 //    * 右值比较数据对
 //    */
 //   template<typename L,typename R> struct RightIndexPair:public Pair<L,R>
	//{
 //       using SuperClass=Pair<L,R>;
 //       using RIPClass=struct RightIndexPair<L,R>;

 //       virtual int Comp(const R &r)
 //       {
 //           return this->right-r;
 //       }

 //       virtual int Comp(const SuperClass &p)
 //       {
 //           return this->right-p.right;
 //       }

 //       virtual int Comp(const RIPClass &p)
 //       {
 //           return this->right-p.right;
 //       }

 //       CompOperator(const R &,Comp);
 //       CompOperator(const SuperClass &,Comp);
 //       CompOperator(const RIPClass &,Comp);
 //   };//template<typename L,typename R> struct RightIndexPair:public Pair<L,R>

 //   template<typename L,typename R> struct LeftPointerPair
 //   {
 //       using PairClass=Pair<L,R>;
 //       using SelfClass=LeftPointerPair<L,R>;

 //       PairClass *data;

 //   public:

 //       LeftPointerPair(PairClass *p)
 //       {
 //           data=p;
 //       }

 //       virtual int Comp(const PairClass &p)
 //       {
 //           return data->left-p.left;
 //       }

 //       virtual int Comp(const SelfClass &p)
 //       {
 //           return data->left-p.data->left;
 //       }

 //       CompOperator(const PairClass &,Comp);
 //       CompOperator(const SelfClass &,Comp);
 //   };//template<typename L,typename R> struct LeftPointerPair

 //   template<typename L,typename R> struct RightPointerPair
 //   {
 //       using PairClass=Pair<L,R>;
 //       using SelfClass=RightPointerPair<L,R>;

 //       PairClass *data;

 //   public:

 //       RightPointerPair(PairClass *p)
 //       {
 //           data=p;
 //       }

 //       virtual int Comp(const PairClass &p)
 //       {
 //           return data->right-p.right;
 //       }

 //       virtual int Comp(const SelfClass &p)
 //       {
 //           return data->right-p.data->right;
 //       }

 //       CompOperator(const PairClass &,Comp);
 //       CompOperator(const SelfClass &,Comp);
 //   };//template<typename L,typename R> struct RightPointerPair
}//namespace hgl
#endif//HGL_TYPE_PAIR_INCLUDE
