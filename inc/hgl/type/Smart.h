#ifndef HGL_SMART_INCLUDE
#define HGL_SMART_INCLUDE

#include<hgl/TypeFunc.h>
#include<hgl/thread/Atomic.h>
namespace hgl
{
	struct RefCount
	{
		atom_int count;
		atom_int weak;

	public:

		RefCount()
		{
			count=1;
			weak=0;
		}

		virtual ~RefCount()=default;

		virtual void Delete()=0;

		int inc_ref()
		{
			return ++count;
		}

		virtual int unref()
		{
			count--;

			if(count<=0)
			{
				Delete();

				if(weak<=0)
					delete this;

				return 0;
			}

			return count;
		}

		int inc_ref_weak()
		{
			return ++weak;
		}

		int unref_weak()
		{
			weak--;

			if(weak<=0)
			{
				if(count<=0)
				{
					delete this;
					return 0;
				}
			}

			return weak;
		}
	};//struct RefCount

	template<typename T> struct SmartData:public RefCount
	{
		T *data;

	public:

		SmartData(T *ptr)
		{
			data=ptr;
		}

		~SmartData()
		{
			Delete();
		}

		void Delete()
		{
			SAFE_CLEAR(data);
		}
	};//struct template<typename T> struct SmartData

	template<typename T> struct SmartArrayData:public RefCount
	{
		T *data;

	public:

		SmartArrayData(T *ptr)
		{
			data=ptr;
		}

		~SmartArrayData()
		{
			Delete();
		}

		void Delete()
		{
			SAFE_CLEAR_ARRAY(data);
		}

		const T &operator *() const	{return data;}
		const bool operator!() const{return !data;}
	};//struct template<typename T> struct SmartArrayData

	template<typename SD,typename T> class _Smart
	{
	protected:

		typedef _Smart<SD,T> SelfClass;

		SD *sd;

	public:

		_Smart()
		{
			sd=0;
		}

		_Smart(T *ptr)
		{
			if(ptr)
				sd=new SD(ptr);
			else
				sd=0;
		}

		_Smart(const SelfClass &st)
		{
			sd=0;

			set(st);
		}

		virtual ~_Smart()=default;

		T *set(T *ptr)
		{
			if(sd)
				sd->unref();

			if(!ptr)
			{
				sd=0;
				return(0);
			}

			sd=new SD(ptr);

			return ptr;
		}

		void inc_ref(const SelfClass &sc)
		{
			if(sd==sc.sd)return;

			unref();

			sd=sc.sd;

			if(sd)
				sd->inc_ref();
		}

		void unref()
		{
			if(sd)
			{
				sd->unref();
				sd=0;
			}
		}

		void inc_ref_weak(const SelfClass &sc)
		{
			if(sd==sc.sd)return;

			unref();

			sd=sc.sd;

			if(sd)
				sd->inc_ref_weak();
		}

		void unref_weak()
		{
			if(sd)
			{
				sd->unref_weak();
				sd=0;
			}
		}

				T *get()const{return sd?sd->data:0;}
		virtual bool valid()const{return sd;}
				int use_count()const{return sd?sd->count:-1;}
				bool only()const{return sd?sd->count==1:true;}

	public:

		const T &operator *() const	{return *(sd->data);}
		const bool operator!() const{return sd?!(sd->data):true;}

		operator T *()const{return(sd?sd->data:0);}
		T *operator ->()const{return(sd?sd->data:0);}

		bool operator == (const SelfClass &	rp)const{return(get()==rp.get());	}
		bool operator == (const T *			rp)const{return(get()==rp);			}

		bool operator != (const SelfClass &	rp)const{return !(operator==(rp));	}
		bool operator != (const T *			rp)const{return !(operator==(rp));	}
	};//template <typename T> class _Smart

	template<typename T> class WeakPtr;

