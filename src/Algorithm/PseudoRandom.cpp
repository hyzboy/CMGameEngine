#include<hgl/algorithm/PseudoRandom.h>

namespace hgl
{
    namespace algorithm
    {
        /**
        * 本类构造函数
        * @param num 初始化的数据数量
        */
        PseudoRandom::PseudoRandom(uint32 num)
        {
            data = nullptr;
            Init(num);
        }

        PseudoRandom::~PseudoRandom()
        {
            Clear();
        }

        /**
        * 初始化假随机数
        * @param num 初始化的数据数量
        */
        void PseudoRandom::Init(uint32 num)
        {
            Clear();

            if (num < 64)
                count = 64;
            else
                count = power_to_2(num);

            data = new uint8[count];

            Restart();
        }

        void PseudoRandom::Restart()
        {
            if (!data)
                return;

            const int size = count >> 1;
            uint16 *p = (uint16 *)data;

            for (int i = 0; i < size; i++)
                *p++ = rand();
        }

        /**
        * 清除已存数据
        */
        void PseudoRandom::Clear()
        {
            SAFE_CLEAR_ARRAY(data);

            count = 0;
            pos = 0;
        }

        void PseudoRandom::Push(const void *ptr, int size)
        {
            if (!data || !ptr || size <= 0)return;

            if (pos + size <= count)
            {
                memcpy(data + pos, ptr, size);

                pos += size;
            }
            else
            {
                const int s = count - pos;

                memcpy(data + pos, ptr, s);
                pos = size - s;
                memcpy(data, ((char *)ptr) + s, pos);
            }

            if (pos >= count)pos = 0;
        }

        bool PseudoRandom::Rand(uint8 *result, int size)
        {
            if (!data)
                return(false);

            for (int i = 0; i < size; i++)
            {
                *result = data[pos];

                ++result;
                ++pos;
                if (pos >= count)pos = 0;
            }

            return(true);
        }
    }//namespace algorithm
}//namespace hgl
