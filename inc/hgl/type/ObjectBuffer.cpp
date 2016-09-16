#ifndef HGL_OBJECT_BUFFER_CPP
#define HGL_OBJECT_BUFFER_CPP

#include<hgl/type/ObjectBuffer.h>
// #include<hgl/io/InputStream.h>

// #ifdef _DEBUG
// #include<hgl/Loginfo.h>
// #endif//_DEBUG

namespace hgl
{
	template<typename F,typename T>
    bool ObjectBuffer<F,T>::Create(const F &name,T *&item)
    {
    	item=new T(name);

        return(true);
    }

	template<typename F,typename T>
	void ObjectBuffer<F,T>::Clear(const F &,T *&item)
    {
		delete item;
	}

	template<typename F,typename T>
	ObjectBuffer<F,T>::~ObjectBuffer()
	{
		fud_class::Clear();
    }
}

// namespace hgl
// {
// 	template<typename T>
// 	void ObjectBufferFromHAC<T>::InitPrivate()
// 	{
// 		hglSetPropertyRead(hac,this,ObjectBufferFromHAC<T>::GetHac);
// 	}
//
// 	template<typename T>
// 	ObjectBufferFromHAC<T>::ObjectBufferFromHAC(int max):ObjectBuffer<T>(max)
// 	{
// 		hac_data=nullptr;
// 		hac_private=false;
//
// 		InitPrivate();
// 	}
//
// 	template<typename T>
// 	ObjectBufferFromHAC<T>::ObjectBufferFromHAC(HAC *_hac,int max):ObjectBuffer<T>(max)
// 	{
//     	SetHac(_hac);
// 		InitPrivate();
// 	}
//
// 	template<typename T>
// 	ObjectBufferFromHAC<T>::ObjectBufferFromHAC(const UTF16String &fn,int max):ObjectBuffer<T>(max)
// 	{
// 		SetHac(fn);
// 		InitPrivate();
// 	}
//
// 	template<typename T>
// 	ObjectBufferFromHAC<T>::~ObjectBufferFromHAC()
// 	{
// 		if(hac_private)
// 			delete hac_data;
// 	}
//
// 	template<typename T>
// 	bool ObjectBufferFromHAC<T>::Create(const UTF16String &name,T *&item)
// 	{
// 		if(!hac_data)return(false);
//
// 		if(hac_data->FindFile(name))
// 		{
// 			item=new T(hac_data,name);
//
// 			return(true);
// 		}
// 		else
// 		{
//         	ErrorHint(u"要取得的资源没有找到:%s",name.wc_str());
// 			return(false);
// 		}
// 	}
//
// 	template<typename T>
// 	void ObjectBufferFromHAC<T>::SetHac(HAC *_hac)
// 	{
// 		hac_data=_hac;
//
// 		hac_private=false;
// 	}
//
// 	template<typename T>
// 	void ObjectBufferFromHAC<T>::SetHac(const UTF16String &fn)
// 	{
// 		hac_data=OpenHAC(fn);
//
// 		hac_private=true;
// 	}
// }
#endif//HGL_OBJECT_BUFFER_CPP
