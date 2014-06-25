#ifndef HGL_GRAPH_MAKEUP_INCLUDE
#define HGL_GRAPH_MAKEUP_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/RectScope.h>
#include<hgl/type/Color4f.h>
#include<hgl/type/List.h>

namespace hgl
{
	namespace graph
	{
		/**
		* 文本排列方式枚举
		*/
		enum TextAlignment			///文本排列方式
		{
			taNone=0,				///<起始定义，无意义

			taLeft,					///<居左(横排)
			taCenter,				///<居中(横排)
			taRight,				///<居右(横排)

			taVRight,				///<居右(竖排)

			taEnd					///<结束定义，无意义
		};//enum TextAlignment

		/**
		* 排版管理
		*/
		struct Makeup
		{
			struct CharMakeup
			{
				int left;
				int top;

				Color4f color;

				RectScope2f scope;

				char16_t ch;
			};

			List<CharMakeup> MakeupString;

			RectScope2f End;

			int Width;		//文本块的宽度
			int Height;		//文本块的高度

			bool UseColor;	//使用这里的颜色

		public:

			__inline const RectScope2f &GetCharScope(int n)const
			{
				if(n>=MakeupString.GetCount())
					return End;
				else
					return MakeupString[n].scope;
			}

			__inline int GetCharOffset(int n)const
			{
				return GetCharScope(n).Left;
			}

			__inline int GetCharWidth(int n)const
			{
				return GetCharScope(n).Width;
			}

		public:

			Makeup()
			{
        		Width=Height=0;
				End.Set(0,0,0,0);
				UseColor=false;
			}

			void Clear()
			{
				MakeupString.Clear();

        		Width=Height=0;
				End.Set(0,0,0,0);
			}

			void SetColor(float r,float g,float b,float a)
			{
				int n=MakeupString.GetCount();

				while(n--)
					MakeupString[n].color.Set(r,g,b,a);
			}

			void SetColor(const Color4f &c)
			{
        		SetColor(c.r,c.g,c.b,c.a);
			}

			void ProcEnd(int dw)
			{
				if(MakeupString.GetCount())
				{
					End=MakeupString[MakeupString.GetCount()-1].scope;		//取最后一个字符的位置

					End.Left+=End.Width;					//向后移
					End.Width=dw;

					Height=End.GetBottom();
				}
				else
					End.Set(0,0,dw,0);
			}

			void operator +=(const CharMakeup &cm)
			{
				MakeupString+=cm;
			}
		};//struct Makeup
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_MAKEUP_INCLUDE
