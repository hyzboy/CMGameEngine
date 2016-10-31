#ifndef HGL_OBJECT_INCLUDE
#define HGL_OBJECT_INCLUDE

#include<hgl/object/_Object.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	/**
	* Object类是HGL类库中所有游戏对象类的基类,它提供对象该具备的所有基本要素。
	*/
	class Object:public _Object                                                                     ///对象基类
	{
    protected:  //私有属性

        bool enabled;                                                                               ///<是否可以刷新
        bool visible;                                                                               ///<是否可以显示
        bool give_event;                                                                            ///<是否接收事件

	public:	//属性

		UTF16String      				Name;                                                       ///<对象名称

	public:

        Object(){}                                                                                  ///<本类构造函数
        Object(const UTF16String &n){Name=n;}                                                       ///<本类构造函数
        virtual ~Object(){}                                                                         ///<本类析构函数

        virtual void SetEnabled(bool e){enabled=e;}
        virtual void SetVisible(bool v){visible=v;}
        virtual void SetGiveEvnet(bool ge){give_event=ge;}

        virtual const bool IsEnabled()const{return enabled;}
        virtual const bool IsVisible()const{return visible;}
        virtual const bool IsGetEvent()const{return give_event;}

        virtual void Update(){enabled=false;}
        virtual void Draw(){visible=false;}
	};//class Object
}//namespace hgl
#endif//HGL_OBJECT_INCLUDE
