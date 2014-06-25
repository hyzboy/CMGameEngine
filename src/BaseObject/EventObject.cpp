#include<hgl/object/EventObject.h>
// #include<hgl/object/ObjectControl.h>

namespace hgl
{
	namespace graph
	{
		EventObject::EventObject()
		{
			object_attrib|=oaProcEvent;
			active=false;

			OnEvent=nullptr;
			OnActive=nullptr;

			hglSetProperty(Active,this,EventObject::GetActive,EventObject::SetActive);
		}

		void EventObject::SetActive(bool a)
		{
			if(active==a)return;

			active=a;

			SafeCallEvent(OnActive,(this,active));

// 			if(super_object)
// 				super_object->SetActiveAttrib(this,active);
		}

		bool EventObject::Proc_Event(int type,void *data)
		{
			SafeCallEvent(OnEvent,(type,data));
			return(false);
		}

		void EventObject::Destroy()
		{
			Active=false;

			VisualObject::Destroy();
		}
	}//namespace graph
}//namespace hgl
