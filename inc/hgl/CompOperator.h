#ifndef HGL_COMP_OPERATOR_INCLUDE
#define HGL_COMP_OPERATOR_INCLUDE

namespace hgl
{
	#define CompOperator(name,compfunc)	const bool operator > (name i)const 	{return compfunc(i)>0;}	\
										const bool operator < (name i)const 	{return compfunc(i)<0;}	\
										const bool operator >=(name i)const 	{return compfunc(i)>=0;}\
										const bool operator <=(name i)const 	{return compfunc(i)<=0;}\
										const bool operator ==(name i)const 	{return compfunc(i)==0;}\
										const bool operator !=(name i)const 	{return compfunc(i)!=0;}
}//namespace hgl
#endif//HGL_COMP_OPERATOR_INCLUDE
