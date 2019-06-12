#include<hgl/ut/ShortCut.h>
#include<hgl/InputState.h>
namespace hgl
{
    /**
    * 本类构造函数
    */
    ShortCut::ShortCut()
    {
        Ctrl    =false;
        Alt     =false;
        Shift   =false;

        Key     =kb_NONE;

        OnShortCut=nullptr;
    }

    /**
    * 本类构造函数
    * @param func 快捷键事件处理函数
    * @param _key 按键
    * @param _ctrl Ctrl按键是否要处于按下状态
    * @param _alt Alt按键是否要处于按下状态
    * @param _shift Shift按键是否要处于按下状态
    */
    ShortCut::ShortCut(ShortCut::OnShortEvent func,KeyButton key,bool ctrl,bool alt,bool shift)
    {
        Ctrl    =ctrl;
        Alt     =alt;
        Shift   =shift;

        Key=    key;

        OnShortCut=func;
    }

    void ShortCut::Update()
    {
        if(OnShortCut==nullptr)return;
        if(Key<=kb_NONE||Key>=kb_END)return;

        if(GetKeyState(Key)&ksDown)     //被按下的瞬间才起效
        {
            if(Alt)         //如果Alt被要求处于按下状态
            {
                if(!(GetKeyState(kbLeftAlt  )&ksPress)
                 ||!(GetKeyState(kbRightAlt )&ksPress))return;
            }
            else
            {
                if((GetKeyState(kbLeftAlt    )&ksPress)
                 ||(GetKeyState(kbRightAlt   )&ksPress))return;
            }

            if(Ctrl)
            {
                if(!(GetKeyState(kbLeftCtrl )&ksPress)
                 ||!(GetKeyState(kbRightCtrl)&ksPress))return;
            }
            else
            {
                if((GetKeyState(kbLeftCtrl )&ksPress)
                 ||(GetKeyState(kbRightCtrl)&ksPress))return;
            }

            if(Shift)
            {
                if(!(GetKeyState(kbLeftShift    )&ksPress)
                 ||!(GetKeyState(kbRightShift   )&ksPress))return;
            }
            else
            {
                if((GetKeyState(kbLeftShift    )&ksPress)
                 ||(GetKeyState(kbRightShift   )&ksPress))return;
            }

            SafeCallEvent(OnShortCut,(this));
        }
    }
}//namespace hgl
