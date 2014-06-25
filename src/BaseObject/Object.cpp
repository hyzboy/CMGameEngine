#include<hgl/object/Object.h>
// #include<hgl/object/ObjectControl.h>

namespace hgl
{
	void Object::InitPrivate()
	{
		hglSetProperty(		SuperObject,this,Object::GetSuperObject,	Object::SetSuperObject	);
		hglSetPropertyRead(	Attrib,		this,Object::GetObjectAttrib);
		hglSetProperty(		Enabled,	this,Object::GetEnabled,		Object::SetEnabled		);

		destroy=false;
		enabled=true;
		object_attrib=0;

		super_object=nullptr;

		OnLink  =nullptr;
		OnUnlink=nullptr;
	}

	Object::Object()
	{
		InitPrivate();
	}

	Object::Object(const UTF16String &name)
	{
		InitPrivate();
    	Name=name;
	}

	/**
	* 本类析构函数
	*/
	Object::~Object()
	{
// 		if(super_object&&(super_object->is_res()==false)&&(super_object->is_control()))            //不是资源控制器
// 			super_object->Unlink(this);
	}

	void Object::SetSuperObject(ObjectControl *so)
	{
// 		if(super_object)    //原来有连接
// 		{
// 			SafeCallEvent(OnUnlink,(super_object,this));
// 		}
//
		super_object=so;
//
// 		if(so)              //新的连接不是空的
// 		{
// 			SafeCallEvent(OnLink,(super_object,this));
// 		}
	}

	void Object::SetEnabled(bool e)
	{
		if(enabled==e)return;
			else enabled=e;

// 		if(super_object)
// 			super_object->SetEnabledAttrib(this,enabled);
	}

	void Object::Destroy()
	{
		if(destroy==false)
		{
			destroy=true;

// 			if(super_object)
// 				super_object->SetDestroyAttrib(this);
		}
	}
}//namespace hgl
