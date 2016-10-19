#ifndef HGL_CLASLIST_INCLUDE
#define HGL_CLASLIST_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
	/**
	* 分类对象模板<br>
	* 允许将一批数据分成几类保存，而且允许一个数据在好几个类别的列表中均存在。
	*/
	template<typename T,int TypeCount> class ClasList												///分类对象模板
	{
	protected:

		ObjectList<T> all_object;                                                                   ///<全部对象列表
		List<T *> clas_object[TypeCount];                                                           ///<分类对象列表

	public:

		ClasList()=default;
		virtual ~ClasList()=default;

		virtual void Add(T *);																		///<添加一个对象到当前控制器器
		virtual void Clear();                                                                       ///<清除所有对象

		virtual bool Unlink(T *);                                                                   ///<将指定对象与当前控制器断开关联
		virtual bool Delete(T *);                                                                   ///<将指定对象从当前控制器删除

		virtual void UnlinkAll();																	///<解除所有对象关联
				void DeleteAll(){Clear();}															///<删除所有对象

				void operator += (T *obj){Add(obj);}                                                ///<添加一个对象到当前控制器器
				void operator << (T *obj){Add(obj);}

		List<T *> &operator[](int index){return clas_object[index];}								///<按分类使用列表
	};//template<typename T,int TypeCount> class ClasList
}//namespace hgl
#include<hgl/type/ClasList.cpp>
#endif//HGL_CLASLIST_INCLUDE
