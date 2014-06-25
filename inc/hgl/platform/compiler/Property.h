#ifndef HGL_PROPERTY_INCLUDE
#define HGL_PROPERTY_INCLUDE

#include<hgl/platform/compiler/EventFunc.h>
namespace hgl
{
	template<typename T> class Property
	{
	public:

		DefEvent(T,Get,() const);
		DefEvent(void,Set,(T));

	public:

		Property()
		{
			Get=0;
			Set=0;
		}

		virtual ~Property()	{}

#ifdef HGL_VARIADIC_TEMPLATES
		operator T() const{return Get();}
		void operator = (T v){Set(v);}
		void operator = (const Property<T> &v){Set(v.operator T());}

        T operator !(){ return !(operator T());}
        T operator ~(){ return ~(operator T());}

		T operator ++ ()	{	T v=operator T();   Set(++v);   return v;	}           ///<前置++
		T operator -- ()	{	T v=operator T();   Set(--v);	return v;   }           ///<前置--

		T operator ++ (int)	{	T r,v;  v=operator T(); r=v; Set(++v); return r;	}   ///<后置++
		T operator -- (int)	{	T r,v;  v=operator T(); r=v; Set(--v); return r;	}   ///<后置--

		void operator += (T v)	{	if(v){Set(operator T() + v);}	}
		void operator -= (T v)	{	if(v){Set(operator T() - v);}	}
		void operator *= (T v)	{	Set(operator T() * v);	}
		void operator /= (T v)	{	Set(operator T() / v);	}
		void operator %= (T v)	{	Set(operator T() % v);	}

        void operator &= (T v)	{	Set(operator T() & v);	}
        void operator |= (T v)	{	Set(operator T() | v);	}

		void operator >>= (int n)	{	Set((operator T())>>n);}
		void operator <<= (int n)	{	Set((operator T())<<n);}
#else
		operator T() const{return CallEvent(Get,());}
		void operator = (T v){CallEvent(Set,(v));}
		void operator = (const Property<T> &v){CallEvent(Set,(v.operator T()));}

        T operator !(){ return !(operator T());}
        T operator ~(){ return ~(operator T());}

		T operator ++ ()	{	T v=operator T();   CallEvent(Set,(++v));   return v;	}           ///<前置++
		T operator -- ()	{	T v=operator T();   CallEvent(Set,(--v));	return v;   }           ///<前置--

		T operator ++ (int)	{	T r,v;  v=operator T(); r=v; CallEvent(Set,(++v)); return r;	}   ///<后置++
		T operator -- (int)	{	T r,v;  v=operator T(); r=v; CallEvent(Set,(--v)); return r;	}   ///<后置--

		void operator += (T v)	{	if(v){CallEvent(Set,(operator T() + v));}	}
		void operator -= (T v)	{	if(v){CallEvent(Set,(operator T() - v));}	}
		void operator *= (T v)	{	CallEvent(Set,(operator T() * v));	}
		void operator /= (T v)	{	CallEvent(Set,(operator T() / v));	}
		void operator %= (T v)	{	CallEvent(Set,(operator T() % v));	}

        void operator &= (T v)	{	CallEvent(Set,(operator T() & v));	}
        void operator |= (T v)	{	CallEvent(Set,(operator T() | v));	}

		void operator >>= (int n)	{	CallEvent(Set,((operator T())>>n));}
		void operator <<= (int n)	{	CallEvent(Set,((operator T())<<n));}
#endif//HGL_VARIADIC_TEMPLATES

		T operator >> (int n)	{	return (operator T())>>n;}
		T operator << (int n)	{	return (operator T())<<n;}

		bool operator == (const T &v){return(operator T()==v);}
		bool operator != (const T &v){return(operator T()!=v);}
	};//class Property

	template<typename T> class PropertyObject:public Property<T *>
	{
	public:

#ifdef HGL_VARIADIC_TEMPLATES
		T *operator ->()const{return Property<T *>::Get();}
		void operator =(void *pointer){Property<T *>::Set((T *)pointer);}
#else
		T *operator ->()const{return CallEvent(Property<T *>::Get,());}
		void operator =(void *pointer){CallEvent(Property<T *>::Set,((T *)pointer));}
#endif//HGL_VARIADIC_TEMPLATES
	};//class PropertyObject

	#ifdef __BORLANDC__
		#define hglSetPropertyRead(name,tp,get)		{name.Get=tp->get;}
		#define hglSetPropertyWrite(name,tp,set)	{name.Set=tp->set;}
	#else
		#define hglSetPropertyRead(name,tp,get)		{	\
														name.Get.vp_this=tp;	\
														name.Get.omf=ObjectMemberFunc(&get);	\
													}

		#define hglSetPropertyWrite(name,tp,set)	{	\
														name.Set.vp_this=tp;	\
														name.Set.omf=ObjectMemberFunc(&set);	\
													}
	#endif//

		#define hglSetProperty(name,tp,get,set)		{	\
														hglSetPropertyRead(name,tp,get);	\
														hglSetPropertyWrite(name,tp,set);	\
                                                	}
}//namespace hgl
#endif//HGL_PROPERTY_INCLUDE
