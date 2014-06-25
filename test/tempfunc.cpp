#include<stdio.h>

const char *abc()
{
	const char *str="abc";

	return str;
}

const char *hex()
{
	const char *str="hex";

	return str;
}

template<typename func> void out()
{
	printf("%s\n",func());
}

int main()
{
	out<abc>();
	out<hex>();
}
