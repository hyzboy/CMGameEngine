#include<hgl/ut/CtrlRect.h>
#include<hgl/gl/OpenGL.h>

namespace hgl
{
    namespace ctrl
    {
        CtrlRect::CtrlRect(float l,float t,float w,float h)
        {
            in=false;
            select=false;

            Scope.Left=l;
            Scope.Top=t;
            Scope.Width=w;
            Scope.Height=h;

            {
                operator+=(cp[0]=new CtrlPoint2D(l        ,t          ,8));
                operator+=(cp[1]=new CtrlPoint2D(l+w/2.0f ,t          ,8));
                operator+=(cp[2]=new CtrlPoint2D(l+w      ,t          ,8));

                operator+=(cp[3]=new CtrlPoint2D(l        ,t+h/2.0f   ,8));
                operator+=(cp[4]=new CtrlPoint2D(l+w/2.0f ,t+h/2.0f   ,8));
                operator+=(cp[5]=new CtrlPoint2D(l+w      ,t+h/2.0f   ,8));

                operator+=(cp[6]=new CtrlPoint2D(l        ,t+h      ,8));
                operator+=(cp[7]=new CtrlPoint2D(l+w/2.0f ,t+h      ,8));
                operator+=(cp[8]=new CtrlPoint2D(l+w      ,t+h      ,8));

                SetEventCall(cp[0]->OnMove,this,CtrlRect,OnMoveLeftTop);
                SetEventCall(cp[1]->OnMove,this,CtrlRect,OnMoveCenterTop);
                SetEventCall(cp[2]->OnMove,this,CtrlRect,OnMoveRightTop);

                SetEventCall(cp[3]->OnMove,this,CtrlRect,OnMoveLeftCenter);
                SetEventCall(cp[4]->OnMove,this,CtrlRect,OnMoveCenter);
                SetEventCall(cp[5]->OnMove,this,CtrlRect,OnMoveRightCenter);

                SetEventCall(cp[6]->OnMove,this,CtrlRect,OnMoveLeftBottom);
                SetEventCall(cp[7]->OnMove,this,CtrlRect,OnMoveCenterBottom);
                SetEventCall(cp[8]->OnMove,this,CtrlRect,OnMoveRightBottom);

                cp[4]->Enabled=false;
            }

            SetEventCall(OnDraw,this,CtrlRect,DefaultDraw);
            OnChange=nullptr;

            color.One();
            line_color.Set(0.75,0.75,0.75);
            select_color.Set3i(237,243,254);
        }

        CtrlRect::~CtrlRect()
        {
        }

        void CtrlRect::DefaultDraw(Object *)
        {
            CloseTexture();
            CloseBlend();

            SetLineWidth(1);
            SetLineStipple(1,0x5555);

            glColor(line_color);

            glBegin(GL_LINE_LOOP);
                glVertex2fv(cp[0]->Coord);
                glVertex2fv(cp[2]->Coord);
                glVertex2fv(cp[8]->Coord);
                glVertex2fv(cp[6]->Coord);
            glEnd();

            SetScope();
        }

        void CtrlRect::SetLineColor(const Color3f &c)
        {
            line_color=c;
        }

        void CtrlRect::SetColor(const Color3f &c)
        {
            int n=9;

            while(n--)
                cp[n]->Color=c;

            color=c;
        }

        void CtrlRect::SetSelectColor(const Color3f &c)
        {
            int n=9;

            while(n--)
                cp[n]->SelectColor=c;

            select_color=c;
        }

