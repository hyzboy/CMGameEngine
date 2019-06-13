#include<hgl/gui/ObjectControl.h>

namespace hgl
{
    namespace graph
    {
        bool ObjectControl::Proc_CursorPos(int x,int y)
        {
            const bool result=GUIObject::Proc_CursorPos(x,y);           //必须先通过这个刷新mouse_focus

            if(active_obj)
                if(active_obj->topmost)
                {
                    if(active_obj->Proc_CursorPos(x,y))return(true);

                    int n=clas_object[ocProcEvent].GetCount();

                    while(n--)
                        if(((EventObject *)clas_object[ocProcEvent][n])->Proc_CursorPos(x,y))return(true);

                    return(result);
                }

            //先跑topmost的对象
            {
                int n=clas_object[ocProcEvent].GetCount();

                while(n--)
                {
                    EventObject *obj=(EventObject *)(clas_object[ocProcEvent][n]);

                    if(obj->topmost==true)
                    {
                        if(obj->Proc_CursorPos(x,y))
                            return(true);
                    }
                }
            }

            //活动对象
            {
                if(active_obj)
                    if(active_obj->Proc_CursorPos(x,y))return(true);
            }

            //其它对象
            {
                int n=clas_object[ocProcEvent].GetCount();

                while(n--)
                {
                    EventObject *obj=(EventObject *)(clas_object[ocProcEvent][n]);

                    if(obj->topmost==false)
                    {
                        if(obj->Proc_CursorPos(x,y))
                            return(true);
                    }
                }
            }

            return(result);
        }

        bool ObjectControl::Proc_MouseButton(int key,bool push)
        {
            const bool result=GUIObject::Proc_MouseButton(key,push);

            //活动对像
            if(active_obj)
                if(active_obj->topmost)
                {
                    if(active_obj->Proc_MouseButton(key,push))return(true);

                    int n=clas_object[ocProcEvent].GetCount();

                    while(n--)
                        if(((EventObject *)clas_object[ocProcEvent][n])->Proc_MouseButton(key,push))return(true);

                    return(result);
                }

            //顶层对象
            int n=clas_object[ocProcEvent].GetCount();

            while(n--)
            {
                Object *obj=clas_object[ocProcEvent][n];

                if(obj->is_visual())
                    if(((VisualObject *)obj)->topmost)
                        if(((EventObject *)obj)->Proc_MouseButton(key,push))return(true);
            }

            if(active_obj)
                if(active_obj->Proc_MouseButton(key,push))return(true);

            //非顶层对象
            n=clas_object[ocProcEvent].GetCount();

            while(n--)
            {
                Object *obj=clas_object[ocProcEvent][n];

                if(obj->is_visual())
                    if(!((VisualObject *)obj)->topmost)
                        if(((EventObject *)obj)->Proc_MouseButton(key,push))return(true);
            }

            return(result);
        }

        bool ObjectControl::Proc_Scroll(int x,int y)
        {
            const bool result=GUIObject::Proc_Scroll(x,y);

            if(active_obj)
                if(((EventObject *)active_obj)->Proc_Scroll(x,y))return(true);

            int n=clas_object[ocProcEvent].GetCount();

            while(n--)
                if(((EventObject *)clas_object[ocProcEvent][n])->Proc_Scroll(x,y))return(true);

            return(result);
        }

        bool ObjectControl::Proc_JoystickButton(int key,bool push)
        {
            const bool result=GUIObject::Proc_JoystickButton(key,push);

            if(active_obj)
                if(((EventObject *)active_obj)->Proc_JoystickButton(key,push))return(true);

            //if(key==jbUp  ){return Proc_Key(kbUp      ,push);}else
            //if(key==jbDown    ){return Proc_Key(kbDown    ,push);}else
            //if(key==jbLeft    ){return Proc_Key(kbLeft    ,push);}else
            //if(key==jbRight   ){return Proc_Key(kbRight   ,push);}else
            //if(key==jbCircle){return Proc_Key(kbEnter ,push);}else
            //if(key==jbFork    ){return Proc_Key(kbEsc     ,push);}else
            //if(key==jbL1  ){return Proc_Key(kbHome    ,push);}else
            //if(key==jbR1  ){return Proc_Key(kbEnd     ,push);}else
            //if(key==jbL2  ){return Proc_Key(kbPageUp  ,push);}else
            //if(key==jbR2  ){return Proc_Key(kbPageDown,push);}else

            return(result);
        }

        bool ObjectControl::Proc_Key(int key,bool push)
        {
            const bool result=GUIObject::Proc_Key(key,push);

            if(active_obj)
                if(((EventObject *)active_obj)->Proc_Key(key,push))return(true);

            if(!push)return(result);

            if(clas_object[ocKeyFocus].GetCount())
            {
                if(key==kbUp    ){KeyFocusUpMove();     return(true);}else
                if(key==kbLeft  ){KeyFocusLeftMove();   return(true);}else
                if(key==kbDown  ){KeyFocusDownMove();   return(true);}else
                if(key==kbRight ){KeyFocusRightMove();  return(true);}else
                    return(result);
            }

            return(result);
        }
        bool ObjectControl::Proc_Char(u16char ch)
        {
            const bool result=GUIObject::Proc_Char(ch);

            if(active_obj)
                return(((EventObject *)active_obj)->Proc_Char(ch));

            return(result);
        }

        bool ObjectControl::Proc_Event(int type,void *data)
        {
            const bool result=GUIObject::Proc_Event(type,data);

            if(active_obj)
                if(((EventObject *)active_obj)->Proc_Event(type,data))return(true);

            int n=clas_object[ocProcEvent].GetCount();

            while(n--)
                if(((EventObject *)clas_object[ocProcEvent][n])->Proc_Event(type,data))return(true);

            return(result);
        }
    }//namespace graph
}//namespace hgl
