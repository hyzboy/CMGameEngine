#include<iostream>

using namespace std;

typedef int HP;
typedef int MP;

class TestClass
{
public:

	TestClass &operator << (HP hp)
	{
		cout<<"HP:"<<hp<<endl;
	}

	TestClass &operator << (MP mp)
	{
		cout<<"MP:"<<mp<<endl;
	}
};

int main(int,char **)
{
	TestClass tc;

	HP hp=10;
	MP mp=20;

	tc<<hp<<mp;
}
