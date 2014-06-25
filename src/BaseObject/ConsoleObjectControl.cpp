#include<hgl/object/ConsoleObjectControl.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	#define iabs(a)		((a)>0?(a):-(a))

	ConsoleObjectControl::ConsoleObjectControl()
	{
		Name=u"ConsoleObjectControl";

		object_attrib|=oaControl;
	}

	ConsoleObjectControl::~ConsoleObjectControl()
	{
		Clear();
	}

	/**
	* 清除当前控制器中的所有对象
	*/
	void ConsoleObjectControl::Clear()
	{
		int n=all_object.GetCount();

		destroy_obj.UnlinkAll();            //解除所有的关联

		while(n--)
			all_object[n]->SuperObject=nullptr;

		SuperClass::Clear();
	}

	/**
	* 让指定对象和当前对象控制器断开关联
	* @param vo 要断开的对象
	* @return 是否成功
	*/
	bool ConsoleObjectControl::Unlink(Object *obj)
	{
		if(all_object.Find(obj)==-1)
		{
			//有已被删除的对象存在，是已断开的，可能被再次断开，所以是正确的

//			DebugLog(u"将不存在的对象和控制器断开！");
			return(false);
		}

		obj->SuperObject=nullptr;
		SuperClass::Unlink(obj);

		return(true);
	}

	/**
	* 解除所有对象关联
	*/
	void ConsoleObjectControl::UnlinkAll()
	{
		int n=all_object.GetCount();

		destroy_obj.UnlinkAll();            //解除所有的关联

		while(n--)
			all_object[n]->SuperObject=nullptr;

		SuperClass::UnlinkAll();
	}

	/**
	* 将指定对象从当前控制器删除
	* @param vo 要删除的对象
	* @return 是否成功
	*/
	bool ConsoleObjectControl::Delete(Object *vo)
	{
		if(vo)
			if(Unlink(vo))
			{
				delete vo;

				return(true);
			}

		return(false);
	}

	void ConsoleObjectControl::Add(Object *obj)
	{
		if(!obj)
		{
			#ifdef _DEBUG
				LOG_ERROR(u"添加了一个空的对象到控制器中！");
			#endif//_DEBUG

			return;
		}

		SuperClass::Add(obj);

		obj->SuperObject=this;				// Form 会在设置 SuperObject时设置自己为活动对象
                                        	// 所以必须先将Form添加到ClasList后再设置SuperObject
	}

	/**
	* 对象控制器对应的刷新函数
	*/
	void ConsoleObjectControl::Update()
	{
		if(destroy_obj.GetCount())
		{
			destroy_obj.Clear();        //清除被标记为要清除的对象
		}

		{
			int n=clas_object[ocUpdate].GetCount();

			while(n--)
			{
				#ifdef _DEBUG
					EnumObject *obj=(EnumObject *)(clas_object[ocUpdate][n]);

					obj->Update();
				#else
					((EnumObject *)(clas_object[ocUpdate][n]))->Update();
				#endif//_DEBUG
            }
		}
	}

	Object *ConsoleObjectControl::FindObject(const char16_t *obj_name)
	{
		int n=clas_object[ocName].GetCount();

		//在本级查找,只有有名称的对象才会被加入到ocName的类别中
		while(n--)
			if(clas_object[ocName][n]->Name==obj_name)
				return(clas_object[ocName][n]);

		//在子级查找
		{
			Object *result_object;

			n=clas_object[ocControl].GetCount();
			while(n--)
			{
				result_object=((ConsoleObjectControl *)(clas_object[ocControl][n]))->FindObject(obj_name);

				if(result_object)
					return(result_object);
			}
		}

		return(nullptr);
	}

	bool ConsoleObjectControl::FindObject(Object *obj)
	{
		return(all_object.Find(obj)!=-1);
    }

	void ConsoleObjectControl::SetEnabledAttrib(Object *obj,bool e)
	{
		if(!e)
		{
			clas_object[ocUpdate	].DeleteByData(obj);    //不再刷新
		}
		else
		{
			#ifdef _DEBUG
			if(all_object.Find(obj)==-1)
			{
				LOG_ERROR(u"一个被要求设置为显示属性的对象并不在这个对象控制器里");
			}
			else
			#endif//_DEBUG
			{
				if(obj->is_update()    	)clas_object[ocUpdate   ].Add(obj);
			}
		}
	}

	void ConsoleObjectControl::SetUpdateAttrib(EnumObject *obj,bool e)
	{
		if(!obj)return;

		if(all_object.Find(obj)==-1)
		{
			LOG_ERROR(u"一个被要求设置刷新属性的对象并不在这个对象控制器里");
		}
		else
		{
			if(e)
				clas_object[ocUpdate].Add(obj);
			else
				clas_object[ocUpdate].DeleteByData(obj);
		}
	}

	void ConsoleObjectControl::SetDestroyAttrib(Object *obj)
	{
		if(!obj)return;

		if(destroy_obj.Find(obj)==-1)
			destroy_obj.Add(obj);
	}
}//namespace hgl
