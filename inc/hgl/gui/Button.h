#ifndef HGL_GUI_BUTTON_INCLUDE
#define HGL_GUI_BUTTON_INCLUDE

#include<hgl/object/GUIObject.h>
#include<hgl/type/Color4f.h>
#include<hgl/graph/TileFont.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			/**
			* 按钮是最常见的、不可缺少的组件之一。<br>
			* 需要注意的是: 1.按钮在活动状态下可以被回车或空格激活OnClick事件。被鼠标点击时，只有鼠标弹起的时候，才会激活OnClick事件。<br>
			* 2.只有在当前按钮上按下鼠标左键后又弹起，才会激活OnClick事件。如果鼠标不是在当前按钮上按下的，是不会激活OnClick事件的。
			*/
			class Button:public GUIObject																///按钮控件基类
			{
			public:

				enum State        		//按钮状态
				{
					bsNormal=0,         //正常
					bsDisable,          //无效
					bsSelect,           //选中
					bsPush,             //按下
				};

			protected:

				void InitPrivate();

			public:	//属性

				Color4f OuterColor;																		///<外框颜色
				Color4f InnerColor;																		///<内框颜色

			public:

				Button();
				Button(int,int,int,int);
				virtual ~Button();

				virtual void Draw(const Matrix4f *);
			};//class Button

			/**
			* 文本按钮类，在普通按钮的基础上多显示了一串文本
			*/
			class TextButton:public Button																///文本按钮
			{
			protected:

				UTF16String caption;																			///<标题

				TextAlignment ta;																		///<标题对齐设置
				Makeup makeup;																			///<排版信息

			protected:

				void SetCaption();

				TextAlignment GetTextAlign()const{return ta;}
				void SetTextAlign(TextAlignment a){ta=a;}

				void InitPrivate();

			public:

				PropertyString			Caption;														///<按钮标题属性
				Property<TextAlignment>	Alignment;														///<按钮标题对齐属性
				Color4f					FontColor;														///<字符绘制颜色
				int						Border;															///<字符和按钮边界宽度(默认为2)

			public:

				TextButton();
				TextButton(int,int,int,int,const UTF16String &);
				TextButton(int,int,const UTF16String &);
				virtual ~TextButton();

				virtual void Draw(const Matrix4f *);
			};//class TextButton
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_BUTTON_INCLUDE
