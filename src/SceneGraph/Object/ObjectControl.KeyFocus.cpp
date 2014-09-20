#include<hgl/gui/ObjectControl.h>

namespace hgl
{
	namespace graph
	{
		void ObjectControl::SetKeyFocus(GUIObject *obj)
		{
			if(!obj)return;

			keyfocus_obj=obj;

			obj->Active=true;

	//		Proc_CursorPos(obj->menux,obj->menuy,0);
		}

		GUIObject *ObjectControl::PrevKeyFocus()
		{
			int index=-1;

			if(clas_object[ocKeyFocus].GetCount()<=0)
				return(nullptr);

			if(active_obj&&active_obj->is_key_focus())
				index=clas_object[ocKeyFocus].Find(active_obj);

			if(index==-1)
			{
				if(keyfocus_obj)
					index=clas_object[ocKeyFocus].Find(keyfocus_obj);

				if(index==-1)index=0;
				else
					index--;
			}
			else
				index--;

			if(index<0)
				index=clas_object[ocKeyFocus].GetCount()-1;

			SetKeyFocus((GUIObject *)(clas_object[ocKeyFocus][index]));
			return(keyfocus_obj);
		}

		GUIObject *ObjectControl::NextKeyFocus()
		{
			int index=-1;

			if(clas_object[ocKeyFocus].GetCount()<=0)
				return(nullptr);

			if(active_obj&&active_obj->is_key_focus())
				index=clas_object[ocKeyFocus].Find(active_obj);

			if(index==-1)
			{
				if(keyfocus_obj)
					index=clas_object[ocKeyFocus].Find(keyfocus_obj);

				if(index==-1)index=0;
				else
					index++;
			}
			else
				index++;

			if(index>=clas_object[ocKeyFocus].GetCount())
				index=0;

			SetKeyFocus((GUIObject *)(clas_object[ocKeyFocus][index]));
			return(keyfocus_obj);
		}

		GUIObject *ObjectControl::FindKeyFocus(GUIObject *center,int ang)
		{
			if(!center)
			{
				if(clas_object[ocKeyFocus].GetCount()<=0)
					return(nullptr);

				if(active_obj&&active_obj->is_key_focus())
					center=(GUIObject *)active_obj;

				if(!center)
				{
					if(keyfocus_obj)
						center=keyfocus_obj;
					else
						return(nullptr);			//即没有活动对象也没有键盘焦点对象
				}
			}

			int n=clas_object[ocKeyFocus].GetCount();

			Vector2f center_point(center->view_scope.GetCenterX(),center->view_scope.GetCenterY());
			Vector2f comp_point;
			Vector2f offset;
			int comp_ang;
			int min_gap=-1,comp_gap;
			GUIObject *fin_obj=nullptr;

			while(n--)
			{
				GUIObject *comp_obj=(GUIObject *)(clas_object[ocKeyFocus][n]);

				if(comp_obj==center)continue;

				comp_point.set(comp_obj->view_scope.GetCenterX(),comp_obj->view_scope.GetCenterY());

				//comp_ang=center_point.Angle(comp_point);
				offset=comp_point-comp_point;
				if(offset[0]>0)		//x>0
				{
					comp_ang=0;		//向右

					if(offset[1]<0)	//y<0
					{
						if(abs(offset[1])>abs(offset[0]))comp_ang=270;			//向上
					}
					else
					if(offset[1]>0)	//y>0
					{
						if(abs(offset[1])>abs(offset[0]))comp_ang=90;			//向下
					}
				}
				else
				if(offset[0]<0)		//x<0
				{
					comp_ang=180;		//向左

					if(offset[1]<0)	//y<0
					{
						if(abs(offset[1])>abs(offset[0]))comp_ang=270;			//向上
					}
					else
					if(offset[1]>0)	//y>0
					{
						if(abs(offset[1])>abs(offset[0]))comp_ang=90;			//向下
					}
				}
				else
				{
					if(offset[1]<0)comp_ang=270;	//向上
							else comp_ang=90;		//向下
				}

				if(hgl_abs(ang-comp_ang)<45)									//找在90度范围内的
				{
					comp_gap=length(center_point,comp_point);				//计算两中心点距离

					if(min_gap==-1||min_gap>comp_gap)						//找出最近的
					{
						min_gap=comp_gap;
						fin_obj=comp_obj;
					}
				}
			}

			if(fin_obj)
				return(fin_obj);
			else
				return(center);
		}

		GUIObject *ObjectControl::KeyFocusUpMove()
		{
			SetKeyFocus(FindKeyFocus(nullptr,270));

			return(keyfocus_obj);
		}

		GUIObject *ObjectControl::KeyFocusDownMove()
		{
			SetKeyFocus(FindKeyFocus(nullptr,90));

			return(keyfocus_obj);
		}

		GUIObject *ObjectControl::KeyFocusLeftMove()
		{
			SetKeyFocus(FindKeyFocus(nullptr,180));

			return(keyfocus_obj);
		}

		GUIObject *ObjectControl::KeyFocusRightMove()
		{
			SetKeyFocus(FindKeyFocus(nullptr,0));

			return(keyfocus_obj);
		}
	}//namespace graph
}//namespace hgl
