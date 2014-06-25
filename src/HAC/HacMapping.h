#ifndef HAC_MAPPING_INCLUDE
#define HAC_MAPPING_INCLUDE

#include<hgl/Hac.h>
#include<hgl/type/List.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	struct FolderMapping;

	struct FileMapping
	{
		FolderMapping *folder;
		    	
		UTF16String filename;

		uint filesize;
	};

	struct FolderMapping
	{
		UTF16String full_filename;

		ObjectList<FileMapping> File;
	};

	class HacMapping:public HAC
	{
		UTF16String FolderName;

		char16_t full_filename[4096];

		ObjectList<FolderMapping> Folder;

	public:

		HacMapping(const char16_t *);
		~HacMapping();

		bool 	FindFile(const char16_t *);                                                          ///<查找一个文件

		bool	FindFile(void *,const char16_t *);

		bool	LoadFilePart(void *,uint,uint,void *);												///<加载一个文件的一部分

		Stream *LoadFileFrom(void *,const char16_t *,bool=false);                                    ///<加载一个文件到内存流
		bool 	LoadFileFrom(void *,const char16_t *,void **,int *);                                 ///<加载一个文件到指定内存块

		Stream *LoadFile(const char16_t *,bool=false);                                               ///<加载一个文件到内存流
		bool 	LoadFile(const char16_t *,void **,int *);                                            ///<加载一个文件到指定内存块
//		bool 	OpenFile(const char16_t *,FileStream **,int *,int *);                                ///<打开一个文件

		void *	GetFolder(const char16_t *);															///<取得一个目录

		int 	GetFileCount(void *);																///<取得目录下的文件数量

		bool 	GetFileInfo(void *,int,UTF16String &,int &);												///<取得文件信息
		
		void *	GetFile(void *,const char16_t *,int *);												///<取得文件指针
		void *	GetFile(const char16_t *,int *);														///<取得文件指针
	};
}
#endif//HAC_MAPPING_INCLUDE
