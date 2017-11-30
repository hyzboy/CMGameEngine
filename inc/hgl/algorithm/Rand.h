#ifndef HGL_ALGORITHM_RAND_INCLUDE
#define HGL_ALGORITHM_RAND_INCLUDE

#include<hgl/type/DataType.h>
#include<cstdlib>
namespace hgl
{
    namespace algorithm
    {
        //此代码取自xiph.org的opus dec
        static inline uint32 FastRand32()
        {
            static uint32 rngseed = 22222;

            rngseed = (rngseed * 96314165) + 907633515;
            return rngseed;
        }

        template<typename T> inline T rand(T min_value,T max_value,T gap)
        {
            int count=(max_value-min_value)/gap;

            return min_value+T(::rand()%count)*gap;
        }
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_RAND_INCLUDE
