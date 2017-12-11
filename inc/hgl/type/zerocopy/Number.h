#ifndef HGL_TYPE_ZERO_COPY_NUMBER_INCLUDE
#define HGL_TYPE_ZERO_COPY_NUMBER_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    namespace zerocopy
    {
        template<typename T> class ZCNumber
        {
            T *value_pointer;

        public:

            ZCNumber()
            {
                value_pointer=nullptr;
            }

            ZCNumber(T *v)
            {
                value_pointer=v;
            }

            void *FromStream(void *p)
            {
                value_pointer=(T *)p;

                return(((char *)p)+sizeof(T));
            }

            operator T (){return *value_pointer;}

            bool operator !(){return !(*value_pointer);}

            void operator=(const T &value){*value_pointer=value;}

            T       operator +  (T v){return((*value_pointer)+v);}
            T       operator -  (T v){return((*value_pointer)-v);}
            T       operator *  (T v){return((*value_pointer)*v);}
            T       operator /  (T v){return((*value_pointer)/v);}
            T       operator %  (T v){return((*value_pointer)%v);}
            T       operator &  (T v){return((*value_pointer)&v);}
            T       operator |  (T v){return((*value_pointer)|v);}
            T       operator ~  (T v){return((*value_pointer)~v);}

            void    operator += (T v){(*value_pointer)+=v);}
            void    operator -= (T v){(*value_pointer)-=v);}
            void    operator *= (T v){(*value_pointer)*=v);}
            void    operator /= (T v){(*value_pointer)/=v);}
            void    operator %= (T v){(*value_pointer)%=v);}
            void    operator &= (T v){(*value_pointer)&=v);}
            void    operator |= (T v){(*value_pointer)|=v);}
            void    operator ~= (T v){(*value_pointer)~=v);}

            T       operator ++ (int){return((*value_pointer)++);}                           		///<后置++
            T       operator -- (int){return((*value_pointer)--);}                           		///<后置--

            T       operator ++ ()   {return(++(*value_pointer));}                             	///<前置++
            T       operator -- ()   {return(--(*value_pointer));}                             	///<前置--

            bool    operator >  (T v){return((*value_pointer)> v);}
            bool    operator >= (T v){return((*value_pointer)>=v);}
            bool    operator <  (T v){return((*value_pointer)< v);}
            bool    operator <= (T v){return((*value_pointer)<=v);}

            bool    operator == (T v){return((*value_pointer)==v);}
            bool    operator != (T v){return((*value_pointer)!=v);}
        };//template<typename T> class ZCNumber

        using ZCChar    =ZCNumber<char>;
        using ZCUChar   =ZCNumber<unsigned char>;
        using ZCShort   =ZCNumber<short>;
        using ZCUShort  =ZCNumber<unsigned short>;
        using ZCInt     =ZCNumber<int>;
        using ZCUInt    =ZCNumber<unsigned int>;
        using ZCLong    =ZCNumber<long>;
        using ZCULong   =ZCNumber<unsigned long>;
        using ZCFloat   =ZCNumber<float>;
        using ZCDouble  =ZCNumber<double>;

        using ZCInt8    =ZCNumber<int8>;
        using ZCUInt8   =ZCNumber<uint8>;
        using ZCInt16   =ZCNumber<int16>;
        using ZCUInt16  =ZCNumber<uint16>;
        using ZCInt32   =ZCNumber<int32>;
        using ZCUInt32  =ZCNumber<uint32>;
        using ZCInt64   =ZCNumber<int64>;
        using ZCUInt64  =ZCNumber<uint64>;
    }//namespace zerocopy
}//namepsace hgl
#endif//HGL_TYPE_ZERO_COPY_NUMBER_INCLUDE
