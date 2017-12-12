#ifndef HGL_TYPE_ZERO_COPY_DATA_INCLUDE
#define HGL_TYPE_ZERO_COPY_DATA_INCLUDE

namespace hgl
{
    namespace zerocopy
    {
        template<typename T> class ZCData
        {
            T *data_pointer;

        public:

            ZCData()
            {
                data_pointer=nullptr;
            }

            ZCData(void *d)
            {
                data_pointer=(T *)d;
            }

            void *FromStream(void *p)
            {
                data_pointer=(T *)p;

                return(((char *)p)+sizeof(T));
            }

            bool operator !(){return !data_pointer;}

            operator T *(){return data_pointer;}
            T *operator ->(){return data_pointer;}

            bool operator == (const T *d){return((*data_pointer)==(*d));}
            bool operator != (const T *d){return((*data_pointer)!=(*d));}
        };//template<typename T> class ZCData
    }//namespace zerocopy
}//namepsace hgl
#endif//HGL_TYPE_ZERO_COPY_DATA_INCLUDE
