#include<hgl/gui/ObjectControl.h>
#include<hgl/type/RectScope.h>

namespace hgl
{
	namespace graph
	{
		void GetScreenSize(int &,int &);															///<取得绘制区尺寸

		void ObjectControl::MoveToTopLevel(Object *obj)
		{
			int max;
			int pos=clas_object[ocVisual].Find(obj);

			if(pos==-1||pos==0)return;

			//clas_object[ocVisual].Delete(pos);        //从中移除

			if(((VisualObject *)obj)->TopMost)        //如果本身是要求置顶
			{
				//clas_object[ocVisual].Insert(0,obj);    //直接插在最前面
				clas_object[ocVisual].Move(pos,0);
			}
			else
			{
				max=clas_object[ocVisual].GetCount();

				for(int n=0;n<max;n++)
				{
					#ifdef _DEBUG
						VisualObject *obj=(VisualObject *)(clas_object[ocVisual][n]);

						if(obj->TopMost==false)
					#else
						if(((VisualObject *)(clas_object[ocVisual][n]))->TopMost==false)       //找到不需要置顶的对象
					#endif//_DEBUG
					{
						//clas_object[ocVisual].Insert(n,obj);
						clas_object[ocVisual].Move(pos,n);
						return;
					}
				}

				//运行到这里表明没有找到不是置顶的对象
				//clas_object[ocVisual].Add(obj);
				clas_object[ocVisual].Move(pos,max-1);
			}
		}

		void ObjectControl::MoveToBottomLevel(Object *obj)
		{
			int index=clas_object[ocVisual].Find(obj);

			if(index==-1||index==clas_object[ocVisual].GetCount()-1)return;

			//clas_object[ocVisual].Delete(index);        //从中移除
			//clas_object[ocVisual].Add(obj);             //添加到最后面

			clas_object[ocVisual].Move(index,clas_object[ocVisual].GetCount()-1);
		}

		void ObjectControl::MoveUpLevel(Object *obj)
		{
			int index=clas_object[ocVisual].Find(obj);

			if(index==-1||index==0)return;

			clas_object[ocVisual].Exchange(index,index-1);  //和上一个交换位置
		}

		void ObjectControl::MoveDownLevel(Object *obj)
		{
			int index=clas_object[ocVisual].Find(obj);

			if(index==-1||index==clas_object[ocVisual].GetCount()-1)return;

			clas_object[ocVisual].Exchange(index,index+1);  //和下一个交换位置
		}

		/**
		* 将指定对象移到指定对象之上一层
		* @param obj 要移动的对话
		* @param ref_obj 参考的对象
		*/
		void ObjectControl::MoveUpLevel(Object *obj,Object *ref_object)
		{
			int index		=clas_object[ocVisual].Find(obj);

			if(index==-1||index==0)return;

			clas_object[ocVisual].Delete(index);

			int ref_index	=clas_object[ocVisual].Find(ref_object);

			clas_object[ocVisual].Insert(ref_index,obj);
		}

		/**
		* 将指定对象移到指定对象之上一层
		* @param obj 要移动的对话
		* @param ref_obj 参考的对象
		*/
		void ObjectControl::MoveDownLevel(Object *obj,Object *ref_object)
		{
			int index		=clas_object[ocVisual].Find(obj);

			if(index==-1||index==0)return;

			clas_object[ocVisual].Delete(index);

			int ref_index	=clas_object[ocVisual].Find(ref_object);

			clas_object[ocVisual].Insert(ref_index+1,obj);
		}

		/**
		* 将对象位置调整到左右居中,并清除对象的对齐(Align)属性
		* @param obj 要调整位置的对象
		* @param center 中心线，默认为-1，表示在正中间
		*/
		void ObjectControl::RowCenter(GUIObject *obj,int center)
		{
			if(center==-1)
				obj->view_scope.Left=(view_scope.Width-obj->view_scope.Width)/2;
			else
				obj->view_scope.Left=center-obj->view_scope.Width/2;

			obj->Align=alNone;
		}

		/**
		* 将对象位置调整到上下居中,并清除对象的对齐(Align)属性
		* @param obj 要调整位置的对象
		* @param center 中心线，默认为-1，表示在正中间
		*/
		void ObjectControl::ColCenter(GUIObject *obj,int center)
		{
			if(center==-1)
				obj->view_scope.Top=(view_scope.Height-obj->view_scope.Height)/2;
			else
				obj->view_scope.Top=center-obj->view_scope.Height/2;

			obj->Align=alNone;
		}

		/**
		* 将对象位置调整到左右居中,如果使用正中坐标则同时设置对齐(Align)属性为alCenter
		* @param obj 要调整位置的对象
		* @param x 中心点X坐标，默认为-1，表示在正中间
		* @param y 中心点Y坐标，默认为-1，表示在正中间
		*/
		void ObjectControl::Center(GUIObject *obj,int x,int y)
		{
			if(x==-1)
				obj->view_scope.Left=(view_scope.Width-obj->view_scope.Width)/2;
			else
				obj->view_scope.Left=x-obj->view_scope.Width/2;

			if(y==-1)
				obj->view_scope.Top=(view_scope.Height-obj->view_scope.Height)/2;
			else
				obj->view_scope.Top=y-obj->view_scope.Height/2;

			if(x==-1&&y==-1)
				obj->align=alCenter;
		}

		void ObjectControl::SetRefresh()
		{
			if(RefreshFlag)return;

			RefreshFlag=true;

			int n=clas_object[ocGUI].GetCount();
			while(n--)
				if(clas_object[ocGUI][n]->is_control())
					((ObjectControl *)(clas_object[ocGUI][n]))->SetRefresh();
		}

