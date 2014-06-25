#ifndef HGL_AFFECTOR_OBJECT_INCLUDE
#define HGL_AFFECTOR_OBJECT_INCLUDE

#include<hgl/object/EnumObject.h>
namespace hgl
{
	class TDFNode;
	class AffectorControl;

	/**
	* 影响器对像
	*/
	class AffectorObject:public EnumObject															///影响器对像
	{
	protected:

		AffectorControl *control;																	///<控制器

	public:	//方法

		AffectorObject(){control=nullptr;}															///<本类构造函数
		virtual ~AffectorObject()HGL_DEFAULT_MEMFUNC;															///<本类析构函数

		virtual void SetControl(AffectorControl *ac){control=ac;}									///<设置控制器

		virtual const char16_t *GetClassName(){return NULL;}											///<取得类名

		virtual bool Start(){return(true);}															///<开始
		virtual bool Pause(){return(true);}															///<暂停
		virtual bool Resume(){return(true);}														///<恢复
		virtual bool Stop(){return(true);}															///<停止

		virtual void Update()HGL_DEFAULT_MEMFUNC;																///<刷新

	public:		//保存方法

		//virtual bool SaveToTDF(TDFNode *);															///<将当前影响器保存到TDF节点
		//virtual bool LoadFromTDF(TDFNode *);														///<从TDF节点中加载当前影响器
	};//class AffectorObject
}//namespace hgl
#endif//HGL_AFFECTOR_OBJECT_INCLUDE
