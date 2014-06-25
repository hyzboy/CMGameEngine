#include<iostream>

using namespace std;

typedef void (__cdecl *InitPROC)();

void __cdecl Init()
{
	cout<<"hello,world"<<endl;
}

int main(int,char **)
{
	InitPROC func=Init;

	func();


}
