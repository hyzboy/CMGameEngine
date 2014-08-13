#include<iconv.h>
#include<malloc.h>
#include<string.h>
#include<iostream>

using namespace std;

int main(int argc,char **argv)
{
	cout<<"sizeof(u16char)="<<sizeof(u16char)<<endl;

	iconv_t cd=iconv_open("utf-32le","utf-8");

	if(cd==(iconv_t)-1)
		cout<<"gbk->utf32le failed!"<<endl;

	cout<<"cd ="<<cd<<endl;

	char *utf8_str=new char[9];

	strcpy(utf8_str,"utf8_str");

	size_t in_len=8;

	size_t out_len=36;
	u16char *out_str=new u16char[out_len];

	char *in=utf8_str;
	char *out=(char *)out_str;

	cout<<"in: "<<in<<endl;
	cout<<"in_len: "<<in_len<<endl;
	
	size_t result=iconv(cd,
						&in,&in_len,
						&out,&out_len);

	cout<<"iconv(nullptr,nullptr,nullptr,nullptr) return "<<result<<endl;

	//正确返回0，如果不够，in_len会得到一个还没有转的字符个数

	cout<<"in: "<<in<<endl;
	cout<<"in_len: "<<in_len<<endl;
	cout<<"utf8_str: "<<utf8_str<<endl;
	cout<<"out: "<<out<<endl;
	cout<<"out_len: "<<out_len<<endl;
	wcout<<L"out_str: "<<out_str<<endl;

	for(int i=0;i<out_len;i++)
		wcout<<out_str[i]<<L',';

	cout<<endl;

	iconv_close(cd);
	
	return(0);
}
