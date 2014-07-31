#ifndef HGL_TYPE_ACTIVE_CHAIN_INCLUDE
#define HGL_TYPE_ACTIVE_CHAIN_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
	/**
	* 活跃链表缓冲区管理模板(当缓冲区满时，将最长时间没有使用的清除)<br>
	* 现这个模板使用双头链表，每次添加或使用的数据会被移到链表的最前端。<br>
	* 这样使用率最低的数据会被存在链表的最末端，当缓冲区满时，最末端的数据将会被清除。
	*/
	template<typename F,typename T> class ActiveChain:public Object									///缓冲区管理模板(以最终使用时间为基准)
	{
		typedef ActiveChain<F,T> this_class;

	protected:

        struct Item
        {
            F flag;                 //识别标识

            T data;                 //数据

            Item    *prev,          //前一数据
                    *next;          //后一数据
        };

        using ACItem=ActiveChain<F,T>::Item;

		ACItem  *start_item,	    //首数据
				*end_item;		    //尾数据

		int count,max_count;

	protected:

		virtual bool Create(const F &,T &);															///<创建数据
		virtual void Clear(const F &,T &);															///<清除数据

				void MoveToStart(ACItem *);											                ///<移动某一个数据到最前面

				void Delete(ACItem *);												                ///<清除某一个数据

				void ClearEnd();																	///<清除最后一个数据

	public:

				const	int		GetCount	()const{return count;}									///<取得当前有多少数据
				const	int		GetMaxCount	()const{return max_count;}								///<取得最大可以有多少数据
		virtual 		void	SetMaxCount	(int);													///<设置最大可以有多少数据

	public:

		ActiveChain(int);
		virtual ~ActiveChain();

		virtual ACItem *        Add		(const F &,const T &);	  									///<增加一个数据
		virtual bool			Find	(const F &,T &,bool=true);									///<取得一个数据(如果没有不会自动创建)
		virtual bool 			Get		(const F &,T &,bool=true); 									///<取得一个数据(如果没有会自动创建)
				void 			Clear	();						  									///<清除所有数据

				void			DeleteByFlag(const F &);
				void 			DeleteByData(T &);
// 				bool			Update		(const F &,T &);										///<更新数据
	};//template<typename F,typename T> class ActiveChain
}//namespace hgl
#include<hgl/type/ActiveChain.cpp>
#endif//HGL_TYPE_ACTIVE_CHAIN_INCLUDE
