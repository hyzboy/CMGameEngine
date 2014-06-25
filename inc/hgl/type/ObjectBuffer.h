#ifndef HGL_OBJECT_BUFFER_INCLUDE
#define HGL_OBJECT_BUFFER_INCLUDE

#include<hgl/type/FinUseData.h>
#include<hgl/type/BaseString.h>
// #include<hgl/HAC.h>
namespace hgl
{
	template<typename T> class ObjectBuffer:public FinUseData<UTF16String,T *>
    {
		typedef FinUseData<UTF16String,T *> fud_class;

        virtual bool Create(const UTF16String &,T *&);
    	void Clear(const UTF16String &,T *&);

    public:

		using FinUseData<UTF16String,T *>::FinUseData;
    	virtual ~ObjectBuffer();
	};

// 	template<typename T> class ObjectBufferFromHAC:public ObjectBuffer<T>
// 	{
//     	bool hac_private;
//
// 		HAC *hac_data;
//
// 		virtual bool Create(const UTF16String &,T *&);
//
// 		HAC *GetHac(){return hac_data;}
// 		void InitPrivate();
//
// 	public:
//
// 		Property<HAC *> hac;
//
// 	public:
//
//     	ObjectBufferFromHAC(int max);
// 		ObjectBufferFromHAC(HAC *,int max);
// 		ObjectBufferFromHAC(const UTF16String &,int max);
// 		virtual ~ObjectBufferFromHAC();
//
// 		void SetHac(HAC *);
// 		void SetHac(const UTF16String &);
//     };
}//namespace hgl
#include<hgl/type/ObjectBuffer.cpp>
#endif//HGL_OBJECT_BUFFER_INCLUDE
