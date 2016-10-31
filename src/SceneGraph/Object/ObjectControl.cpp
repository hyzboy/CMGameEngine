#include<hgl/gui/ObjectControl.h>
#include<hgl/gui/RadioObject.h>
#include<hgl/graph/Render.h>
#include<hgl/LogInfo.h>
#include<hgl/VectorMath.h>

namespace hgl
{
	namespace graph
	{
		void ObjectControl::InitPrivate()
		{
			Name=U16_TEXT("ObjectControl");

			object_attrib|=oaControl;

			active_obj=nullptr;
			keyfocus_obj=nullptr;

			RefreshFlag=true;

	//		hglSetProperty(		KeyFocusObject,	this,ObjectControl::GetKeyFocusObject,ObjectControl::SetKeyFocusObject);
	//		hglSetPropertyRead(	ActiveObject,	this,ObjectControl::GetActiveObject);
		}

		ObjectControl::ObjectControl()
		{
			InitPrivate();
		}

		ObjectControl::ObjectControl(int l,int t,int w,int h):GUIObject(l,t,w,h)
		{
			InitPrivate();
		}

		ObjectControl::ObjectControl(const RectScope2i &rs):GUIObject(rs)
		{
			InitPrivate();
		}

		ObjectControl::~ObjectControl()
		{
			Clear();
		}

		bool ObjectControl::CheckObjectAttrib(Object *obj,int index)
		{
			//这样排版只是为了调试方便

			if(index==ocName        )
				return(obj->Name.Length());else
			if(index==ocRes         )
				return(obj->is_res());else
			if(index==ocControl     )
				return(obj->is_control());else
			if(index==ocUpdate      )
				return(obj->is_update()&&obj->Enabled);else
			if(index==ocVisual      )
			{
				if(obj->is_visual()
				&&((VisualObject *)(obj))->Visible)return(true);
				else
					return(false);
			}
			else
			if(index==ocProcEvent   )
			{
				if(!obj->is_proc_event())return(false);

				return(((VisualObject *)(obj))->Visible&&obj->Enabled);
			}
			else
			if(index==ocGUI         )
				return(obj->is_gui());else
			if(index==ocAlign       )
			{
				if(obj->is_gui())
				{
					if(((GUIObject *)obj)->Align<=alNone
					||((GUIObject *)obj)->Align>=alEnd)return(false);
					else return(true);
				}
				else
					return(false);
			}
			else
			if(index==ocRadio		)
			{
				if(obj->is_radio())
				{
					if(((gui::RadioObject *)(obj))->RadioTeam!=-1)
						return(true);
				}
				else
					return(false);
			}
			else
			if(index==ocKeyFocus	)
			{
				if(!obj->is_key_focus())return(false);

				if(!obj->is_gui())
				{
					LOG_ERROR(	OS_TEXT("严重错误，此对象被设置为键盘焦点对象，但它却不属于GUI对象，请修正你的代码！\n"
								"这个报错在release版中不会出来，但这是个严重错误，你必须修正它！"));

					return(false);
				}

				return(((VisualObject *)(obj))->Visible&&obj->Enabled);
			}

			return(false);
		}

		/**
		* 清除当前控制器中的所有对象
		*/
		void ObjectControl::Clear()
		{
			int n=all_object.GetCount();

			active_obj=nullptr;
			keyfocus_obj=nullptr;

			stack_obj.Clear();

			destroy_obj.UnlinkAll();            //解除所有的关联

			while(n--)
				all_object[n]->SuperObject=nullptr;

			obj_clas.Clear();
		}

		/**
		* 让指定对象和当前对象控制器断开关联
		* @param vo 要断开的对象
		* @return 是否成功
		*/
		bool ObjectControl::Unlink(Object *obj)
		{
			if(all_object.Find(obj)==-1)
			{
				//有已被删除的对象存在，是已断开的，可能被再次断开，所以是正确的

	//			DebugLog(u"将不存在的对象和控制器断开！");
				return(false);
			}

			if(active_obj==obj)
				SetActiveAttrib(active_obj,false);

			if(keyfocus_obj==obj)
				SetKeyFocus(nullptr);

			obj->SuperObject=nullptr;
			obj_clas.Unlink(obj);

			if(CheckObjectAttrib(obj,ocAlign))
				SetRefresh();

			return(true);
		}

		/**
		* 解除所有对象关联
		*/
		void ObjectControl::UnlinkAll()
		{
			int n=all_object.GetCount();

			active_obj=nullptr;
			keyfocus_obj=nullptr;

			stack_obj.Clear();

			destroy_obj.UnlinkAll();            //解除所有的关联

			while(n--)
				all_object[n]->SuperObject=nullptr;

			obj_clas.UnlinkAll();
		}

