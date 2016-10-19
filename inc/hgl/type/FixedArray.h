#ifndef HGL_TYPE_FIXED_ARRAY_INCLUDE
#define HGL_TYPE_FIXED_ARRAY_INCLUDE

#include<hgl/TypeFunc.h>
namespace hgl
{
	/**
	 * 固定阵列数据模板
	 */
	template<typename T,size_t MAX_SIZE,T NULL_VALUE> class FixedArray
	{
	protected:

		T items[MAX_SIZE];
		size_t item_count=0;

	public:

		FixedArray()
		{
			ClearAll();
		}

		virtual ~FixedArray()
		{
			ClearAll();
		}

		const size_t GetCount()const
		{
			return item_count;
		}

		T operator[](int n)
		{
			if(n<0||n>=MAX_SIZE)return NULL_VALUE;

			return items[n];
		}

		const T operator[](int n)const
		{
			if(n<0||n>=MAX_SIZE)return NULL_VALUE;

			return items[n];
		}

		virtual bool Set(int n,T &data)
		{
			if(n<0||n>MAX_SIZE)return(false);

			if(data==NULL_VALUE)return(false);

			if(items[n]!=NULL_VALUE)
			{
				if(items[n]==data)
					return(true);

				return(false);
			}

			items[n]=data;
			++item_count;
			return(true);
		}

		virtual bool Clear(int n)
		{
			if(n<0||n>MAX_SIZE)return(false);

			if(items[n]==NULL_VALUE)return(true);

			items[n]=NULL_VALUE;
			--item_count;
			return(true);
		}

		virtual void ClearAll()
		{
			for(size_t i=0;i<MAX_SIZE;i++)
				items[i]=NULL_VALUE;

			item_count=0;
		}
	};//class FixedArray

	/**
	 * 固定对象数据阵列模板
	 */
	template<typename T,size_t MAX_SIZE> class ObjectFixedArray:public FixedArray<T *,MAX_SIZE,nullptr>
	{
	public:

		~ObjectFixedArray() override
		{
			ClearAll();
		}

		bool Clear(int n) override
		{
			if(n<0||n>MAX_SIZE)return(false);

			if(!this->items[n])return(true);

			delete this->items[n];

			this->items[n]=nullptr;
			--this->item_count;
			return(true);
		}

		void ClearAll() override
		{
			for(size_t i=0;i<MAX_SIZE;i++)
			{
				if(this->items[i])delete this->items[i];
				this->items[i]=nullptr;
			}

			this->item_count=0;
		}
	};//class ObjectFixedArray
}//namespace hgl
#endif//HGL_TYPE_FIXED_ARRAY_INCLUDE
