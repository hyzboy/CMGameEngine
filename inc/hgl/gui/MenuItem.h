#ifndef HGL_GUI_MENUITEM_INCLUDE
#define HGL_GUI_MENUITEM_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/List.h>
#include<hgl/type/Color4f.h>
#include<hgl/gui/RadioObject.h>
#include<hgl/TypeFunc.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			class MenuPage;

			typedef DefEvent(void,MenuItemClickPROC,(GUIObject *));

			/**
			* 菜单项是用来组成一个菜单的基本单元,它本身不能直接显示出来,需要由其它控件来调用显示.<br>
			* 它只负责显示它本身,如果它的下面有子菜单,它会自动创建PopupMenu来显示.而新建的PopupMenu并不在于其下,而是位于整个根GUIControl下.<br>
			*/
			class MenuItem:public RadioObject                          									///菜单项
			{
				#include<hgl/gui/MenuItem.Attrib.h>

			public: //属性

				Property<UTF16String>		Caption;                                                    ///<菜单标题虚拟变量

				Color4f						SeparateLineColor;											///<分隔线颜色
				Color4f						SelectBackColor;											///<选中后的底色
				Color4f						SelectTextColor;											///<选中后的字体颜色
				Color4f						TextColor;													///<文本颜色

				bool        				Visible;                                                    ///<是否显示

				Property<int>				Index;                                                      ///<取当前菜单的索引

				ObjectList<MenuItem>		SubMenu;                                                    ///<子菜单

				PropertyObject<MenuPage>	SubMenuPage;												///<子菜单页

			public: //事件

				DefEvent(void,OnDraw,		(GUIObject *,const Matrix4f *));                            ///<绘制事件
				DefEvent(void,OnClick,		(GUIObject *));                                             ///<菜单点击事件
				DefEvent(void,OnMeasureItem,(GUIObject *,float &,float &));                        		///<取得菜单项尺寸
				DefEvent(void,OnPopupMenu,	(GUIObject *,GUIObject *));                              	///<弹出菜单
				DefEvent(void,OnClose,		(GUIObject *,GUIObject *));                                 ///<关闭菜单

			public:

				MenuItem(int=-1);                                                                       ///<本类构造函数
				MenuItem(const u16char *,int=-1);                                              			///<本类构造函数
				virtual ~MenuItem();                                                                    ///<本类析构函数

				virtual void Draw(const Matrix4f *);

				virtual GUIObject *Popup(ObjectControl *,float,float);                                  ///<弹出菜单
						GUIObject *Popup(ObjectControl *);												///<弹出菜单,在当前鼠标位置
				virtual void Close();                                                                   ///<关闭菜单

				virtual void Measure(float &,float &);                                                  ///<取菜单项尺寸
				virtual void Draw(float,float,const Color4f &);											///<绘制菜单项

				MenuItem *Find(const u16char *);                                                    	///<根据名称查找菜单项

				MenuItem *CreateRadio(int,const u16char * =nullptr);										///<创建收音机按钮性质子菜单并设置点击事件
				MenuItem *Create(const u16char * =nullptr);												///<创建子菜单并设置点击事件

				MenuItem *CreateRadio(int,const u16char *,MenuItemClickPROC);							///<创建收音机按钮性质子菜单并设置点击事件
				MenuItem *Create(const u16char *,MenuItemClickPROC);									///<创建子菜单并设置点击事件

				MenuItem *CreateCheck(const u16char *,bool *);											///<创建勾选项子菜单，并绑定指定变量

				void Clear(){SubMenu.Clear();}															///<清除所有子菜单
			};//class MenuItem
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_MENUITEM_INCLUDE;
