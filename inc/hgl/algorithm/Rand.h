#ifndef HGL_ALGORITHM_RAND_INCLUDE
#define HGL_ALGORITHM_RAND_INCLUDE

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
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_RAND_INCLUDE
