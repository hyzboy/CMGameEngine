#include"Common.h"

int ParseField(UTF16StringList *sl,const UTF16String &str,const char16_t ch,bool null_end)
{
	const char16_t *sp,*p;
	char16_t *tmp;
	bool all_null=true;

	sl->Clear();

	sp=str.c_str();
	p=sp;

	int count=0;

	if(!sp)
		return(-1);

	tmp=new char16_t[str.Length()+1];

	do
	{
		p=strchr(sp,ch);

		if(p)
		{
			strcpy(tmp,sp,p-sp);
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

				sl->Add(u"");	//空的也要加
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
