#include"Common.h"
#include<hgl/Str.h>

int ParseField(UTF16StringList *sl,const UTF16String &str,const u16char ch,bool null_end)
{
	const u16char *sp,*p;
	u16char *tmp;
	bool all_null=true;

	sl->Clear();

	sp=str.c_str();
	p=sp;

	int count=0;

	if(!sp)
		return(-1);

	tmp=new u16char[str.Length()+1];

	do
	{
		p=strchr(sp,ch);

		if(p)
		{
			hgl::strcpy(tmp,str.Length(),sp,p-sp);
			tmp[p-sp]=0;

			if(p>sp)
			{
				sl->Add(tmp);
				LOG_INFO(tmp);
            	all_null=false;
				count++;
			}
			else
			{					//如果出现字段为空
				if(null_end)
					break;

				sl->Add(U16_TEXT(""));	//空的也要加
				count++;
			}

			sp=++p;
		}
		else
		{
			if(strlen(sp)>0)
			{
				LOG_INFO(sp);
				sl->Add(sp);
				count++;
			}

			if(!p||p>sp)
				all_null=false;

			break;
		}
	}while(*sp);

	if(all_null)
    	sl->Clear();

	delete[] tmp;
	return(count);
}
