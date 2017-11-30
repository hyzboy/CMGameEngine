#include<hgl/type/MultiLevelMemoryPool.h>
#include<hgl/algorithm/Rand.h>
namespace hgl
{
    union MEMORY_STACK_MAGIC
    {
        uint8 level;
        uint32 magic;
    };

    struct MemoryStack
    {
        MEMORY_STACK_MAGIC magic;
        uint size;
        Stack<uchar *> stack;

    public:

        void *Acquire()
        {
            uchar *result;

            if(stack.GetCount()<=0)
            {
                result=new uchar[size+sizeof(MEMORY_STACK_MAGIC)];               //多分配一个魔数空间

                *(MEMORY_STACK_MAGIC *)result=magic;                             //写上自己的魔数用于标识
            }
            else
            {
                stack.Pop(result);
            }

            return(result+sizeof(MEMORY_STACK_MAGIC));
        }

        void Release(uchar *raw)
        {
            stack.Push(raw);
        }

        bool IsEmpty()
        {
            return(stack.GetCount()==0);
        }

        ~MemoryStack()
        {
            const int count=stack.GetCount();

            if(count<=0)return;

            uchar **p=stack.GetData();

            for(int i=0;i<count;i++)
            {
                delete[] *p;
                ++p;
            }
        }
    };

    /**
        * @param min_s 最小容量单位
        * @param l 级别(每一级翻一倍)
        */
    MultiLevelMemoryPool::MultiLevelMemoryPool(const uint min_s,const uint l)
    {
        min_size=min_s;
        total_level=l;

        use_total=0;
        use_total_block=0;

        mem_stack=new MemoryStack[l];

        max_size=min_s;
        for(uint i=0;i<total_level;i++)
        {
            mem_stack[i].magic.magic=FastRand32();      //产生一个随机数
            mem_stack[i].magic.level=i;                 //写上级别

            mem_stack[i].size=max_size;
            max_size<<=1;
        }
    }

    MultiLevelMemoryPool::~MultiLevelMemoryPool()
    {
        delete[] mem_stack;
    }

    //**********************************************************************************************
    // 未来需要建立一个机制，记录已分配的内存总量。如果达到上限，需要释放掉不使用的
    //**********************************************************************************************

    void *MultiLevelMemoryPool::Acquire(uint size)
    {
        if(size<=0)return(nullptr);
        if(size>max_size)return(nullptr);

        for(uint i=0;i<total_level;i++)
        {
            if(size+1<=mem_stack[i].size)
            {
                void *result=mem_stack[i].Acquire();

                if(result)
                {
                    use_total+=mem_stack[i].size;
                    ++use_total_block;
                }

                return(result);
            }
        }

        return(nullptr);
    }

    bool MultiLevelMemoryPool::Release(void *data)
    {
        if(!data)return(false);

        MEMORY_STACK_MAGIC *magic=(MEMORY_STACK_MAGIC *)data;

        --magic;

        if(magic->level>=total_level)
        {
            //严重错误，级别不对
            return(false);
        }

        MemoryStack *ms=&mem_stack[magic->level];

        if(magic->magic!=ms->magic.magic)
        {
            //严重错误，标识用魔数不对
            return(false);
        }

        ms->Release((uchar *)magic);
        use_total-=ms->size;
        --use_total_block;
        return(true);
    }
}//namespace hgl
