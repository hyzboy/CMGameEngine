#include<hgl/object/EnumObject.h>
// #include<hgl/object/ObjectControl.h>
namespace hgl
{
	EnumObject::EnumObject(bool e)
	{
		if(e)
			object_attrib|=oaUpdate;
	}

	/**
	* 对象刷新函数。如果对象没有重载这个函数，即被视为放弃刷新属性，将不会存在于对象控制器的刷新列表里
	*/
	void EnumObject::Update()
	{
		object_attrib=object_attrib&(~oaUpdate);

// 		if(super_object)
// 			super_object->SetUpdateAttrib(this,false);
	}
}//namespace hgl
