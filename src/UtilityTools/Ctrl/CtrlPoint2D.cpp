#include<hgl/ut/ctrlpoint2d.h>
#include<hgl/gl/OpenGL.h>
namespace hgl
{
	namespace ctrl
	{
		CtrlPoint2D::CtrlPoint2D(float x,float y,float s)
		{
			Coord.Set(x,y);

			SetEventCall(OnDraw,this,CtrlPoint2D,DefaultDraw);
			OnMove=nullptr;

			in=false;
			select=false;
			Size=s;

			Color.Zero();
			SelectColor.Set3i(237,243,254);
		}

		CtrlPoint2D::~CtrlPoint2D()
		{
		}

		void CtrlPoint2D::DefaultDraw(Object *)
		{
			float l,t,w,h;

			w=Size;
			h=Size;

			l=Coord.x-w/2.0f;
			t=Coord.y-h/2.0f;

			CloseTexture();
			CloseBlend();

			glColor(1,1,1);
			glRectf(l,t,l+w,t+h);

			if(in)
				glColor(SelectColor);
			else
				glColor(Color);

			glRectf(l+1,t+1,l+w-1,t+h-1);
		}

		void CtrlPoint2D::Draw()
		{
			SafeCallEvent(OnDraw,(this));
		}

		void CtrlPoint2D::CheckCoord(int x,int y)
		{
			float s=Size/2;

			if((x>=Coord.x-s)&&(x<Coord.x+s)
			 &&(y>=Coord.y-s)&&(y<Coord.y+s))in=true;
										else in=false;
		}

		bool CtrlPoint2D::Proc_CursorPos(int x,int y,unsigned __int32)
		{
			CheckCoord(x,y);

			if(!select)return(in);                  //即便没有选中，只要鼠标在范围内也算使用了这个事件

			Coord.Move(x-SelectX,y-SelectY);

			SelectX=x;
			SelectY=y;

			SafeCallEvent(OnMove,(this));

			return(true);
		}

		bool CtrlPoint2D::Proc_MouseLeftDown(int x,int y,unsigned __int32)
		{
			if(!in)return(false);

			SelectX=x;
			SelectY=y;

			return(select=true);
		}

		bool CtrlPoint2D::Proc_MouseLeftUp(int,int,unsigned __int32)
		{
			if(select)
			{
				select=false;
				return(true);
			}

			return(false);
		}
	}
}
