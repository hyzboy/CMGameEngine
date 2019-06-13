#ifndef HGL_ALGORITHM_PSEUDO_RANDOM_NUMBER_INCLUDE
#define HGL_ALGORITHM_PSEUDO_RANDOM_NUMBER_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    namespace algorithm
    {
        /**
        * 假随机数是一种常用技术，用于需要回放的环境，或是需要快速产生随机数的环境。<br>
        * 注: 一般的随机数请使用hgl/Random.h
        * @see Random
        */
        class PseudoRandom                                                                              ///假随机数
        {
        protected:

            uint8 *data;
            uint32 count, pos;

        public:

            PseudoRandom(uint32 init = 1024);                                                           ///<本类构造函数
            ~PseudoRandom();                                                                            ///<本类析构函数

            void Init(uint32 = 1024);                                                                   ///<初始化随机数
            void Clear();                                                                               ///<清除

            void Restart();                                                                             ///<重计算一批随机数

            bool Rand(uint8 *, int);                                                                    ///<产生一段随机数

            void Push(const void *, int);                                                               ///<压入一些数据做为随机数

            template<typename T> void Push(const T &v) { Push(&v, sizeof(T)); }                         ///<压入一些数据做为随机数

            template<typename T> bool Rand(T &v) { return Rand((uint8 *)&v, sizeof(T)); }               ///<产生一个随机数

            template<typename T> T Rand() { T v; Rand((uint8 *)&v, sizeof(T)); return v; }              ///<产生一个随机数

        };//class PseudoRandom
    }//namespace algorithm

//c++11随机数引擎
//  default_random_engine
//  knuth_b
//  ranlux48
//  ranlux24
//  mt19937_64
//  mt19937
//  minstd_rand
//  minstd_rand0

    //c++11分布器
//  uniform_int_distribution (离散型均匀分布)
//  bernoulli_distribution (伯努利分布)
//  geometric_distribution (几何分布)
//  poisson_distribution (卜瓦松分布)
//  binomial_distribution (二项分布)
//  uniform_real_distribution (离散型均匀分布)
//  exponential_distribution (指数分布)
//  normal_distribution (正态分布)
//  gamma_distribution (伽玛分布)

    //c++11随机数用法
//  std::uniform_int_distribution<int> distribution(0, 99); // 以離散型均勻分佈方式產生 int 亂數，範圍落在 0 到 99 之間
//  std::mt19937 engine; // 建立亂數生成引擎

//  auto generator = std::bind(distribution, engine); // 利用 bind 將亂數生成引擎和分布組合成一個亂數生成物件
//  int random = generator();  // 產生亂數

//  int random = distribution(engine)   //等同上面2行
}//namespace hgl
#endif//HGL_ALGORITHM_PSEUDO_RANDOM_NUMBER_INCLUDE
