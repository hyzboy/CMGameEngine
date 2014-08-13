#ifndef HGL_GUI_FORM_INCLUDE
#define HGL_GUI_FORM_INCLUDE

#include<hgl/gui/ObjectControl.h>
#include<hgl/type/Color4f.h>
#include<hgl/graph/Font.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			class MenuItem;

			/**
			* Form是用于提供一个放置控件的面板，它与普通面板的区别在于它会提供有纹理的背景、边框、标题条等等。
			*/
			class Form:public ObjectControl                                                             ///Form类
			{
			protected:

				UTF16String  caption;																	//窗体标题
				Makeup		makeup;																		//标题绘制排版信息

				int			title_bar_height;															//标题条高度

				#include<hgl/object/Object.ProcEvent.h>
				#include<hgl/object/Object.OnEvent.h>

				void InitPrivate(const u16char * =nullptr);

				void SetCaption();

			public:	//属性

				PropertyString	Caption;																///<窗体标题

				bool			TitleBar;																///<是否画标题条
				Color4f			TitleBarColor;															///<标题条颜色
				Color4f			TitleFontColor;															///<标题条字体颜色
				Color4f         BackColor;                                                              ///<窗体背景颜色
				int				Border;																	///<边框尺寸

				bool			CutEvent;																///<是否截取事件

				bool			ClickActive;															///<是否非活动状态需要点击激活(默认为真)

				MenuItem       *PopupMenu;                                                              ///<弹出式菜单

			public:	//事件

				DefEvent(void,OnClose,	(GUIObject *));                                                 ///<窗口被关闭事件

			public:

				Form();
				Form(const RectScope2i &,const u16char * =nullptr);
				Form(int,int,int,int,const u16char * =nullptr);
				virtual ~Form();

				void SetSize(int,int,bool client_rect=false);											///<设置尺寸

				virtual void Draw(const Matrix4f *);
			};//class Form
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_FORM_INCLUDE
