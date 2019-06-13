#include<hgl/script/DevilVM.h>
#include<hgl/LogInfo.h>

/*
    设计思想:

        将所有解析好的指令建立成一个列表，使用时按顺序执行。
        指令存放于GameX86Command

        每个指令如果是要调用真实函数，则将参数自动先合并，调用之时直接压入堆栈。
        真实函数呼叫使用AngelScript代码
*/


