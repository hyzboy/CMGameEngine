#include<hgl/platform/Platform.h>

/*
 * Implements the AMD64 calling convention for gcc-based 64bit Unices
 *
 * Author: Ionut "gargltk" Leonte <ileonte@bitdefender.com>
 *
 * Initial author: niteice
 */

using namespace hgl;

typedef uint64 ( *funcptr_t )( void );

static uint64 __attribute__((noinline)) CallX64(const uint64 *args, int cnt, funcptr_t func, uint64 &retQW2, bool returnFloat)
{
    uint64   retQW1;

    // Reference: http://www.x86-64.org/documentation/abi.pdf

    __asm__ __volatile__ (

        "  movq %0, %%rcx \n"   // rcx = cnt
        "  movq %1, %%r10 \n"   // r10 = args
        "  movq %2, %%r11 \n"   // r11 = func

    // Backup stack pointer in R15 that is guaranteed to maintain its value over function calls
        "  movq %%rsp, %%r15 \n"

    // Skip the first 128 bytes on the stack frame, called "red zone",
    // that might be used by the compiler to store temporary values
        "  sub $128, %%rsp \n"

    // Make sure the stack pointer will be aligned to 16 bytes when the function is called
        "  movq %%rcx, %%rdx \n"
        "  salq $3, %%rdx \n"
        "  movq %%rsp, %%rax \n"
        "  sub %%rdx, %%rax \n"
        "  and $15, %%rax \n"
        "  sub %%rax, %%rsp \n"

    // Push the stack parameters, i.e. the arguments that won't be loaded into registers
        "  movq %%rcx, %%rsi \n"
        "  testl %%esi, %%esi \n"
        "  jle endstack \n"
        "  subl $1, %%esi \n"
        "  xorl %%edx, %%edx \n"
        "  leaq 8(, %%rsi, 8), %%rcx \n"
        "loopstack: \n"
        "  movq 112(%%r10, %%rdx), %%rax \n"
        "  pushq %%rax \n"
        "  addq $8, %%rdx \n"
        "  cmpq %%rcx, %%rdx \n"
        "  jne loopstack \n"
        "endstack: \n"

    // Populate integer and floating point parameters
        "  movq %%r10, %%rax \n"
        "  mov     (%%rax), %%rdi \n"
        "  mov    8(%%rax), %%rsi \n"
        "  mov   16(%%rax), %%rdx \n"
        "  mov   24(%%rax), %%rcx \n"
        "  mov   32(%%rax), %%r8 \n"
        "  mov   40(%%rax), %%r9 \n"
        "  add   $48, %%rax \n"
        "  movsd   (%%rax), %%xmm0 \n"
        "  movsd  8(%%rax), %%xmm1 \n"
        "  movsd 16(%%rax), %%xmm2 \n"
        "  movsd 24(%%rax), %%xmm3 \n"
        "  movsd 32(%%rax), %%xmm4 \n"
        "  movsd 40(%%rax), %%xmm5 \n"
        "  movsd 48(%%rax), %%xmm6 \n"
        "  movsd 56(%%rax), %%xmm7 \n"

    // Call the function
        "  call *%%r11 \n"

    // Restore stack pointer
        "  mov %%r15, %%rsp \n"

    // Put return value in retQW1 and retQW2, using either RAX:RDX or XMM0:XMM1 depending on type of return value
        "  movl %5, %%ecx \n"
        "  testb %%cl, %%cl \n"
        "  je intret \n"
        "  lea %3, %%rax \n"
        "  movq %%xmm0, (%%rax) \n"
        "  lea %4, %%rdx \n"
        "  movq %%xmm1, (%%rdx) \n"
        "  jmp endcall \n"
        "intret: \n"
        "  movq %%rax, %3 \n"
        "  movq %%rdx, %4 \n"
        "endcall: \n"

        : : "r" ((uint64)cnt), "r" (args), "r" (func), "m" (retQW1), "m" (retQW2), "m" (returnFloat)
        : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7",
          "%rdi", "%rsi", "%rax", "%rdx", "%rcx", "%r8", "%r9", "%r10", "%r11", "%r15");

    return retQW1;
}
