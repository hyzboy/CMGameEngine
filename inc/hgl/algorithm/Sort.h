#ifndef HGL_ALGORITHM_SORT_INCLUDE
#define HGL_ALGORITHM_SORT_INCLUDE

#include<hgl/TypeFunc.h>
#include<string.h>
namespace hgl
{
	template<typename T> class SortBase
	{
	protected:

		T *buffer;										//数据
		int number;										//数据个数

		Comparator<T> *comp;							//比较函数类

	public:

		/**
		 * 本类构造函数
		 * @param buf 数据缓冲区
		 * @param n 数据个数
		 * @param c 数据大小比较类
		 */
		SortBase(T *buf,int n,Comparator<T> *c)
		{
			buffer	=buf;
			number	=n;
			comp	=c;
		}

		virtual ~SortBase()=default;

				int GetCount()const
		{
			return number;
		}

				int compare(const T &a,const T &b)
		{
			return comp->compare(a,b);
		}

		virtual	int compare_by_index(int a,int b)
		{
			return comp->compare(buffer[a],buffer[b]);
		}

		virtual	void exchange(T &a,T &b)
		{
			comp->exchange(a,b);
		}

		virtual	void exchane_by_index(int a,int b)					//交换两个数据
		{
			comp->exchange(buffer[a],buffer[b]);
		}

		virtual void cpy(T *dst,T *src)
		{
			comp->cpy(dst,src);
		}

		virtual void cpy_by_index(int dst,int src)
		{
			comp->cpy(buffer+dst,buffer+src);
		}

		virtual bool sort()=0;								//排序
	};//struct SortBase

	//堆排序
	template<typename T> class HeapSort:public SortBase<T>
	{
		void isift(int i,int n)
		{
			int j;
			T temp;

			SortBase<T>::cpy(&temp,SortBase<T>::buffer+i);

			j=2*(i+1)-1;

			while(j<=n)
			{
				if((j<n)&&(SortBase<T>::compare_by_index(j,j+1)<0))j++;

				if(SortBase<T>::compare(temp,SortBase<T>::buffer[j])<0)
				{
					SortBase<T>::cpy_by_index(i,j);
					i=j;
					j=2*(i+1)-1;
				}
				else j=n+1;
			}

			SortBase<T>::cpy(SortBase<T>::buffer+i,&temp);
		}

	public:

		/**
		 * 本类构造函数
		 * @param buf 数据缓冲区
		 * @param n 数据个数
		 * @param c 数据大小比较类
		 */
		HeapSort(T *buf,int n,Comparator<T> *c=new Comparator<T>()):SortBase<T>(buf,n,c)
		{
		}

		bool sort()
		{
			if(!SortBase<T>::buffer||SortBase<T>::number<=2||!SortBase<T>::comp)
				return(false);

			int i;
			int mm=SortBase<T>::number>>1;

			for(i=mm-1;i>=0;i--)
				isift(i,SortBase<T>::number-1);

			for(i=SortBase<T>::number-1;i>=1;i--)
			{
				SortBase<T>::exchane_by_index(0,i);

				isift(0,i-1);
			}

			return(true);
		}
	};//class HeapSort:public SortBase<T>

	template<typename T>
	bool Sort(T *data,int count,Comparator<T> *comp=new Comparator<T>())
	{
		HeapSort<T> hs(data,count,comp);

		return hs.sort();
	}

	template<typename T>
	bool Sort(List<T> &list,Comparator<T> *comp=Comparator<T>())
	{
		return Sort(list.GetData(),
					list.GetCount(),
					comp);
	}

/*
	//仅实现模拟虚拟成员函数即可，无需整个类重载
	template<> int Comparator<BagCell>::compare(const BagCell &it1,const BagCell &it2) const
	{
		int r=it1.GetItemID()-it2.GetItemID();

		if(r!=0)
			return r;

		return it1.count-it2.count;
	}

	void BagManage::Sort()
	{
		Comparator<BagCell> comp_baginfo;

		BagCell cell_list[BAG_SLOT_COUNT];

		hgl::Sort<BagCell>(cell_list,BAG_SLOT_COUNT,&comp_baginfo);
	}
 */
}//namespace hgl
#endif//HGL_ALGORITHM_SORT_INCLUDE
