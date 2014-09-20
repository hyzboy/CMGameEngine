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
				CharMakeup():color(0xffffffff){}
				int left;
				int top;

				uint32 color;

				RectScope2f scope;

				u16char ch;
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

			void SetColor(uint32 color)
			{
				int n = MakeupString.GetCount();

				CharMakeup *cm = MakeupString.GetData();
				
				while (n--)
				{
					cm->color = color;
					++cm;
				}
			}

			void SetColor(float r,float g,float b,float a)
			{
				SetColor(HGL_FLOAT_TO_RGBA8(r, g, b, a));
			}

			void SetColor(const Color4f &c)
			{
        		SetColor(c.ToRGBA8());
			}

			void SetColor(int index,const Color4f &c)
			{
				if(index<0)return;

				const int count=MakeupString.GetCount();

				if(index>=count)return;
				
				CharMakeup *cm=MakeupString.GetData();

				cm[index].color = c.ToRGBA8();
			}

			void SetColor(int start,int num,const Color4f &c)
			{
				if(start<0||num<=0)return;

				const int count=MakeupString.GetCount();

				if(start>=count)return;

				if(start+num>count)
					num=count-start;
				
				CharMakeup *cm=MakeupString.GetData();

				cm+=start;

				uint32 c32 = c.ToRGBA8();

				for(int i=0;i<num;i++)
				{
					cm->color=c32;
					++cm;
				}
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
