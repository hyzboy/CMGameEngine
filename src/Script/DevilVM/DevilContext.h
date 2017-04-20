#ifndef DevilContextH
#define DevilContextH

#include<hgl/script/DevilVM.h>
#include"DevilFunc.h"
namespace hgl
{
	class DevilScriptModule;

	struct ScriptFuncRunState
	{
		DevilFunc *func;	//函数指针

		int index;			//运行到的指令编号
	};//struct ScriptFuncRunState

	class DevilScriptContext:public DevilContext
	{
		DevilScriptModule *module;

	private:

		ObjectList<ScriptFuncRunState>					run_state;	//运行状态
		ScriptFuncRunState *							cur_state;	//当前状态
		void ClearStack();											//清空运行堆栈
		bool RunContext();											//运行

		bool Start(DevilFunc *,const va_list &);

	private:	//内部方法

		void ScriptFuncCall(DevilFunc *);
		bool Goto(DevilFunc *,int);
		bool Goto(DevilFunc *);
		bool Goto(DevilFunc *);
		bool Return();

	public:

		bool Start(DevilFunc *,...);
		bool Start(const u16char *,...);
		bool StartFlag(DevilFunc *,const u16char *,);
		bool StartFlag(const u16char *,const u16char *,);
		bool Run(const u16char *);
		void Pause();
		void Stop();

		bool Goto(const u16char *);
		bool Goto(const u16char *,const u16char *);

		bool GetCurrentState(UTF16String &,int &);

		bool SaveState(io::DataOutputStream *);
		bool LoadState(io::DataInputStream *);
	};//class DevilContext
}//namespace hgl
#endif//DevilContextH
