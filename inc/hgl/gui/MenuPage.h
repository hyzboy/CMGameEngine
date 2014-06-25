#ifndef HGL_GUI_MENUPAGE_INCLUDE
#define HGL_GUI_MENUPAGE_INCLUDE

#include<hgl/gui/ObjectControl.h>
#include<hgl/type/Color4f.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			class MenuItem;

			/**
			* 菜单页就是显示一个菜单的列表让用户选择的控件
			*/
			class MenuPage:public ObjectControl                                                         ///菜单页
			{
				List<Object *> SubMenu;

				void UnlinkSubMenu();

			protected:

				virtual void SetActive(bool);

				virtual bool Proc_CursorPos		(int,int);
				virtual bool Proc_MouseButton	(int,bool);

				//virtual bool Proc_JoystickDown	(uint);
				//virtual bool Proc_KeyDown       (uint);

			protected:

				bool ScrollMenu;                    //是否需要滚动菜单
				bool ViewUpScroll,ViewDownScroll;
				float max_width,total_height;

			public: //属性

				MenuItem *Menu;                                                                         ///<菜单

				Color4f OuterColor;																		///<外框颜色
				Color4f InnerColor;																		///<内框颜色

			public:

				MenuPage(MenuItem *,float,float);
				virtual ~MenuPage();

				virtual void Draw(const Matrix4f *);

				virtual void Destroy();
			};//class MenuPage
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_MENUPAGE_INCLUDEDE
