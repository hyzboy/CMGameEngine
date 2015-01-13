#ifndef HGL_FUNC_LOAD_INCLUDE
#define HGL_FUNC_LOAD_INCLUDE

	/**
	 * 函数加载定义数据结构
	 */
	struct FuncLoad
	{
		char func_name[64];				///<函数名称
		void **func_pointer;			///<函数指针
	};//struct FuncLoad

	#define HGL_FUNC_LOAD_LIST_BEGIN(name)	struct FuncLoad name[]={
	#define HGL_FUNC_LOAD(name)				{#name,(void **)&name},
	#define HGL_FUNC_LOAD_LIST_END			{"",(void **)0}};

	inline void ClearFuncLoadPointer(struct FuncLoad *flp)
	{
		if(!flp)return;

		while(flp->func_pointer)
		{
			flp->func_pointer=(void **)0;

			++flp;
		}
	}
#endif//HGL_FUNC_LOAD_INCLUDE
