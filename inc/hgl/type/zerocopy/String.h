#ifndef HGL_TYPE_ZERO_COPY_STRING_INCLUDE
#define HGL_TYPE_ZERO_COPY_STRING_INCLUDE

namespace hgl
{
    namespace zerocopy
    {
        /**
        * ZeroCopy字符串类
        */
        template<typename T> class ZCString
        {
            const T *data;
            int size;

        public:

            ZCString()
            {
                data=nullptr;
                size=0;
            }

            ZCString(const T *d,const int s)
            {
                data=d;
                size=s;
            }

            virtual ~ZCString()=default;

            template<typename L> void *FromStreamString(void *raw)
            {
                size=*(L *)raw;

                data=(T *)(((char *)raw)+sizeof(L));

                return ((char *)data)+size;
            }

            void *FromStreamLongString(void *raw){return FromStreamString<uint32>(raw);}
            void *FromStreamShortString(void *raw){return FromStreamString<uint16>(raw);}
            void *FromStreamTinyString(void *raw){return FromStreamString<uint8>(raw);}

            const int length()const{return size;}
            const T *c_str()const{return data;}

            operator const T *()const{return data;}
        };//template<typename T> class ZCString
    }//namespace zerocopy
}//namespace hgl
#endif//HGL_TYPE_ZERO_COPY_STRING_INCLUDE
