#ifndef HGL_CONSOLE_OBJECT_CONTROL_INCLUDE
#define HGL_CONSOLE_OBJECT_CONTROL_INCLUDE

#include<hgl/object/Object.h>
#include<hgl/object/ObjectAttrib.h>
#include<hgl/type/ClasList.h>
namespace hgl
{
    class ConsoleObjectClasList:public ClasList<Object,ObjectClas::ocEnd>
    {
    public:

        bool CheckObjectAttrib(Object *,int type) override
        {
        }
    };//class ConsoleObjectClasList


	/**
	* 对象控制器，这是一个最基本的对象控制器，可以添加若干个对象到里面。然后由它控制显示、刷新、清除。它也可以被继承。
	*/
	class ConsoleObjectControl:public Object     ///对象控制器
	{
	private:

        ConsoleObjectClasList clas;

	protected:

		ObjectList<Object> destroy_obj;                                                             ///<要销毁的对象

	public: //设置对象属性(内部方法，请不要调用)

		virtual void SetEnabledAttrib(Object *,bool);
		virtual void SetUpdateAttrib(Object *,bool);
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
		virtual bool IsMember(Object *obj)const{all_object.IsExist(obj);}							///<确认对象是否存在管理器中
	};//class ConsoleObjectControl
}//namespace hgl
#endif//HGL_CONSOLE_OBJECT_CONTROL_INCLUDE
