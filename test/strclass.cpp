#include<hgl/type/BaseString.h>
#include<iostream>

using namespace hgl;

int main(int,char **)
{
	UTF8String as1,as2,as3;

	char str[32];
	u16char str16[32]=u"utf16 string";

	std::cout<<str16<<std::endl;

	ftos(str,32,0.03);std::cout<<str<<std::endl;
	ftos(str,32,-1.1);std::cout<<str<<std::endl;
	ftos(str,32,2.2);std::cout<<str<<std::endl;

	as1="123";
	as2="abc";

	as3=as1+as2;

	as3="123"+as1+'a';

	UTF16String ws;

	ws=u"wstest";

	if(ws==u"ws")
		ws=to_u16(as2);
}