		/**
		* 将指定对象从当前控制器删除
		* @param vo 要删除的对象
		* @return 是否成功
		*/
		bool ObjectControl::Delete(Object *vo)
		{
			if(vo)
				if(Unlink(vo))
				{
					delete vo;

					return(true);
				}

			return(false);
		}

		void ObjectControl::Add(Object *obj)
		{
			if(!obj)
			{
				#ifdef _DEBUG
				LOG_ERROR(U16_TEXT("添加了一个空的对象到控制器中！"));
				#endif//_DEBUG

				return;
			}

			obj_clas.Add(obj);

			obj->SuperObject=this;				// Form 会在设置 SuperObject时设置自己为活动对象
												// 所以必须先将Form添加到ClasList后再设置SuperObject
			if(obj->is_visual())
			{
				if(((VisualObject *)obj)->TopMost)
					MoveToTopLevel(obj);

				if(((VisualObject *)obj)->BottomMost)
					MoveToBottomLevel(obj);
			}

	//        if(obj->isGUI)
			{
				//传给他们一次鼠标坐标，以保证旧的鼠标焦点被清除
			}

			//if(CheckObjectAttrib(obj,ocAlign))    //不管有没有对齐，都该设定刷新，因为需要设置scissor
				SetRefresh();

			/**
			* 这么做是因为有些对像在创建时,它构造函数内又创建的一些子对象要求设为活动状态,
			* 而这时它还没有被加进来,它的上一级也就是这一级无法知晓，就会造成以后的问题．
			*/
			if(obj->is_proc_event())
			{
				if(((EventObject *)obj)->Active)
					SetActiveAttrib((EventObject *)obj,true);

				#ifdef _DEBUG	//调试用代码，无意义
					int index=clas_object[ocProcEvent].Find(obj);

					index=index;
				#endif//
			}
		}

		/**
		* 对象控制器对应的刷新函数
		*/
		void ObjectControl::Update()
		{
			if(destroy_obj.GetCount())
			{
				destroy_obj.Clear();        //清除被标记为要清除的对象
				SetRefresh();
			}

			Refresh();

			{
				int n=clas_object[ocUpdate].GetCount();

				while(n--)
				{
					#ifdef _DEBUG
						EnumObject *obj=(EnumObject *)(clas_object[ocUpdate][n]);

						obj->Update();
					#else
						((EnumObject *)(clas_object[ocUpdate][n]))->Update();
					#endif//_DEBUG
				}
			}
		}

		/**
		* 对象控制器对应的绘制函数
		*/
		void ObjectControl::Draw(const Matrix4f *)
		{
		}

		void ObjectControl::DrawObject(const Matrix4f *mv)
		{
			Matrix4f new_mv;

			//PushMatrix(GL_MODELVIEW);

			if(view_scope.Left!=0||view_scope.Top!=0)
				//glTranslate3f(left,top,0);
				new_mv=(*mv)*translate(view_scope.Left,view_scope.Top,0);
			else
				new_mv=*mv;

			OpenScissor(scissor);
			Draw(&new_mv);

			{
				int n=clas_object[ocVisual].GetCount();

				while(n--)
				{
					//裁剪放在这里而不放在外面是因为旗下的对象有可能改变裁剪范围
					OpenScissor(inside_scissor);

					#ifdef _DEBUG
						VisualObject *obj=(VisualObject *)(clas_object[ocVisual][n]);

						obj->DrawObject(&new_mv);
					#else
						((VisualObject *)(clas_object[ocVisual][n]))->DrawObject(&new_mv);
					#endif//_DEBUG
				}
			}

			OpenScissor(scissor);   //恢复自己的裁剪设置,因为旗下的对象有可能改变裁剪范围
			LastlyDraw(&new_mv);

			//#ifdef _DEBUG
			//	if(MouseFocus)
			//	{
			//		CloseTexture();
			//		CloseBlend();

			//		glBegin(GL_LINE_LOOP);
			//			glColor(1,1,1,1);glVertex2f(0,0);
			//			glColor(1,0,0,1);glVertex2f(width,0);
			//			glColor(1,1,0,1);glVertex2f(width,height);
			//			glColor(0,1,0,1);glVertex2f(0,height);
			//		glEnd();
			//	}
			//#endif//_DEBUG

			//PopMatrix(GL_MODELVIEW);
		}


		Object *ObjectControl::FindObject(const UTF16String &obj_name)
		{
			int n=clas_object[ocName].GetCount();

			//在本级查找,只有有名称的对象才会被加入到ocName的类别中
			while(n--)
				if(clas_object[ocName][n]->Name==obj_name)
					return(clas_object[ocName][n]);

			//在子级查找
			{
				Object *result_object;

				n=clas_object[ocControl].GetCount();
				while(n--)
				{
					result_object=((ObjectControl *)(clas_object[ocControl][n]))->FindObject(obj_name);

					if(result_object)
						return(result_object);
				}
			}

			return(nullptr);
		}

