#include<stdio.h>
class BASE
{
public:

    BASE()
    {
        printf("BASE::BASE()\n");
    }

    virtual ~BASE()
    {
        printf("BASE::~BASE()\n");
    }
};

class A:public BASE
{
public:

    A()
    {
        printf("A:A()\n");
    }

    // ~A()
    // {
    //     printf("A::~A()\n");
    // }
};

void main()
{
    A a;
}