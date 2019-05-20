#ifndef HGL_EVENT_FUNC_INCLUDE
#define HGL_EVENT_FUNC_INCLUDE

#include<hgl/object/_Object.h>
#include<string.h>
namespace hgl
{
	#ifdef __BORLANDC__

		#define	SetEventCall(event_obj,obj_this,class_name,event_func)	event_obj=obj_this->class_name::event_func
		#define SafeCallEvent(event_obj,intro)							{if(event_obj)event_obj intro;}
		#define CallEvent(event_obj,intro)								event_obj intro
		#define DefEvent(result,name,intro)				    			result (__closure *name)intro

	#else

		template <typename RT,typename Func> struct EventFunc
		{
			typedef EventFunc<RT,Func> SelfClass;

			union
			{
				void *vp_this;
				_Object *this_pointer;
			};

			union
			{
				void *vp_func;
				Func func_pointer;
				ObjectMemberFunc omf;
			};

		public:

			EventFunc()
			{
				ClearFunc();
			}

			EventFunc(void *t,void *f)
            {
                ClearFunc();

                vp_this=t;
                vp_func=f;
            }

			void ClearFunc()
            {
                memset(this,0,sizeof(SelfClass));
            }

			bool operator !()const
			{
				if(!vp_func)return(true);

				return(false);
			}

			void operator = (void *v)
			{
				if(v==0)
					memset(this,0,sizeof(SelfClass));			//omf可能不止一个指针的长度，所以必须这样清
			}

			void operator = (const SelfClass &ef)
			{
				memcpy(this,&ef,sizeof(SelfClass));
			}

			bool operator == (void *v)
			{
				return(vp_func==v);
			}

			bool operator != (void *v)
			{
				return(vp_func!=v);
			}

			template<typename ...ARGS>
			RT operator()(ARGS...args)
			{
				return (this_pointer->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			RT operator()(ARGS...args)const
			{
				return (this_pointer->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			RT ThisCall(void *tp,ARGS...args)
			{
				return (((_Object *)tp)->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			RT ThisCall(void *tp,ARGS...args)const
			{
				return (((_Object *)tp)->*(func_pointer))(args...);
			}
		};//template<typename RT,typename Func> struct EventFunc

		template<typename Func> struct EventFunc<void,Func>
		{
			typedef EventFunc<void,Func> SelfClass;

			union
			{
				void *vp_this;
				_Object *this_pointer;
			};

			union
			{
				void *vp_func;
				Func func_pointer;
				ObjectMemberFunc omf;
			};

		public:

			EventFunc()
			{
				memset(this,0,sizeof(SelfClass));
			}

			EventFunc(void *t,void *f)
            {
                memset(this,0,sizeof(SelfClass));

                vp_this=t;
                vp_func=f;
            }

			bool operator !()const
			{
				if(!vp_func)return(true);

				return(false);
			}

			void operator = (void *v)
			{
				if(v==0)
					memset(this,0,sizeof(SelfClass));			//omf可能不止一个指针的长度，所以必须这样清
			}

			void operator = (const SelfClass &ef)
			{
				memcpy(this,&ef,sizeof(SelfClass));
			}

			bool operator == (void *v)
			{
				return(vp_func==v);
			}

			bool operator != (void *v)
			{
				return(vp_func!=v);
			}

			template<typename ...ARGS>
			void operator()(ARGS...args)
			{
				(this_pointer->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			void operator()(ARGS...args)const
			{
				(this_pointer->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			void ThisCall(void *tp,ARGS...args)
			{
				(((_Object *)tp)->*(func_pointer))(args...);
			}

			template<typename ...ARGS>
			void ThisCall(void *tp,ARGS...args)const
			{
				(((_Object *)tp)->*(func_pointer))(args...);
			}
		};//template<void,typename Func> struct EventFunc

		#define SetEventCall(event_obj,obj_this,class_name,event_func)	{	\
																			event_obj.vp_this=obj_this;	\
																			event_obj.vp_func=GetMemberFuncPointer(class_name,event_func);	\
																		}

		#define	SetEventThis(event_obj,obj_this)			event_obj.vp_this=obj_this;

//		#define CallEvent(event_obj,intro)					((event_obj.this_pointer->*(event_obj.func_pointer))intro)

		#define SafeCallEvent(event_obj,intro)				{if(event_obj.vp_func)event_obj intro;}

		#define DefEvent(result,name,intro)					EventFunc<result,result (_Object:: *)intro> name;

	#endif//__BORLANDC__

	/*

	使用方法:

		class Example
		{
			//原Borland/CodeGear方式
			void (__closure *OnClick)(Object *);

			//现通用方式
			DefEvent(void,OnClick,(Object *));
		};

		void Test::ClickProc(Object *);

		void Test::func()
		{
			Example *exp;

			{
				//原Borland/CodeGear方式
				exp->OnClick=this->ClickProc;
				或
				exp->OnClick=ClickProc;

				//现通用方式
				SetEventCall(exp->OnClick,this,Test,ClickProc);
			}

			{
				//原Borland/CodeGear方式
				exp->OnClick(nullptr);

				//现通用方式
				CallEvent(exp->OnClick,(nullptr));

				//C++11方式
				exp->OnClick(nullptr);
			}
		}

	*/
}//namespace hgl
#endif//HGL_EVENT_FUNC_INCLUDE
