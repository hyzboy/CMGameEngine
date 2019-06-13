#include<hgl/gui/RadioObject.h>
#include<hgl/gui/MenuItem.h>
#include<hgl/gui/ObjectControl.h>

namespace hgl
{
    namespace graph
    {
        namespace gui
        {
            /**
            * 收音机对像构造函数
            * @param t 当前对象所在组,-1表示不按分组处理
            */
            RadioObject::RadioObject(int t)
            {
                hglSetPropertyRead( RadioTeam,  this,RadioObject::GetTeam);
                hglSetProperty(     Checked,    this,RadioObject::GetCheck,RadioObject::SetCheck);

                object_attrib|=oaRadio;

                team=t;

                check=false;

                OnChange=nullptr;
            }

            void RadioObject::SetCheck(bool _check)
            {
                if(check!=_check)
                {
                    if(!check)      //如果当前是假
                    {
                        check=true;
                        if(team!=-1)        //-1表示不分组
                        {
                            if(super_object)
                                super_object->SetRadioAttrib(this);
                        }
                    }
                    else            //如果当前是真
                    {
                        check=false;
                    }

                    SafeCallEvent(OnChange,(this,_check));
                }
            }
        }//namespace gui
    }//namespace graph
}//namespace hgl
