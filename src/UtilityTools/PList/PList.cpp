#include<hgl/ut/PList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/type/StringList.h>

namespace hgl
{
	void PList::ReadData(const UTF16StringList &sl)
	{
		int n=sl.GetCount();

		while(n--)
			Add(sl[n]);
	}

	const UTF16String PList::operator[](const UTF16String &name)const
	{
		UTF16String result;

		Get(name,result);

		return result;
	}

	bool PList::LoadTxt(const OSString &filename)
	{
		UTF16StringList sl;

        if(LoadStringListBOM(sl,filename)<=0)
            return(false);

		ReadData(sl);

		return(true);
	}

	bool PList::Add(const UTF16String &key)
	{
		UTF16String name;
		u16char *value;
		int off;

		if(key.Length()<2)return(false);

		if(((off=key.FindChar(U16_TEXT('\t')))==-1)
		&& ((off=key.FindChar(U16_TEXT(' '))) == -1)
		&& ((off=key.FindChar(U16_TEXT('='))) == -1))
			return(false);

		name.Strcpy(key,off);
		off++;

		value=key.c_str()+off;

		while(true)
		{
			if(*value == U16_TEXT('\t')
			|| *value == U16_TEXT('=')
			|| *value == U16_TEXT(' '))
				value++;
			else
			{
				UTF16String str=value;

				Map<UTF16String,UTF16String>::Add(name,str);

				return(true);
			}
		}
	}//bool PList::Add

	bool PList::Add(const UTF16String &key,const UTF16String &value)
	{
		return Map<UTF16String,UTF16String>::Add(key,value);
	}
}//namespace hgl
