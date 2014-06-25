#ifndef HGL_RING_LIST_INCLUDE
#define HGL_RING_LIST_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	/**
	* 环形链表用于保存需要不断添加及自动释放的数据<br>
	* 此模板类仍测试中，请谨慎使用。
	*/
	template <typename T> class RingList                                                          ///环形链表类
	{
		int max_count;
		int rpos,wpos;

		T *item;

	protected:

    	int GetMax(){return max_count;}
		void SetMax(int);

		void SetItem(int,T &);
		T &GetItem(int);

	public:	//属性

		Property<int> MaxCount;                                                                     ///<环形链接数据数量虚拟变量

	public:

		RingList(int);
		virtual ~RingList();

		void Clear();                                                                               ///<清除所有数据

		bool Push(const T &);                                                                       ///<压入一个数据
		bool Pop(T &);                                                                              ///<取出一个数据
		bool Peek(T &);                                                                             ///<访问一个数据，但不移动读取指针

		T & operator[](int n){return GetItem(n);}
	};//template <typename T> class RingList
}//namespace hgl
#include <hgl/type/RingList.cpp>
#endif//HGL_RING_LIST_INCLUDE
