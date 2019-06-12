#ifndef HGL_RAND_SELECT_INCLUDE
#define HGL_RAND_SELECT_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    /**
    * 随机选择,可以根据设定的范围产生随机数
    */
    template<typename T> class RandSelect                                                           ///随机选择
    {
        T min_value,max_value,pre;
        int gap;

    protected:

        T GetMin(){return min_value;}
        T GetMax(){return max_value;}
        void SetMin(T);
        void SetMax(T);

        void InitPrivate();

    public: //属性

        Property<T> MinValue;                                                                         ///<随机数最小值虚拟变量
        Property<T> MaxValue;                                                                         ///<随机数最大值虚拟变量

    public: //方法

        RandSelect();
        RandSelect(T,T,T);
        virtual ~RandSelect()=default;

        virtual void Set(T,T,T);                                                                    ///<设置随机数产生条件

        virtual T Select();                                                                         ///<随机产生一个随机值
    };//template<typename T> class RandSelect
}//namespace hgl
#include<hgl/type/RandSelect.cpp>
#endif//HGL_RAND_SELECT_INCLUDE;
