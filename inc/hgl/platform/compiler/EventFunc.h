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
				memset(this,0,sizeof(EventFunc<RT,Func>));
			}

			bool operator !()const
			{
				if(!vp_func)return(true);

				return(false);
			}

			void operator = (void *v)
			{
				if(v==0)
					memset(this,0,sizeof(EventFunc<RT,Func>));			//omf可能不止一个指针的长度，所以必须这样清
			}

			void operator = (const EventFunc<RT,Func> &ef)
			{
				memcpy(this,&ef,sizeof(EventFunc<RT,Func>));
			}

			bool operator == (void *v)
			{
				return(vp_func==v);
			}

			bool operator != (void *v)
			{
				return(vp_func!=v);
			}

#ifdef HGL_VARIADIC_TEMPLATES
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
#endif//HGL_VARIADIC_TEMPLATES
		};//template <typename Func> struct EventFunc

		#define SetEventCall(event_obj,obj_this,class_name,event_func)	{	\
																			event_obj.vp_this=obj_this;	\
																			event_obj.vp_func=GetMemberFuncPointer(class_name,event_func);	\
																		}

		#define	SetEventThis(event_obj,obj_this)			event_obj.vp_this=obj_this;

		#define CallEvent(event_obj,intro)					((event_obj.this_pointer->*(event_obj.func_pointer))intro)

		#define SafeCallEvent(event_obj,intro)				{if(event_obj.vp_func)CallEvent(event_obj,intro);}

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
			}
		}

	*/
}//namespace hgl
#endif//HGL_EVENT_FUNC_INCLUDE
