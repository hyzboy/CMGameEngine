#ifndef DevilVariableH
#define DevilVariableH

#include<hgl/type/BaseString.h>
namespace hgl
{
	class DevilEngine;

	class DevilVariable
	{
		DevilEngine *vm;

	public:

    	UTF16String var_name;

	public:

		DevilVariable(DevilEngine *dvm,const UTF16String &name){vm=dvm;var_name=name;}
	};//class DevilVariable
}//namespace hgl
#endif//DevilVariableH
