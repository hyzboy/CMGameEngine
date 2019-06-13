#ifndef HGL_RADIO_OBJECT_INCLUDE
#define HGL_RADIO_OBJECT_INCLUDE

#include<hgl/gui/GuiObject.h>
namespace hgl
{
    namespace graph
    {
        namespace gui
        {
            /**
            * 收音机对象用于多选一的情况，比如一组按钮，或一组菜单<br>
            * 收音机对象有一个组编号，每一组的互相排斥，只能有一个处于Checked=true状态。<br>
            * 但如果这个组编号为-1则没有不产生排斥
            */
            class RadioObject:public GUIObject                                                          ///收音机对像
            {
            protected:

                int team;

                bool check;

            protected:

                int GetTeam(){return team;}

                bool GetCheck(){return check;}
                virtual void SetCheck(bool);

            public: //属性

                Property<int>   RadioTeam;                                                              ///<收音机按钮组编号

                Property<bool>  Checked;                                                                ///<选择状态

            public: //事件

                DefEvent(void,OnChange,(Object *,bool));                                                ///<状态改变事件

            public:

                RadioObject(int=0);
            };//class RadioObject
        }//namespace gui
    }//namespace graph
}//namespace hgl
#endif//HGL_RADIO_OBJECT_INCLUDE
