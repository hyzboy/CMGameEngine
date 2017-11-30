#ifndef HGL_MULTI_LEVEL_MEMORY_POOL_INCLUDE
#define HGL_MULTI_LEVEL_MEMORY_POOL_INCLUDE

#include<hgl/type/Stack.h>
namespace hgl
{
    struct MemoryStack;

    /**
    * 多级内存池模板，总是保证提供单一的内存块
    */
    class MultiLevelMemoryPool
    {
        uint min_size;
        uint max_size;
        uint total_level;

        uint use_total;                                                                             ///<已使用总量
        uint use_total_block;

        MemoryStack *mem_stack;

    public:

        MultiLevelMemoryPool(const uint min_s,const uint l);
        virtual ~MultiLevelMemoryPool();

        void *Acquire(uint size);                                                                   ///<申请一块内存
        bool Release(void *data);                                                                   ///<释放一块内存

        const uint GetUseTotalBytes()const{return use_total;}                                       ///<取得已使用的内存块总字节数
        const uint GetUseTotalBlock()const{return use_total_block;}                                 ///<取得已使用的内存块数
    };//class MultiLevelMemoryPool
}//namespace hgl
#endif//HGL_MULTI_LEVEL_MEMORY_POOL_INCLUDE
