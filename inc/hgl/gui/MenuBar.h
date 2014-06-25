#ifndef HGL_GUI_MENU_BAR_INCLUDE
#define HGL_GUI_MENU_BAR_INCLUDE

#include<hgl/gui/MenuPage.h>
#include<hgl/gui/MenuItem.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			/**
			* 菜单条一种专为菜单美化设计的工具条(普通工具条ToolsBar也可用来替代菜单条)<br>
			* 按照我们一般的应用性况，它默认属性是顶端对齐，不可移动。
			*/
			class MenuBar:public GUIObject                                                              ///菜单条
			{
				MenuPage *menu;
				MenuItem *active;

				bool menu_popup;

				void DefaultDraw(GUIObject *);

				void PopupMenu();

			protected:

				virtual bool Proc_CursorPos             (int x,int y,unsigned __int32 val);
				virtual bool Proc_MouseLeftDown			(int x,int y,unsigned __int32 val);

				virtual void MenuCloseProc(GUIObject *,GUIObject *);

			public: //属性

				MenuItem Item;                                                                          ///<菜单项

			public: //事件

				DefEvent(void,OnDraw,(GUIObject *));	                                                ///<绘制事件

			public:

				MenuBar();
				~MenuBar();

				virtual void Draw();

				MenuItem *CreateRadio(int index,const char16_t *name=nullptr)							///<创建收音机按钮性质子菜单并设置点击事件
					{return Item.CreateRadio(index,name);}

				MenuItem *Create(const char16_t *name=nullptr){return Item.Create(name);}				///<创建子菜单并设置点击事件

				MenuItem *CreateRadio(int index,const char16_t *name,MenuItemClickPROC proc)				///<创建收音机按钮性质子菜单并设置点击事件
					{return Item.CreateRadio(index,name,proc);}

				MenuItem *Create(const char16_t *name,MenuItemClickPROC proc)							///<创建子菜单并设置点击事件
					{return Item.Create(name,proc);}

				MenuItem *CreateCheck(const char16_t *name,bool *value)                      			///<创建勾选项子菜单，并绑定指定变量
					{return Item.CreateCheck(name,value);}
			};//class MenuBar
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_MENU_BAR_INCLUDE;
