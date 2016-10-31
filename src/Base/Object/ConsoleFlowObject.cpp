#include"hgl/object/ConsoleFlowObject.h"

namespace hgl
{
	/**
	* 本类构造函数
	*/
	ConsoleFlowObject::ConsoleFlowObject()
	{
		fos=fosNotInit;

		NextObject		=nullptr;
	}

	/**
	* 流程对像基类析构函数<br>
	* NextObject如果没有被清除，在这里会被清除
	*/
	ConsoleFlowObject::~ConsoleFlowObject()
	{
		delete NextObject;
	}

	/**
	* 取得下一个对象。如果不重载此函数，它将返回当前对象中的NextObject变量
	*
	* 注意：下一个对象可以不是流程对象(ConsoleFlowObject)，但非流程对象不能进行流程操作，即不能流程控制器清除自己进入下一流程
	*/
	ConsoleFlowObject *ConsoleFlowObject::GetNextObject()
	{
		if(NextObject)
		{
			ConsoleFlowObject *result=NextObject;

			NextObject=nullptr;

			return(result);
		}

       	return(nullptr);
	}
}//namespace hgl
