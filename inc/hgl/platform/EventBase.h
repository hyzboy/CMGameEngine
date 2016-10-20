#ifndef HGL_PLATFORM_EVENT_BASE_INCLUDE
#define HGL_PLATFORM_EVENT_BASE_INCLUDE

#include<hgl/platform/Platform.h>
#include<hgl/type/Set.h>
#include<hgl/type/Vertex2.h>
namespace hgl
{
    /**
     * 事件回呼收发器基模板
     */
    template<typename ...ARGS> class EventBaseCB
    {
    public:

        DefEvent(bool,OnProc,(ARGS...));

    public:

        virtual bool Proc(ARGS...args)
        {
            if(!OnProc)return(false);

            return OnProc(args...);
        }
    };//template<typename ...ARGS> class EventBaseCB

    /**
     * 事件收发器基模板类
     */
    template<typename ...ARGS> class EventBase:public EventBaseCB<ARGS...>
    {
    protected:

        Set<EventBase *> sub_list;

    public:

        virtual bool Proc(ARGS...args)
        {
            const int count=sub_list.GetCount();

            if(count<=0)return(false);

            EventBase **ev=sub_list.GetData();

            for(int i=0;i<count;i++)
            {
                if((*ev)->Proc(args...))return(true);
                ++ev;
            }

            return this->EventBaseCB<ARGS...>::Proc(args...);
        }

    public:

        void Join(EventBase<ARGS...> *sub_ev)
        {
            if(!sub_ev)return;

            sub_list.Add(sub_ev);
        }

        void Unjoin(EventBase<ARGS...> *sub_ev)
        {
            if(!sub_ev)return;

            sub_list.Delete(sub_ev);
        }
    };//template<typename ...ARGS> class EventBase

    #define HGL_EV_SUB_JOIN(name)   this->name.Join(&(ev->name));
    #define HGL_EV_SUB_UNJOIN(name) this->name.Unjoin(&(ev->name));

    /**
     * 根事件收发器
     */
    class RootEventBase
    {
    public:

        EventBase<int,int>          MouseMove;                                  ///<鼠标移动事件
        EventBase<int,bool>         MouseButton;                                ///<鼠标按键事件
        EventBase<int,int>          MouseWheel;                                 ///<鼠标滚轮事件
        EventBase<int,bool>         JoystickButton;                             ///<游戏杆按键事件
        EventBase<int,bool>         Key;                                        ///<键盘按键事件
        EventBase<os_char>          Char;                                       ///<字符事件
        EventBase<int,Vertex2i *>   Touch;                                      ///<触控点事件
        EventBase<int,void *,int>   Event;                                      ///<自定义事件

    public:

        virtual void Join(RootEventBase *ev)
        {
            if(!ev)return;

            HGL_EV_SUB_JOIN(MouseMove);
            HGL_EV_SUB_JOIN(MouseButton);
            HGL_EV_SUB_JOIN(MouseWheel);
            HGL_EV_SUB_JOIN(JoystickButton);
            HGL_EV_SUB_JOIN(Key);
            HGL_EV_SUB_JOIN(Char);
            HGL_EV_SUB_JOIN(Touch);
            HGL_EV_SUB_JOIN(Event);
        }

        virtual void Unjoin(RootEventBase *ev)
        {
            if(!ev)return;

            HGL_EV_SUB_UNJOIN(MouseMove);
            HGL_EV_SUB_UNJOIN(MouseButton);
            HGL_EV_SUB_UNJOIN(MouseWheel);
            HGL_EV_SUB_UNJOIN(JoystickButton);
            HGL_EV_SUB_UNJOIN(Key);
            HGL_EV_SUB_UNJOIN(Char);
            HGL_EV_SUB_UNJOIN(Touch);
            HGL_EV_SUB_UNJOIN(Event);
        }
    };//class RootEventBase

    /**
     * 应用程序事件收发器
     */
    class AppEventBase:public RootEventBase
    {
    public:

        EventBaseCB<>           Close;                      ///<应用关闭
        EventBaseCB<int>        EnterBack;                  ///<进入后台
        EventBaseCB<int>        LeaveBack;                  ///<离开后台
        EventBaseCB<int>        EnterSleep;                 ///<进入休眠状态
        EventBaseCB<int>        LeaveSleep;                 ///<离开休眠状态

        EventBaseCB<int,int>    Resize;                     ///<窗口尺寸变动
    };//class AppEventBase:public RootEventBase

    /**
     * UI事件收发器
     */
    class UIEventBase:public RootEventBase
    {
    public:

        EventBase<int,int>  MouseEnter;                     ///<鼠标光标进入事件
        EventBase<int,int>  MouseLeave;                     ///<鼠标光标离开事件
        EventBase<int,int>  MouseDoubleClick;               ///<鼠标按键双击事件

        EventBase<>         GetFocus;                       ///<控件得到焦点
        EventBase<>         LoseFocus;                      ///<控件失去焦点

    public:

        virtual void Join(UIEventBase *ev)
        {
            if(!ev)return;

            this->RootEventBase::Join(ev);

            HGL_EV_SUB_JOIN(MouseEnter);
            HGL_EV_SUB_JOIN(MouseLeave);
            HGL_EV_SUB_JOIN(MouseDoubleClick);

            HGL_EV_SUB_JOIN(GetFocus);
            HGL_EV_SUB_JOIN(LoseFocus);
        }

        virtual void Unjoin(UIEventBase *ev)
        {
            if(!ev)return;

            this->RootEventBase::Unjoin(ev);

            HGL_EV_SUB_UNJOIN(MouseEnter);
            HGL_EV_SUB_UNJOIN(MouseLeave);
            HGL_EV_SUB_UNJOIN(MouseDoubleClick);

            HGL_EV_SUB_UNJOIN(GetFocus);
            HGL_EV_SUB_UNJOIN(LoseFocus);
        }
    };//class UIEventBase:public RootEventBase

    #undef HGL_EV_SUB_JOIN
    #undef HGL_EV_SUB_UNJOIN
}//namespace hgl
#endif//HGL_PLATFORM_EVENT_BASE_INCLUDE
