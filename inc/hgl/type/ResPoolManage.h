#ifndef HGL_RES_POOL_MANAGE_INCLUDE
#define HGL_RES_POOL_MANAGE_INCLUDE

#include<hgl/type/Pool.h>
#include<hgl/type/ResManage.h>

namespace hgl
{
	/**
	 * 资源池是Pool/ResManage两个模板的组合应用
	 */
	template <typename I,typename T> class ResPoolManage:public ResManage<I,T>
	{
	protected:

		ObjectPool<T> data_pool;

	public:

		virtual T *Create(const I &flag){return data_pool.Acquire();}
		virtual void Clear(T *obj){data_pool.Release(obj);}

	public:

		virtual ~ResPoolManage()=default;

		virtual void Clear()
		{
			this->items.Clear();
			data_pool.ClearAll();
		}
	};//template <typename I,typename T> class ResPoolManage<I,T>:public ResManage<I,T>
}//namespace hgl
#endif//HGL_RES_POOL_MANAGE_INCLUDE
