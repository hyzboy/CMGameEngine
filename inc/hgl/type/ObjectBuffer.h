#ifndef HGL_OBJECT_BUFFER_INCLUDE
#define HGL_OBJECT_BUFFER_INCLUDE

#include<hgl/type/LRUCache.h>
#include<hgl/type/BaseString.h>
// #include<hgl/HAC.h>
namespace hgl
{
    template<typename F,typename T> class ObjectBuffer:public LRUCache<F,T *>
    {
        typedef LRUCache<F,T *> fud_class;

        virtual bool Create(const F &,T *&);
        void Clear(const F &,T *&);

    public:

        using LRUCache<F,T *>::LRUCache;
        virtual ~ObjectBuffer();
    };

//  template<typename T> class ObjectBufferFromHAC:public ObjectBuffer<T>
//  {
//      bool hac_private;
//
//      HAC *hac_data;
//
//      virtual bool Create(const UTF16String &,T *&);
//
//      HAC *GetHac(){return hac_data;}
//      void InitPrivate();
//
//  public:
//
//      Property<HAC *> hac;
//
//  public:
//
//      ObjectBufferFromHAC(int max);
//      ObjectBufferFromHAC(HAC *,int max);
//      ObjectBufferFromHAC(const UTF16String &,int max);
//      virtual ~ObjectBufferFromHAC();
//
//      void SetHac(HAC *);
//      void SetHac(const UTF16String &);
//     };
}//namespace hgl
#include<hgl/type/ObjectBuffer.cpp>
#endif//HGL_OBJECT_BUFFER_INCLUDE
