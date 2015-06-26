#include<hgl/thread/ASync.h>
#include<iostream>

using namespace hgl;

/**
 * 异步工作函数
 * 即有参数，也有返回值
 */
int ASyncWorkFuncRP(const int &params)
{
    std::cout<<params;
    return 3;
}

/**
 * 异步工作函数
 * 即有参数，没有返回值
 */
void ASyncWorkFuncP(const int &params)
{
    std::cout<<params;
}

/**
 * 异步工作函数
 * 没有参数，但有返回值
 */
int ASyncWorkFuncR()
{
    return 4;
}

/**
 * 异步工作函数
 * 没有参数，没有返回值
 */
void ASyncWorkFunc()
{
}

int main(int,char **)
{
    ASyncResult<int> *rp=   ASync               (ASyncWorkFuncRP,-1,1);     //创建一个异步工作线程，传入参数1，初始返回值为-1

                            ASyncNoReturn       (ASyncWorkFuncP,2);         //创建一个异步工作线程，传入参数2，无返回值

    ASyncResult<int> *r=    ASyncNoParams<int>  (ASyncWorkFuncR,-2);        //创建一个异步工作线程，无参数，返回值类型为int,初始返回值为-2

                            ASync               (ASyncWorkFunc);            //创建一个异步工作结程，无参数，无返回值


    rp->Wait();                     //等待对应的线程工作结束
    std::cout<<rp->GetResult();     //输出结果

    p->Wait();                      //输出结果
    std::cout<<r->GetResult();      //输出结果

    delete rp;
    delete r;
}
