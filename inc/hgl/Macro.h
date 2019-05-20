#ifndef HGL_MACRO_INCLUDE
#define HGL_MACRO_INCLUDE
namespace hgl
{
	#define RETURN_OBJECT_OF_ARRAY(array,index,max_count)	return (index<0||index>=max_count)?nullptr:array[index];

	#define NEW_NULL_ARRAY(name,type,count)	{	\
												name=new type[count];	\
												\
												memset(name,0,sizeof(type)*count);	\
											}

	#define SAFE_CLEAR(name)	{	\
									if(name)	\
									{	\
										delete name;	\
										name=nullptr;	\
									}	\
								}

	#define SAFE_CLEAR_ARRAY(name)	{	\
										if(name)	\
										{	\
											delete[] name;	\
											name=nullptr;	\
										}	\
                            		}

	#define SAFE_CLEAR_OBJECT_ARRAY(name,num)	{	\
													if(name&&num>=0)	\
													{	\
														int safe_clear_object_array_number=num;	\
														\
														while(safe_clear_object_array_number--)	\
															if(name[safe_clear_object_array_number])	\
																delete name[safe_clear_object_array_number];	\
														\
														delete[] name;	\
														name=nullptr;	\
													}	\
												}

	#define FREE_OBJECT_ARRAY(name,num)	{	\
											if(name&&num>=0)	\
											{	\
												int free_object_array_number=num;	\
												\
												while(free_object_array_number--)	\
													if(name[free_object_array_number])	\
														delete name[free_object_array_number];	\
												\
												hgl_free(name);	\
												name=nullptr;	\
											}	\
										}

	#define SAFE_FREE_OBJECT_ARRAY(name,num)	{	\
													if(name)	\
														FREE_OBJECT_ARRAY(name,num);	\
												}

	#define SAFE_FREE(name)				{	\
                                            if(name)	\
                                                hgl_free(name);	\
                                        }

	#define SAFE_RECREATE(name,code)	{	\
											if(name)	\
												delete name;	\
											\
											name=code;	\
										}

	#define ARRAY_CALL(name,num,code)	{	\
											int array_call_number=num;	\
											\
											while(array_call_number--)	\
												name[array_call_number]->code;	\
										}

	#define LOAD_FUNC(type,func)	type func(void *buf,int buf_size)	\
									{	\
										if(!buf||buf_size<=0)return 0;	\
										\
										MemoryInputStream ms(buf,buf_size);	\
										\
										return(func(&ms));	\
									}	\
									\
									type func(const UTF16String &filename)	\
									{	\
										FileInputStream fs;	\
										\
										if(fs.Open(filename))	\
											return(func(&fs));	\
										else	\
											return 0;	\
									}

	#define SAVE_FUNC(type,func)	bool func(type data,void *buf,int buf_size)	\
									{	\
										if(!buf||buf_size<=0)return(false);	\
										\
										MemoryOutputStream ms(buf,buf_size);	\
										\
										return(func(data,&ms));	\
									}	\
									\
									bool func(type data,const UTF16String &filename)	\
									{	\
										FileOutputStream fs;	\
										\
										if(fs.CreateTrunc(filename))	\
											return(func(data,&fs));	\
										else	\
											return(false);	\
									}
}//namespace hgl
#endif//HGL_MACRO_INCLUDE
