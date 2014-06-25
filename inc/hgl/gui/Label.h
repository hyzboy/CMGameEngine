#ifndef HGL_GUI_LABEL_INCLUDE
#define HGL_GUI_LABEL_INCLUDE

#include<hgl/gui/GuiObject.h>
#include<hgl/graph/TileFont.h>
namespace hgl
{
	namespace graph
	{
		namespace gui
		{
			/**
			* 标签控件
			*/
			class Label:public GUIObject
			{
			protected:

				UTF16String caption;

				Makeup makeup;

			protected:

				void InitPrivate();

				virtual void SetCaption(const UTF16String &);

			public:	//属性

				PropertyString	Caption;											///<标签标题

				Color4f			Color;												///<标签字符绘制颜色

			public:

				Label();
				Label(int,int,int,int);
				Label(int,int,const UTF16String &);
				virtual ~Label();

				virtual void Draw(const Matrix4f *);
			};//class Label
		}//namespace gui
	}//namespace graph
}//namespace hgl
#endif//HGL_GUI_LABEL_INCLUDE
