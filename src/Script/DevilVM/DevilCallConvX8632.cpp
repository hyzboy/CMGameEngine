#include<hgl/platform/Platform.h>

namespace hgl
{
    void *CallCDeclFunction(void *func,const void *args, int paramSize)
    {
        // Copy the data to the real stack. If we fail to do
        // this we may run into trouble in case of exceptions.
        __asm
        {
            // We must save registers that are used
            push ecx

            // Copy arguments from script
            // stack to application stack
            mov  ecx, paramSize
            mov  eax, args
            add  eax, ecx
            cmp  ecx, 0
            je   endcopy
    copyloop:
            sub  eax, 4
            push dword ptr [eax]
            sub  ecx, 4
            jne  copyloop
    endcopy:

            // Call function
            call [func]

            // Pop arguments from stack
            add  esp, paramSize

            // Restore registers
            pop  ecx

            // return value in EAX or EAX:EDX
        }
    }

#ifdef __BORLANDC__
    void *CallThiscallFunction(void *func,const void *obj, const void *args, int paramSize)
    {
        // Copy the data to the real stack. If we fail to do
        // this we may run into trouble in case of exceptions.
        __asm
        {
            // We must save registers that are used
            push ecx

            // Copy arguments from script
            // stack to application stack
            mov  ecx, paramSize
            mov  eax, args
            add  eax, ecx
            cmp  ecx, 0
            je   endcopy
    copyloop:
            sub  eax, 4
            push dword ptr [eax]
            sub  ecx, 4
            jne  copyloop
    endcopy:

            // push object as first parameter
            push obj

            // Call function
            call [func]

            // Pop arguments from stack
            add  esp, paramSize
            add  esp, 4

            // Restore registers
            pop  ecx

            // return value in EAX or EAX:EDX
        }
    }
#else//__BORLANDC__
    void *CallThiscallFunction(void *func,const void *obj, const void *args, int paramSize)
    {
        // Copy the data to the real stack. If we fail to do
        // this we may run into trouble in case of exceptions.
        __asm
        {
            // We must save registers that are used
            push ecx

            // Copy arguments from script
            // stack to application stack
            mov  ecx, paramSize
            mov  eax, args
            add  eax, ecx
            cmp  ecx, 0
            je   endcopy
    copyloop:
            sub  eax, 4
            push dword ptr [eax]
            sub  ecx, 4
            jne  copyloop
    endcopy:

            // Move object pointer to ECX
            mov  ecx, obj

            // Call function
            call [func]

            // Restore registers
            pop  ecx

            // Return value in EAX or EAX:EDX
        }
    }
#endif//__BORLANDC__
}//namespace hgl
