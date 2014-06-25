class BaseClass
{
protected:

	int a;

};

class SubClass:public BaseClass
{
	SubClass()
	{
		a=0;
	}
};

template<typename T> class BaseTemplate
{
protected:

	T a;
};

class CharTemplate:public BaseTemplate<char>
{
	CharTemplate()
	{
		a='c';
	}
};

template<typename T> class SubTemplate:public BaseTemplate<T>
{
	SubTemplate()
	{
		a='s';
	}
};

int main(){}
