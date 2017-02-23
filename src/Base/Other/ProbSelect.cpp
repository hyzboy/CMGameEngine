#include<hgl/type/ProbSelect.h>
#include<hgl/Other.h>
#include<stdarg.h>

namespace hgl
{
    /**
    * 本类构造函数
    * @param data 要添加进去的数据(所占机率表)
    * @param n 数据个数
    */
    ProbSelect::ProbSelect(const uint *data,uint n)
    {
        total=0;

        Add(data,n);
    }

    ProbSelect::ProbSelect()
    {
        total=0;
    }

    void ProbSelect::SetValue(uint index,uint v)
    {
        uint r;

        if(!val.Get(index,r))
            return;

        total-=r;

        val.Set(index,v);

        total+=v;
    }

    /**
    * 添加数据
    * @param data 要添加进去的数据(所占机率表)
    * @param n 数据个数
    */
    void ProbSelect::Add(const uint *data,const uint count)
    {
        uint n=count;
        uint v;

        while(n--)
        {
            v=*data++;

            val.Add(v);

            total+=v;
        }
    }

    /**
    * 添加一个机率数据
    * @param data 机率数据
    */
    void ProbSelect::Add(const uint data)
    {
        val.Add(data);

        total+=data;
    }

    /**
     * 添加多个机率数据,<0为结束
     * @return 添加的数据数量
     */
    uint ProbSelect::AddBatch(const uint first,...)
    {
        va_list va;

        uint count=0;
        int tmp=first;

        va_start(va,first);

        while(tmp>=0)
        {
            val.Add(tmp);
            total+=tmp;

            tmp=va_arg(va,uint);

            ++count;
        }

        va_end(va);
        return(count);
    }

    /**
    * 选择一个机率
    */
    uint ProbSelect::Select()const
    {
        if(!total)return(0);

        uint v=rand()%total;
        uint n=val.GetCount();

        while(n--)
        {
            uint r;

            if(!val.Get(n,r))
                continue;

            if(r==0)continue;

            if(v<r)
                return(n);
            else
                v-=r;
        }

        return(0);
    }
}//namespace hgl
