#include<hgl/graph/FontSource.h>
#include<string.h>

namespace hgl
{
	namespace graph
	{
		FontSource::FontSource(const Font &f)
		{
			fnt=f;

			memset(char_bitmap,0,sizeof(char_bitmap));
		}

		FontSource::~FontSource()
		{
			for(int i=0;i<0xffff;i++)
				delete[] char_bitmap[i].data;		//delete[] NULL;不是错误，所以不用担心
		}
		
		FontSource::Bitmap *FontSource::GetCharBitmap(wchar_t ch)
		{
			if(!this)
				return(NULL);

			if(ch==0
			 ||ch==L' '		//半角空格
			 ||ch==L'　'	//全角空格
			 ||ch==L'\n'
			 ||ch==L'\r'
			 ||ch==L'\t')return(NULL);	//不能显示的数据或是空格

			if(!char_bitmap[ch].data)
			{
				if(char_bitmap[ch].h==-1)
					return(NULL);

				if(!MakeCharBitmap(ch))
				{
					char_bitmap[ch].h=-1;		//标记为-1以保证未来也不取了
					return(NULL);
				}
			}

			return char_bitmap+ch;
		}
	}//namespace graph
}//namespace hgl
