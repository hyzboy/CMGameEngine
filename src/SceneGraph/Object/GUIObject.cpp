#include<hgl/gui/GuiObject.h>
#include<hgl/gui/ObjectControl.h>
#include<hgl/graph/TileFont.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			void GUIObject::InitPrivate(const RectScope2i &rs)
			{
				object_attrib|=oaGUI;

				hglSetProperty(		Align,		this,GUIObject::GetAlign,	GUIObject::SetAlign	);

				hglSetProperty(		Left,		this,GUIObject::GetLeft,	GUIObject::SetLeft	);
				hglSetProperty(		Top,		this,GUIObject::GetTop,		GUIObject::SetTop	);
				hglSetProperty(		Width,		this,GUIObject::GetWidth,	GUIObject::SetWidth	);
				hglSetProperty(		Height,		this,GUIObject::GetHeight,	GUIObject::SetHeight);

				hglSetPropertyRead(	Right,		this,GUIObject::GetRight	);
				hglSetPropertyRead(	Bottom,		this,GUIObject::GetBottom	);

				hglSetProperty(		CenterX,	this,GUIObject::GetCenterX,	GUIObject::SetCenterX);
				hglSetProperty(		CenterY,	this,GUIObject::GetCenterY,	GUIObject::SetCenterY);

		//		hglSetProperty(		ViewScope,	this,GUIObject::GetViewScope,GUIObject::SetViewScope);

				hglSetPropertyRead(	MenuCoord,	this,GUIObject::GetMenuCoord);

				hglSetPropertyRead(	MouseFocus,	this,GUIObject::GetFocus	);

				hglSetProperty(		Move,		this,GUIObject::GetMove,	GUIObject::SetMove);

// 				hglSetPropertyString(Hint,hint,	this,GUIObject,GetHint,	SetHint	);

				Name=U16_TEXT("GuiObject");

// 				DrawFont=(TileFont *)GetPointer(hfpFont);
				DrawFont=nullptr;

				Cutout=false;

				align=alNone;
				topmost=false;

				view_scope=rs;

				menu_coord.Zero();

				AutoActive=false;

				mouse_focus=false;

				for(int i=0;i<mbEnd;i++)
					mouse_down[i]=false;

				move=false;

				OnEnter =nullptr;
				OnLeave =nullptr;
// 				OnHint  =nullptr;

				OnClick	=nullptr;
				OnMove	=nullptr;

				CustomizeData=nullptr;
			}

			GUIObject::GUIObject()
			{
				/*left    =0;
				top     =0;

				GetScreenSize(width,height);*/

				int w,h;

				GetScreenSize(w,h);

				GUIObject::InitPrivate(RectScope2i(0,0,w,h));
			}

			GUIObject::GUIObject(const RectScope2i &rs)
			{
				GUIObject::InitPrivate(rs);
			}

			GUIObject::GUIObject(int l,int t,int w,int h)
			{
				//left    =l;
				//top     =t;

				//width   =w;
				//height  =h;

				GUIObject::InitPrivate(RectScope2i(l,t,w,h));
			}

			void GUIObject::SetSuperObject(ObjectControl *so)
			{
				mouse_focus=false;		//清空鼠标焦点

				Object::SetSuperObject(so);
			}

			void GUIObject::SetAlign(GuiAlign new_align)
			{
				if(new_align==align)return;

				if(new_align>=alNone&&new_align<alEnd)
				{
					if(super_object)
						super_object->SetAlignAttrib(this,new_align);

					align=new_align;
				}
			}

			void GUIObject::CallRefresh()
			{
				if(align>alNone&&align<alEnd)return;

				if(super_object)
					super_object->SetRefresh();
			}

			void GUIObject::SetViewScope(const RectScope2i &vs)
			{
				if(view_scope==vs)return;

				view_scope=vs;

				CallRefresh();
			}

			void GUIObject::SetPosition(int l,int t)
			{
				if(view_scope.Left==l
				&&view_scope.Top==t)
				return;

				view_scope.Left=l;
				view_scope.Top=t;

				CallRefresh();
			}

			void GUIObject::SetSize(int w,int h)
			{
				if(view_scope.Width==w
				&&view_scope.Height==h)
				return;

				view_scope.Width=w;
				view_scope.Height=h;

				CallRefresh();
			}

			void GUIObject::SetLeft		(int v){if(view_scope.Left	==v)return; view_scope.Left		=v; CallRefresh();}
			void GUIObject::SetTop		(int v){if(view_scope.Top	==v)return; view_scope.Top		=v; CallRefresh();}
			void GUIObject::SetWidth	(int v){if(view_scope.Width	==v)return; view_scope.Width	=v; CallRefresh();}
			void GUIObject::SetHeight	(int v){if(view_scope.Height==v)return; view_scope.Height	=v; CallRefresh();}

			bool GUIObject::Proc_CursorPos(int x,int y)
			{
				bool result=false;

				mouse_coord=Vector2f(x,y);

				if(move&&mouse_down[mbLeft])		//可以拖动
				{
					SafeCallEvent(OnMove,(this));

					view_scope.Left+=x-mouse_down_point[mbLeft][0];
					view_scope.Top +=y-mouse_down_point[mbLeft][1];

					mouse_down_point[mbLeft][0]=x;
					mouse_down_point[mbLeft][1]=y;

					CallRefresh();
				}

				x-=MenuX;
				y-=MenuY;

				if((x>=0)&&(x<view_scope.Width)
				&&(y>=0)&&(y<view_scope.Height))
				{
					if(!mouse_focus)    //上次鼠标不在里面
					{
						if(AutoActive)
							Active=true;

						SafeCallEvent(OnEnter,(this));	//进入事件

						result=true;

						mouse_focus=true;
					}
				}
				else
				{
					if(mouse_focus)     //上次鼠标在里面
					{
						//if(AutoActive)Active=false;

						SafeCallEvent(OnLeave,(this));	//离开事件

						result=true;

						mouse_focus=false;
					}
				}

				return(result);
			}

			bool GUIObject::Proc_MouseButton(int index,bool push)
			{
				if(index<0||index>=mbEnd)return(false);

				if(push&&mouse_focus)
				{
					mouse_down[index]=true;
					mouse_down_point[index]=mouse_coord;
				}

				if(!push)
				{
					if(mouse_down[index])
					{
						if(mouse_focus)
							SafeCallEvent(OnClick,(this,index));

						mouse_down[index]=false;
					}
				}

				return(false);
			}

			void GUIObject::SetMenuCoord(const Vertex2i &off)
			{
				menu_coord.x=off.x+view_scope.Left;
				menu_coord.y=off.y+view_scope.Top;
			}

			void GUIObject::DrawObject(const Matrix4f *mv)
			{
				if(view_scope.Left!=0||view_scope.Top!=0)
				{
					const Matrix4f new_mv=(*mv)*translate(view_scope.Left,view_scope.Top,0);
					//PushMatrix(GL_MODELVIEW);
					//glTranslate3f(left,top,0);

					Draw(&new_mv);
		/*
				#ifdef _DEBUG
					if(MouseFocus)
					{
						CloseTexture();
						CloseBlend();

						glBegin(GL_LINE_LOOP);
							glColor(1,1,1,1);glVertex2f(0,0);
							glColor(1,0,0,1);glVertex2f(width,0);
							glColor(1,1,0,1);glVertex2f(width,height);
							glColor(0,1,0,1);glVertex2f(0,height);
						glEnd();
					}
				#endif//_DEBUG
		*/
					//PopMatrix(GL_MODELVIEW);
				}
				else
				{
					Draw(mv);
				}
			}

			void GUIObject::SetScissor(int *sci)
			{
				memcpy(scissor,sci,4*sizeof(int));
			}
		}//namespace gui
	}//namespace graph
}//namespace hgl