		void ObjectControl::Refresh()
		{
			if(RefreshFlag)
			{
				AlignObject();      //对旗下对像做对齐处理
				RefreshScissor();   //更新旗下对象的裁剪范围

				int n=clas_object[ocGUI].GetCount();
				while(n--)
				{
					((GUIObject *)(clas_object[ocGUI][n]))->SetMenuCoord(menux,menuy);

					if(clas_object[ocGUI][n]->is_control())
						((ObjectControl *)(clas_object[ocGUI][n]))->Refresh();
				}

				RefreshFlag=false;
			}
		}

		void ObjectControl::AlignObject()
		{
			int n=clas_object[ocAlign].GetCount();
			RectScope2f scope;
			GUIObject *obj;
			GUIObject *client_obj=nullptr;

			scope.Set(0,0,view_scope.Width,view_scope.Height);

			while(n--)
			{
				obj=(GUIObject *)clas_object[ocAlign][n];

				if(obj->Align>alClient)
				{
					if(obj->Align==alLeftTop)
					{
						obj->view_scope.Left=0;
						obj->view_scope.Top=0;
					}
					else
					if(obj->Align==alTopCenter)
					{
						obj->view_scope.Left=(view_scope.Width-obj->view_scope.Width)/2;
						obj->view_scope.Top=0;
					}
					else
					if(obj->Align==alRightTop)
					{
						obj->view_scope.Left=view_scope.Width-obj->view_scope.Width;
						obj->view_scope.Top=0;
					}
					else
					if(obj->Align==alLeftCenter)
					{
						obj->view_scope.Left=0;
						obj->view_scope.Top =(view_scope.Height-obj->view_scope.Height)/2;
					}
					else
					if(obj->Align==alCenter)
					{
						obj->view_scope.Left=(view_scope.Width-obj->view_scope.Width)/2;
						obj->view_scope.Top =(view_scope.Height-obj->view_scope.Height)/2;
					}
					else
					if(obj->Align==alRightCenter)
					{
						obj->view_scope.Left=view_scope.Width-obj->view_scope.Width;
						obj->view_scope.Top =(view_scope.Height-obj->view_scope.Height)/2;
					}
					else
					if(obj->Align==alLeftBottom)
					{
						obj->view_scope.Left=0;
						obj->view_scope.Top =view_scope.Height-obj->view_scope.Height;
					}
					else
					if(obj->Align==alBottomCenter)
					{
						obj->view_scope.Left=(view_scope.Width-obj->view_scope.Width)/2;
						obj->view_scope.Top =view_scope.Height-obj->view_scope.Height;
					}
					else
					if(obj->Align==alRightBottom)
					{
						obj->view_scope.Left=view_scope.Width-obj->view_scope.Width;
						obj->view_scope.Top =view_scope.Height-obj->view_scope.Height;
					}

					continue;
				}

				if(obj->Align==alTop)
				{
					obj->view_scope.Left   =scope.Left;
					obj->view_scope.Top    =scope.Top;
					obj->view_scope.Width  =scope.Width;

					scope.Top   +=obj->view_scope.Height;
					scope.Height-=obj->view_scope.Height;

					continue;
				}

				if(obj->Align==alBottom)
				{
					obj->view_scope.Left   =scope.Left;
					obj->view_scope.Top    =scope.Top+scope.Height-obj->view_scope.Height;
					obj->view_scope.Width  =scope.Width;

					scope.Height-=obj->view_scope.Height;

					continue;
				}

				if(obj->Align==alLeft)
				{
					obj->view_scope.Left   =scope.Left;
					obj->view_scope.Top    =scope.Top;
					obj->view_scope.Height =scope.Height;

					scope.Left  +=obj->view_scope.Width;
					scope.Width -=obj->view_scope.Width;

					continue;
				}

				if(obj->Align==alRight)
				{
					obj->view_scope.Left   =scope.Left+scope.Width-obj->view_scope.Width;
					obj->view_scope.Top    =scope.Top;
					obj->view_scope.Height =scope.Height;

					scope.Width-=obj->view_scope.Width;

					continue;
				}

				if(obj->Align==alClient)
				{
					if(client_obj)
					{
						//已经有一个client_obj了

						obj->view_scope.Left=0;
						obj->view_scope.Top=0;
						obj->view_scope.Width=view_scope.Width;
						obj->view_scope.Height=view_scope.Height;
					}
					else
						client_obj=obj; //占剩下所有面积的控件放在最后处理

					continue;
				}
			}

			if(client_obj)
			{
				client_obj->view_scope.Left    =scope.Left;
				client_obj->view_scope.Top     =scope.Top;
				client_obj->view_scope.Width   =scope.Width;
				client_obj->view_scope.Height  =scope.Height;
			}
		}

		void ObjectControl::RefreshScissor()
		{
			if(SuperObject==nullptr)                    //如果上级为空，就使用全屏做为裁剪范围
			{
				scissor[0]=0;
				scissor[1]=0;
				GetScreenSize(scissor[2],scissor[3]);
			}

			if(Cutout)  //如果当前控件有被设置裁剪
			{
				//根据控件设置来设置裁剪范围
				inside_scissor[0]=menux;
				inside_scissor[1]=menuy;//GetCanvasHeight()-(menuy+Height);
				inside_scissor[2]=view_scope.Width;
				inside_scissor[3]=view_scope.Height;
			}
			else
			{
				memcpy(inside_scissor,scissor,4*sizeof(int));   //如果没有裁剪，则子控件裁剪范围和当前控件一置
			}

			int n=clas_object[ocGUI].GetCount();

			while(n--)
				((GUIObject *)(clas_object[ocGUI][n]))->SetScissor(inside_scissor);
		}
	}//namespace graph
}//namespace hgl
