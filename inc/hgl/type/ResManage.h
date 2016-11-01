#ifndef HGL_RES_MANAGE_INCLUDE
#define HGL_RES_MANAGE_INCLUDE

#include<hgl/type/Map.h>
namespace hgl
{
	template<typename F,typename T> struct RefFlagData:public Pair<F,T *>
	{
		uint count;

	public:

		RefFlagData():Pair<F,T *>()
		{
			count=1;
		}
	};

	/**
	* 资源管理器,它没有缓冲管理，仅仅是管理数据，并保证不会被重复加载
	*/
	template<typename F,typename T> class ResManage
	{
	protected:

		typedef RefFlagData<F,T> ResItem;

		_Map<F,T *,ResItem> items;

		void ReleaseBySerial(int,bool);

	protected:

		virtual T *Create(const F &)=0;											///<资源创建虚拟函数无实现，请特例化实现
		virtual void Clear(T *obj){delete obj;}									///<资源释放虚拟函数(缺省为直接delete对象)

	public:

		virtual ~ResManage();

		virtual void		Clear();											///<清除所有数据
        virtual void        ClearZero();                                        ///<清除所有没有用到的数据

				const int	GetCount()const{return items.GetCount();}			///<取得数据数量

		virtual bool		Add(const F &,T *);									///<添加一个数据
		virtual T *			Find(const F &);									///<查找一个数据
		virtual T *			Get(const F &);										///<取得一个数据，如不存在则创建

		virtual void		Release(const F &,bool zero_clear=false);			///<释放一个数据
		virtual void		Release(T *,bool zero_clear=false);					///<释放一个数据
	};//template<typename F,typename T> class ResManage
}//namespace hgl
#include<hgl/type/ResManage.cpp>
#endif//HGL_RES_MANAGE_INCLUDE
