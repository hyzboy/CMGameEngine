#ifndef HGL_GUI_EDITBOX_INCLUDE
#define HGL_GUI_EDITBOX_INCLUDE

#include<hgl/gui/GuiObject.h>
#include<hgl/graph/FontInfo.h>
namespace hgl
{
	namespace graph
	{
		class Renderable;
		class Material;

		namespace gui
		{
			/**
			* 输入编辑框用于让用户对单行文本进行输入和编辑
			*/
			class EditBox:public GUIObject                                                  			///输入编辑框
			{
				#include<hgl/gui/EditBox.Attrib.h>

			public: //属性

				Color4f OuterColor;																		///<外框颜色
				Color4f InnerColor;																		///<内框颜色
				Color4f TextColor;																		///<文本颜色

				int CursorWidth;																		///<光标宽度(象素数，0表示整个字符宽)
				double CursorShowTime;                                                                  ///<光标显示时间(默认0.5,单位为秒)

				bool Border;                                                                            ///<是否画边界

				PropertyString		Text;																///<当前文本
				Property<int>		Cursor;                                                     		///<光标位置
				Property<bool>		Password;															///<是否是在密码输入状态
				Property<u16char>	PasswordChar;														///<密码字符

				//MenuItem *PopupMenu;                                                                    ///<弹出式菜单

			public: //事件

				DefEvent(void,OnChange,	(GUIObject *));                                                	///<改变事件
				DefEvent(bool,OnKey,	(GUIObject *,uint32));                              			///<按钮按下事件
				DefEvent(bool,OnChar,	(GUIObject *,u16char));                                         ///<字符输入事件
				DefEvent(void,OnEmpty,	(GUIObject *,bool));                                            ///<当输入框被置空/不为空时所产生的事件

			public:

				EditBox();                                                                              ///<本类构造函数
				EditBox(int,int,int,const u16char * =nullptr);											///<本类构造函数
				EditBox(int,int,int,int,const u16char * =nullptr);										///<本类构造函数
				virtual ~EditBox();                                                                     ///<本类析构函数

				virtual void Draw(const Matrix4f *);
			};//class EditBox
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_EDITBOX_INCLUDE
