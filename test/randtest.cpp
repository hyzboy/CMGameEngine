#include<iostream>
#include<time.h>
#include<hgl/Other.h>
#include<random>
#include <bits/random.h>

using namespace hgl;
using namespace std;

template<typename RandomEngine> void test(RandomEngine &engine,const char *name)
{
	double st,et;

	int64 total;

	st=GetDoubleTime();

	for(int i=0;i<1000000;i++)
		total+=engine();

	et=GetDoubleTime();

	cout<<" "<<name<<" 1000000 time="<<et-st<<endl;
}

#define cpp11test(name)	{std::name engine;test(engine,#name);}

int main(int,char **)
{
	time_t tt=time(nullptr);

	test(rand,"rand");

	cpp11test(default_random_engine)
	cpp11test(knuth_b)
	cpp11test(ranlux48)
	cpp11test(ranlux24)
	cpp11test(mt19937_64)
	cpp11test(mt19937)
	cpp11test(minstd_rand)
	cpp11test(minstd_rand0)

	return 0;
}
