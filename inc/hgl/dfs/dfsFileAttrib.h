#ifndef HGL_DFS_FILE_ATTRIB_INCLUDE
#define HGL_DFS_FILE_ATTRIB_INCLUDE

#define DFS_LOAD_FILE_TRY_COUNT	5	//加载文件最大尝试次数
#define DFS_SAVE_FILE_TRY_COUNT	5	//保存文件最大尝试次数

#define DFS_FILE_CREATE			0x01	//创建新文件
#define DFS_FILE_TEMP			0x10	//临时文件，不产生实际备份
#define DFS_FILE_NO_VERSION		0x20	//无本关联文件，覆盖时不检测版本号

#endif//HGL_DFS_FILE_ATTRIB_INCLUDE
