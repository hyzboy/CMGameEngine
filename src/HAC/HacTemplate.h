#ifndef HAC_TEMPLATE_INCLUDE
#define HAC_TEMPLATE_INCLUDE

#include<hgl/HAC.h>
#include<hgl/type/List.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	template<typename F> struct HacFolder
	{
		UTF16String FolderName;								///<目录名称
		MapObject<UTF16String,HacFolder<F> >	Folder;		///<子目录列表
		MapObject<UTF16String,F>				File;		///<文件列表

	public:

		HacFolder<F> *FindFolder(const UTF16String &);
		F *FindFile(const UTF16String &);
	};//struct HacFolder

	template<typename F> class HacTemplate:public HAC
	{
	protected:

		HacFolder<F> *RootFolder;

		virtual void LoadFolder(FileStream *,HacFolder<F> *)=0;

		UTF16String lower_filename;

	public:

		HacTemplate();
		virtual ~HacTemplate()HGL_DEFAULT_MEMFUNC;

				bool 	FindFile(const UTF16String &);

				bool	FindFile(void *,const UTF16String &);

    	virtual bool	LoadFilePart(void *,uint,uint,void *)=0;

		virtual Stream *LoadFileFrom(void *,const UTF16String &,bool=false)=0;
		virtual bool 	LoadFileFrom(void *,const UTF16String &,void **,int *)=0;
//		virtual bool 	OpenFileFrom(void *,const UTF16String &,FileStream **,int *,int *)=0;

				Stream *LoadFile(const UTF16String &,bool=false);
				bool 	LoadFile(const UTF16String &,void **,int *);
//				bool 	OpenFile(const UTF16String &,FileStream **,int *,int *);

				void *	GetFolder(const UTF16String &);
				int		GetFileCount(void *);
				bool	GetFileInfo(void *,int,UTF16String &,int &);

				void *	GetFile(void *,const UTF16String &,int *);
				void *	GetFile(const UTF16String &,int *);
	};//class HacTemplate
}//namespace hgl
#ifndef HAC_TEMPLATE_CPP
#include"HacTemplate.cpp"
#endif//HAC_TEMPLATE_CPP
#endif//HAC_TEMPLATE_INCLUDE
