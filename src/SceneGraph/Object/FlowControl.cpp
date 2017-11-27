#include<hgl/object/FlowControl.h>

namespace hgl
{
    void FlowControl::OnClose()
    {
        if(active_obj)
            active_obj->OnClose();

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnClose();
            ++p;
        }
    }

    void FlowControl::OnResize      (int w,int h)
    {
        if(active_obj)
            active_obj->OnResize(w,h);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnResize(w,h);
            ++p;
        }
    }

    void FlowControl::OnRotate      (int ang)
    {
        if(active_obj)
            active_obj->OnRotate(ang);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnRotate(ang);
            ++p;
        }
    }

    bool FlowControl::OnMouseMove   (int x,int y)
    {
        if(active_obj)
            active_obj->OnMouseMove(x,y);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnMouseMove(x,y);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnMouseWheel  (double x,double y)
    {
        if(active_obj)
            active_obj->OnMouseWheel(x,y);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnMouseWheel(x,y);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnMouseDown   (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseDown(mb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnMouseDown(mb);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnMouseUp     (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseUp(mb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnMouseUp(mb);
            ++p;
        }
        return(false);
    }

    bool FlowControl::OnMouseRepeat (MouseButton mb)
    {
        if(active_obj)
            active_obj->OnMouseRepeat(mb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnMouseRepeat(mb);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnKeyDown     (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyDown(kb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnKeyDown(kb);
            ++p;
        }
        return(false);
    }

    bool FlowControl::OnKeyUp       (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyUp(kb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnKeyUp(kb);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnKeyRepeat   (KeyboardButton kb)
    {
        if(active_obj)
            active_obj->OnKeyRepeat(kb);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnKeyRepeat(kb);
            ++p;
        }

        return(false);
    }

    bool FlowControl::OnChar        (os_char ch)
    {
        if(active_obj)
            active_obj->OnChar(ch);

        FlowObject **p=objstack.GetData();
        for(int i=0;i<objstack.GetCount();i++)
        {
            (*p)->OnChar(ch);
            ++p;
        }

        return(false);
    }

    void FlowControl::ChangeActiveObject(FlowObject *obj)
    {
        if(active_obj)
            active_obj->UnjoinControl(this);

        _FlowControl<FlowObject>::ChangeActiveObject(obj);

        if(active_obj)
            active_obj->JoinControl(this);
    }

    /**
    * 调用当前控制器中的对象绘制画面
    */
    void FlowControl::Draw(const Matrix4f *mv)
    {
        if(!this->active_obj)return;

        if(!this->active_obj->CanDraw())return;

        {
            int n,count=this->objstack.GetCount();
            FlowObject **p=objstack.GetData();

            for(n=0;n<count;n++)
            {
                if((*p)->CanDraw())
                    (*p)->Draw(mv);

                ++p;
            }
        }

        this->active_obj->Draw(mv);
    }
}//namespace hgl