	/**
	* 共享指针数据类<br>
	* 用于自动释放超出作用域的指针
	*/
	template<typename T> class SharedPtr:public _Smart<SmartData<T>,T>								///共享指针数据类
	{
		friend class WeakPtr<T>;

	public:

		typedef _Smart<SmartData<T>,T> SuperClass;
		typedef SharedPtr<T> SelfClass;

	public:

		SharedPtr():SuperClass(){}
		SharedPtr(T *ptr):SuperClass(ptr){}
		SharedPtr(const SelfClass &sp):SuperClass()
		{
			operator=(sp);
		}

		SharedPtr(const WeakPtr<T> &wp):SuperClass()
		{
			operator=(wp);
		}

		~SharedPtr()
		{
			SuperClass::unref();
		}

		operator T *()
		{
			return SuperClass::get();
		}

		operator const T *()const
		{
			return SuperClass::get();
		}

		T *operator =(T *ptr)
		{
			return SuperClass::set(ptr);
		}

		SelfClass &operator =(const SelfClass &sp)
		{
			SuperClass::inc_ref(sp);

			return(*this);
		}

		SelfClass &operator =(const WeakPtr<T> &wp)
		{
			SuperClass::inc_ref(wp);

			return(*this);
		}

		bool valid()const override{return this->sd?(this->sd->data?true:false):false;}
	};//template <typename T> class SharedPtr

	template<typename T> class WeakArray;

	/**
	* 共享阵列数据类，它在SharedPtr的基础上增加了[]操作符访问，以及在删除时使用delete[]
	*/
	template<typename T> class SharedArray:public _Smart<SmartArrayData<T>,T>						///共享阵列数据类
	{
		friend class WeakArray<T>;

	public:

		typedef _Smart<SmartArrayData<T>,T> SuperClass;
		typedef SharedArray<T> SelfClass;

	public:

		SharedArray():SuperClass(){}
		SharedArray(T *ptr):SuperClass(ptr){}
		SharedArray(const SelfClass &sa):SuperClass()
		{
			operator=(sa);
		}

		SharedArray(const WeakArray<T> &wa):SuperClass()
		{
			operator=(wa);
		}

		~SharedArray()
		{
			SuperClass::unref();
		}

		operator T *()
		{
			return SuperClass::get();
		}

		operator const T *()const
		{
			return SuperClass::get();
		}

		T &operator [](int n)
		{
			return SuperClass::sd->data[n];
		}

		SelfClass &operator =(const SelfClass &sap)
		{
			SuperClass::inc_ref(sap);

			return(*this);
		}

		SelfClass &operator =(const WeakPtr<T> &wp)
		{
			SuperClass::inc_ref(wp);

			return(*this);
		}
	};//template <typename T> class SharedArray

	template<typename T> class WeakPtr:public _Smart<SmartData<T>,T>
	{
		friend class SharedPtr<T>;

	public:

		typedef _Smart<SmartData<T>,T> SuperClass;
		typedef WeakPtr<T> SelfClass;

	public:

		WeakPtr():SuperClass(){}
		WeakPtr(const SharedPtr<T> &sp):SuperClass()
		{
			operator=(sp);
		}

		WeakPtr(const SelfClass &wp):SuperClass()
		{
			operator=(wp);
		}

		~WeakPtr()
		{
			SuperClass::unref_weak();
		}

		operator T *()
		{
			return SuperClass::get();
		}

		operator const T *()const
		{
			return SuperClass::get();
		}

		virtual SuperClass &operator =(const SharedPtr<T> &sp)
		{
			SuperClass::inc_ref_weak(sp);

			return(*this);
		}

		virtual SelfClass &operator =(const SelfClass &wp)
		{
			SuperClass::inc_ref_weak(wp);

			return(*this);
		}
	};//template<typename T> class WeakPtr

	template<typename T> class WeakArray:public _Smart<SmartArrayData<T>,T>
	{
		friend class SharedArray<T>;

	public:

		typedef _Smart<SmartArrayData<T>,T> SuperClass;
		typedef WeakArray<T> SelfClass;

	public:

		WeakArray():SuperClass(){}
		WeakArray(const SharedArray<T> &sap):SuperClass()
		{
			operator=(sap);
		}

		WeakArray(const WeakArray<T> &wap):SuperClass()
		{
			operator=(wap);
		}

		~WeakArray()
		{
			SuperClass::unref_weak();
		}

		operator T *()
		{
			return SuperClass::get();
		}

		operator const T *()const
		{
			return SuperClass::get();
		}

		virtual SuperClass &operator =(const SharedArray<T> &sap)
		{
			SuperClass::inc_ref_weak(sap);

			return(*this);
		}

		virtual SelfClass &operator =(const SelfClass &wap)
		{
			SuperClass::inc_ref_weak(wap);

			return(*this);
		}
	};//template<typename T> class WeakArray
}//namespace hgl
#endif//HGL_SMART_INCLUDE
