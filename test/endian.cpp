#include<iostream>


using namespace std;

#define TEST(name)	cout<<"sizeof("<<#name<<")="<<sizeof(name)<<endl;

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

int main(int,char **)
{
// 	cout<<"__clang_major__ = "<<__clang_major__<<endl;
// 	cout<<"__clang_minor__ = "<<__clang_minor__<<endl;


	cout<<"HGL_PLATFORM="<<HGL_PLATFORM_STRING<<endl;

	cout<<"c type"<<endl;

	TEST(char);
	TEST(u16char);
	TEST(short);
	TEST(int);
	TEST(long);
	TEST(long int);
	TEST(long long);
	TEST(float);
	TEST(double);
	TEST(long double);
	TEST(void *);

	cout<<endl;
	cout<<"engine type"<<endl;

	TEST(uchar);
	TEST(uint);
	TEST(uint8);
	TEST(uint16);
	TEST(uint32);
	TEST(uint64);

	cout<<endl;

	const uint16 _16=0xABCD;
	const uint8 *_8=(uint8 *)&_16;

	if(*_8==0xAB)
		cout<<"big endian"<<endl;
	else
		cout<<"little endian"<<endl;

	return 0;
}