		/**
		* 将当前活动对象压入堆栈
		*/
		void ObjectControl::PushActiveObject()
		{
			if(active_obj)
				stack_obj.Push(active_obj);
		}

		EventObject *ObjectControl::PopActiveObject()
		{
			if(stack_obj.GetCount())
			{
				EventObject *obj;

				stack_obj.Pop(obj);

				if(all_object.Find(obj)==-1)
					return(nullptr);

				if(destroy_obj.Find(obj)!=-1)
					return(nullptr);

				if(obj->is_key_focus())
					SetKeyFocus((GUIObject *)obj);			//设置键盘焦点也会自动设置为活动对象，所以不再设置活动
				else
					obj->Active=true;

				return(obj);
			}
			else
				return(nullptr);
		}

		bool ObjectControl::FindObject(Object *obj)
		{
			return(all_object.Find(obj)!=-1);
		}

		void ObjectControl::SetEnabledAttrib(Object *obj,bool e)
		{
			if(!e)
			{
				if(active_obj==obj)
					active_obj->Active=false;

				clas_object[ocUpdate	].DeleteByData(obj);    //不再刷新
				clas_object[ocProcEvent	].DeleteByData(obj);   	//不再接收事件
				clas_object[ocKeyFocus	].DeleteByData(obj);	//从键盘焦点列表中清除
			}
			else
			{
				#ifdef _DEBUG
				if(all_object.Find(obj)==-1)
				{
					LOG_ERROR(OS_TEXT("一个被要求设置为显示属性的对象并不在这个对象控制器里"));
				}
				else
				#endif//_DEBUG
				{
					if(obj->is_update()    	)clas_object[ocUpdate   ].Add(obj);
					if(obj->is_proc_event()	)clas_object[ocProcEvent].Add(obj);
					if(obj->is_key_focus()	)clas_object[ocKeyFocus ].Add(obj);
				}
			}
		}

		void ObjectControl::SetUpdateAttrib(EnumObject *obj,bool e)
		{
			if(!obj)return;

			if(all_object.Find(obj)==-1)
			{
				LOG_ERROR(OS_TEXT("一个被要求设置刷新属性的对象并不在这个对象控制器里"));
			}
			else
			{
				if(e)
					clas_object[ocUpdate].Add(obj);
				else
					clas_object[ocUpdate].DeleteByData(obj);
			}
		}

		void ObjectControl::SetVisibleAttrib(VisualObject *obj,bool v)
		{
			if(!v)
			{
				if(obj==active_obj)     //是活动对象
					active_obj->Active=false;  //关闭活动状态,这个操作会将活动对象重新加到ProcEvent列表中

				clas_object[ocProcEvent	].DeleteByData(obj);  	//从事件处理列表中清除
				clas_object[ocVisual	].DeleteByData(obj);  	//从显示列表中清除
				clas_object[ocKeyFocus	].DeleteByData(obj);	//从键盘焦点列表中清除
			}
			else
			{
				#ifdef _DEBUG
				if(all_object.Find(obj)==-1)
				{
					LOG_ERROR(OS_TEXT("一个被要求设置为显示属性的对象并不在这个对象控制器里"));
				}
				else
				#endif//_DEBUG
				{
					if(obj->is_visual()    	)clas_object[ocVisual   ].Add(obj);
					if(obj->is_proc_event()	)clas_object[ocProcEvent].Add(obj);
					if(obj->is_key_focus()	)clas_object[ocKeyFocus ].Add(obj);
				}
			}
		}

		void ObjectControl::SetProcEventAttrib(EventObject *obj,bool pe)
		{
			if(!pe)
			{
				clas_object[ocProcEvent].DeleteByData(obj);
			}
			else
			{
				#ifdef _DEBUG
				if(all_object.Find(obj)==-1)
				{
					LOG_ERROR(OS_TEXT("一个被要求设置接收事件属性的对象并不在这个对象控制器里"));
				}
				else
				#endif//_DEBUG
				{
					if(obj->is_proc_event())clas_object[ocProcEvent].Add(obj);
				}
			}
		}