        void CtrlRect::SetScope()
        {
            cp[0]->Coord.Set(Scope.Left                 ,Scope.Top);
            cp[1]->Coord.Set(Scope.Left+Scope.Width/2.0f,Scope.Top);
            cp[2]->Coord.Set(Scope.Left+Scope.Width     ,Scope.Top);

            cp[3]->Coord.Set(Scope.Left                 ,Scope.Top+Scope.Height/2.0f);
            cp[4]->Coord.Set(Scope.Left+Scope.Width/2.0f,Scope.Top+Scope.Height/2.0f);
            cp[5]->Coord.Set(Scope.Left+Scope.Width     ,Scope.Top+Scope.Height/2.0f);

            cp[6]->Coord.Set(Scope.Left                 ,Scope.Top+Scope.Height);
            cp[7]->Coord.Set(Scope.Left+Scope.Width/2.0f,Scope.Top+Scope.Height);
            cp[8]->Coord.Set(Scope.Left+Scope.Width     ,Scope.Top+Scope.Height);
        }

        void CtrlRect::SetScope(const RectScope2f &rs)
        {
                Scope=rs;

                SafeCallEvent(OnChange,(this));

            SetScope();
        }

        bool CtrlRect::Proc_CursorPos(int x,int y,unsigned __int32 key)
        {
            in=Scope.PointIn(x,y);

            if(select)
            {
                RectScope2f NewScope=Scope;

                NewScope.Left+=x-SelectX;
                NewScope.Top +=y-SelectY;

                SelectX=x;
                SelectY=y;

                SetScope(NewScope);

                return(true);
            }

            if(in)
            {
                ObjectControl::Proc_CursorPos(x,y,key);

                return(true);
            }

            return(ObjectControl::Proc_CursorPos(x,y,key));
        }

        bool CtrlRect::Proc_MouseLeftDown(int x,int y,unsigned __int32 key)
        {
            if(in)
            {
                Active=true;

                if(!ObjectControl::Proc_MouseLeftDown(x,y,key))
                {
                    select=true;

                    SelectX=x;
                    SelectY=y;

                    return(true);
                }

                return(true);
            }

            select=false;
            return(ObjectControl::Proc_MouseLeftDown(x,y,key));
        }

        bool CtrlRect::Proc_MouseLeftUp(int x,int y,unsigned __int32 key)
        {
            if(select)
            {
                select=false;
                return(true);
            }

            return(ObjectControl::Proc_MouseLeftUp(x,y,key));
        }

        void CtrlRect::Draw()
        {
            if(in||Active)
            {
                SafeCallEvent(OnDraw,(this));
            }

            if(Active)
                ObjectControl::Draw();
        }

        void CtrlRect::OnMoveLeftTop(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Height+=NewScope.Top -cp[0]->Coord.y;
            NewScope.Width +=NewScope.Left-cp[0]->Coord.x;

            NewScope.Left=cp[0]->Coord.x;
            NewScope.Top =cp[0]->Coord.y;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveCenterTop(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Height+=NewScope.Top-cp[1]->Coord.y;

            NewScope.Top=cp[1]->Coord.y;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveRightTop(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Height+=NewScope.Top-cp[2]->Coord.y;
            NewScope.Width=cp[2]->Coord.x-NewScope.Left;

            NewScope.Top=cp[2]->Coord.y;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveLeftCenter(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Width +=NewScope.Left-cp[3]->Coord.x;

            NewScope.Left=cp[3]->Coord.x;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveCenter(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Left-=(NewScope.Left+NewScope.Width /2.0f)-cp[4]->Coord.x;
            NewScope.Top -=(NewScope.Top +NewScope.Height/2.0f)-cp[4]->Coord.y;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveRightCenter(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Width=cp[5]->Coord.x-NewScope.Left;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveLeftBottom(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Width +=NewScope.Left-cp[6]->Coord.x;
            NewScope.Height=cp[6]->Coord.y-NewScope.Top;

            NewScope.Left=cp[6]->Coord.x;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveCenterBottom(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Height=cp[7]->Coord.y-NewScope.Top;

            SetScope(NewScope);
        }

        void CtrlRect::OnMoveRightBottom(Object *)
        {
            RectScope2f NewScope=Scope;

            NewScope.Width =cp[8]->Coord.x-NewScope.Left;
            NewScope.Height=cp[8]->Coord.y-NewScope.Top;

            SetScope(NewScope);
        }
    }
}
