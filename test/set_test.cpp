#include<hgl/Console.h>
#include<hgl/type/Set.h>
#include<stdlib.h>
#include<iostream>

using namespace hgl;

void out_set(const Set<int> &si)
{
	const int *p=si.GetData();
	const int count=si.GetCount();

	std::cout<<"Set<int>["<<count<<"]:";

	for(int i=0;i<count;i++)
	{
		std::cout<<*p<<",";
		++p;
	}

	std::cout<<std::endl;
}

int main(int,char **)
{
	Set<int> si;

	for(int i=0;i<10;i++)
	{
		si.Add(rand()%100);

		out_set(si);
	}

	for(int i=1;i<9;i++)
	{
		int index=rand()%i;

		std::cout<<"delete "<<index<<std::endl;

		si.DeleteBySerial(index);

		out_set(si);
	}

	return(0);
}
