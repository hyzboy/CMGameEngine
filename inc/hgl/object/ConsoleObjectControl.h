#ifndef HGL_CONSOLE_OBJECT_CONTROL_INCLUDE
#define HGL_CONSOLE_OBJECT_CONTROL_INCLUDE

#include<hgl/object/EnumObject.h>
#include<hgl/type/Stack.h>
#include<hgl/type/ClasList.h>
namespace hgl
{
	/**
	* 对象控制器，这是一个最基本的对象控制器，可以添加若干个对象到里面。然后由它控制显示、刷新、清除。它也可以被继承。
	*/
	class ConsoleObjectControl:public ClasList<Object,Object::ObjectClas::ocEnd>,public EnumObject	///对象控制器
	{
	private:

		typedef ClasList<Object,Object::ObjectClas::ocEnd> SuperClass;

		bool CheckObjectAttrib(Object *,int);

	protected:

		ObjectList<Object> destroy_obj;                                                             ///<要销毁的对象

	public: //设置对象属性(内部方法，请不要调用)

		virtual void SetEnabledAttrib(Object *,bool);
		virtual void SetUpdateAttrib(EnumObject *,bool);
		virtual void SetDestroyAttrib(Object *);

	public: //方法

		ConsoleObjectControl();
		virtual ~ConsoleObjectControl();

        virtual void Add(Object *);

		virtual void Clear();
		virtual bool Unlink(Object *);
		virtual bool Delete(Object *);

		virtual void UnlinkAll();
		virtual void DeleteAll(){Clear();}

		virtual void operator += (Object *obj){Add(obj);}											///<向控制器中添加一个对象
		virtual void operator -= (Object *obj){Unlink(obj);}										///<从控制器中移除一个对象

		virtual void Update();                                                                      ///<刷新

	public: //方法 查找

		virtual Object *FindObject(const u16char *);                                                ///<查找对象
		virtual bool FindObject(Object *);															///<查找对象
	};//class ConsoleObjectControl
}//namespace hgl
#endif//HGL_CONSOLE_OBJECT_CONTROL_INCLUDE
