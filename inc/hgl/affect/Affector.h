#ifndef HGL_AFFECTOR_INCLUDE
#define HGL_AFFECTOR_INCLUDE

#include<hgl/affect/AffectorObject.h>
#include<hgl/type/Map.h>
namespace hgl
{
	/**
	* “影响器对像”控制器
	*/
	class AffectorControl:public AffectorObject														///“影响器对像”控制器
	{
		bool pause;
		bool stop;

		bool _Start();
		bool _Pause();
		bool _Resume();
		bool _Stop();
		bool _Update();

		bool GetPause(){return pause;}
		bool GetStop(){return stop;}
		bool GetRun(){return (pause||stop)?false:true;}

	protected:	//属性

		MapObject<UTF16String,AffectorObject> affector_list;										///<影响器列表
		ObjectList<AffectorObject> delete_list;														///<待删除列表

	public:		//公开属性

		Property<bool> IsPause;																		///<是否暂停状态
		Property<bool> IsStop;																		///<是否停止状态
		Property<bool> IsRun;																		///<是否运行状态

	public:		//方法

		AffectorControl();																			///<本类构造函数
		virtual ~AffectorControl(){DeleteAll();}													///<本类析构函数

		virtual const char16_t *GetClassName(){return u"AffectorControl";}							///<取得类名

		virtual bool Add(const UTF16String &,AffectorObject *);										///<添加一个影响器到控制器中
				bool Add(AffectorObject *ao)														///<使用缺省名称添加一个影响器到控制器中
				{
					if(!ao)return(false);

					Add(ao->GetClassName(),ao);

					return(true);
				}

		virtual bool Unlink(const UTF16String &);													///<从控制器中移除一个影响器
		virtual bool Unlink(AffectorObject *);														///<从控制器中移除一个影响器
		virtual bool Delete(const UTF16String &);													///<从控制器中山删除一个影响器
		virtual bool Delete(AffectorObject *);														///<从控制器中移删除一个影响器
		virtual void UnlinkAll();																	///<解除所有影响器关联
		virtual void DeleteAll();																	///<删除所有影响器

				void Destroy(AffectorObject *);														///<销毁一个影响器,添加到待删除列表

		virtual AffectorObject *Get(const UTF16String &);											///<根据名称取得一个控制器

	public:		//方法

		virtual bool Start();																		///<开始
		virtual bool Pause();																		///<暂停
		virtual bool Resume();																		///<恢复
		virtual bool Stop();																		///<停止

		virtual void Update();																		///<刷新处理
	};//class AffectorControl
}//namespace hgl
#endif//HGL_AFFECTOR_INCLUDE
