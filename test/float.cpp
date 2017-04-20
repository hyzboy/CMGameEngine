#include<iostream>

#define OUT(name) std::cout<<#name<<" : "<<sizeof(name)<<" bytes"<<std::endl;

int main(int,char **)
{
	OUT(__float128);
	OUT(long double);

	return 0;
}