		void ObjectControl::SetActiveAttrib(EventObject *obj,bool a)
		{
			if(obj)
			{
				if(a)
				{
					if(active_obj)                          //将原本的活动对象添加回可视和可事件处理列表
					{
						if(obj==active_obj)return;          //本身就已经是活动对象了，直接返回

						active_obj->Active=false;           //这里使用大写的Active是为了如果对象本身虚拟的SetActive
															//因为对象本身可能是ObjectControl,调用小写的active会造成没有将旗下的对象设为非活动状态
					}

					//理论上到这里时，上面的active_obj->Active=false已经将active_obj赋为NULL了
					if(obj->Visible==false)
					{
						LOG_PROBLEM(OS_TEXT("一个对象被设为活动对象，但它却被要求不显示，请在设置前先将对象Visible属性设为true"));
						return;
					}

					if(obj->Enabled==false)
					{
						LOG_PROBLEM(OS_TEXT("一个对象被设为活动对象，但它却被设定为禁用，请在设置前先将对象Enabled属性设为true"));
						return;
					}

					{
						MoveToTopLevel(obj);

						clas_object[ocProcEvent].DeleteByData(obj);   //将新活动对象从可处理事件列表中删除
						active_obj=obj;

	//					#ifdef _DEBUG
	//						PutInfo(u"对象控制器<%s>内的<%s>对象被设为活动对象!",Name.wc_str(),active_obj->Name.wc_str());
	//					#endif//_DEBUG
					}

					{
						int n=clas_object[ocProcEvent].GetCount();

						while(n--)
							((EventObject *)(clas_object[ocProcEvent][n]))->active=false;
					}

					Active=true;

					return;
				}
				#ifdef _DEBUG
				else
				{
					if(active_obj!=obj)
					{
						//正常不可能到这里
						LOG_ERROR(U16_TEXT("错误:一个非活动对像被要求设为非活动状态!"));
					}
				}
				#endif//_DEBUG
			}

			if(active_obj)
			{
				clas_object[ocProcEvent].Add(active_obj);

				active_obj->active=false;       //这里已经是Active转换的调用了

				active_obj=nullptr;
				//Active=false;     活动对像不存在了,不代表本身不再活动了
									//一个对象被激活,其它对象才被关闭活动状态
			}
		}

		void ObjectControl::SetAlignAttrib(GUIObject *obj,GuiAlign a)
		{
			#ifdef _DEBUG
				if(all_object.Find((Object *)obj)==-1)
				{
					LOG_HINT(U16_TEXT("一个对象要被设置对齐属性，但它不在这个对象控制器里。"));
					return;
				}
			#endif//_DEBUG

			if(obj->Align<=alNone||obj->Align>=alEnd)       //无对齐
			{
				if(a>alNone&&a<alEnd)       //有对齐
				{
					clas_object[ocAlign].Add((Object *)obj);

					SetRefresh();
				}

				return;
			}
			else        //有对齐
			{
				if(a<=alNone||a>=alEnd)     //从有对齐变为无对齐
				{
					clas_object[ocAlign].DeleteByData((Object *)obj);
				}

				SetRefresh();
			}
		}

		void ObjectControl::SetRadioAttrib(GUIObject *go)
		{
			int n=clas_object[ocRadio].GetCount();
			int team=((gui::RadioObject *)go)->RadioTeam;

			while(n--)
			{
				gui::RadioObject *cur=(gui::RadioObject *)(clas_object[ocRadio][n]);

				if(team==cur->RadioTeam)
					if(cur!=go)
						cur->Checked=false;
			}
		}

		void ObjectControl::SetDestroyAttrib(Object *obj)
		{
			if(!obj)return;

			if(destroy_obj.Find(obj)==-1)
				destroy_obj.Add(obj);

			if(active_obj==obj)
			{
				active_obj->Active=false;

				active_obj=nullptr;
			}
		}

		void ObjectControl::SetActive(bool a)
		{
			EventObject::SetActive(a);

			if(!a)      //如果是将本对象设为非活动
			{
				//关闭活动对象
				if(active_obj)
					active_obj->Active=false;
			}
		}

		#ifdef _DEBUG
		void ObjectControl::PutDebugInfo(const UTF16String &forstr)
		{
			int n=all_object.GetCount();
			UTF16String str;

			while(n--)
			{
				Object *obj=all_object[n];

				str=forstr;

				if (n)str.Strcat(U16_TEXT("├"));
				 else str.Strcat(U16_TEXT("└"));

				if(obj->is_gui())
				{
					GUIObject *go=(GUIObject *)obj;

					LOG_INFO(str
						+(go->mouse_focus ? U16_TEXT("●") : U16_TEXT("○"))
						+UTF16String(go->view_scope.Left)
						+UTF16String(go->view_scope.Top)
						+UTF16String(go->view_scope.Width)
						+UTF16String(go->view_scope.Height)
						+UTF16String(obj->Name));
				}
				else
				{
					LOG_INFO(str + U16_TEXT(",Name:") + obj->Name);
				}

				if(!obj->is_control())continue;

				ObjectControl *oc=(ObjectControl *)obj;

				if(oc->all_object.GetCount()<=0)continue;

				str=forstr;

				if(n)str.Strcat(U16_TEXT("│"));
				else str.Strcat(U16_TEXT("　"));

				oc->PutDebugInfo(str);
			}
		}
		#endif//_DEBUG
	}//namespace graph
}//namespace hgl
