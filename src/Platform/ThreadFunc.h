hgl_thread_result ThreadFunc(void *ptr)
{
	hgl::Thread *tc=(Thread *)ptr;

	os_char str[(sizeof(void *)+1)<<1];
    OSString thread_addr;

	htos(str,(sizeof(void *)+1)<<1,(uint64)ptr);

	thread_addr=str;

//	LOG_INFO(OS_TEXT("Thread[")+thread_addr+OS_TEXT("] Init"));

	if(!tc->ProcStartThread())
		return(0);

//	LOG_INFO(OS_TEXT("Thread[")+thread_addr+OS_TEXT("] Start"));

	while(tc->Execute());

//	LOG_INFO(OS_TEXT("Thread[")+thread_addr+OS_TEXT("] End"));

	tc->ProcEndThread();

//	LOG_INFO(OS_TEXT("Thread[")+thread_addr+OS_TEXT("] Exit"));

	if(tc->IsExitDelete())
	{
		delete tc;
//		LOG_INFO(OS_TEXT("Thread[")+thread_addr+OS_TEXT("] Delete"));
	}

	return(0);
}

Thread::Thread()
{
	threadptr=0;
}

Thread::~Thread()
{
	Close();
}
