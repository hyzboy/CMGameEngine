#ifndef HAC_TEMPLATE_CPP
#define HAC_TEMPLATE_CPP

#include"HacTemplate.h"
#include<hgl/LogInfo.h>

namespace hgl
{
	template<typename F>
	HacFolder<F> *HacFolder<F>::FindFolder(const UTF16String &filename)
	{
		int start,end,pos;
		int result;
		char16_t *psym=filename.strchr(HGL_DIRECTORY_SEPARATOR);

		if(psym)
		{
			UTF16String name(filename,psym-filename);

			HacFolder<F> *sub_folder=Folder[name];

			if(!sub_folder)
				return(nullptr);

			psym++;

			return sub_folder->FindFolder(UTF16String(psym,psym-filname));
		}
		else
		{
			return Folder[name];
		}
	}

	template<typename F>
	F *HacFolder<F>::FindFile(const UTF16String &filename)
	{
		int start,end,pos;
		int result;
		char16_t *psym=filename.strchr(HGL_DIRECTORY_SEPARATOR);

		if(psym)
		{
			UTF16String name(filename,psym-filename);

			HacFolder<F> *sub_folder=Folder[name];

			if(!sub_folder)
				return(nullptr);

			psym++;

			return sub_folder->FindFile(UTF16String(psym,psym-filname));
		}
		else            //没有找到\,表明是文件
		{
			return File[name];
		}
	}
}//namespace hgl

namespace hgl
{
	template<typename F>
	bool HacTemplate<F>::FindFile(const UTF16String &filename)
	{
		LowerString(filename,lower_filename);

		return RootFolder->FindFile(lower_filename);
	}

	template<typename F>
	bool HacTemplate<F>::FindFile(void *folder,const UTF16String &filename)
	{
		HacFolder<F> *obj;

		if(folder)
			obj=(HacFolder<F> *)folder;
		else
			obj=RootFolder;

		LowerString(filename,lower_filename);

		return obj->FindFile(lower_filename);
	}

	template<typename F>
	Stream *HacTemplate<F>::LoadFile(const UTF16String &filename,bool load_to_memory)
	{
//		PutInfo(u"将文件名全部转小写化<%s>",filename);

		LowerString(filename,lower_filename);

//		PutInfo(u"文件名由<%s>转换为<%s>完成",filename,lower_filename);

		return(LoadFileFrom(RootFolder,lower_filename,load_to_memory));
	}

	template<typename F>
	bool HacTemplate<F>::LoadFile(const UTF16String &filename,void **data,int *size)
	{
		LowerString(filename,lower_filename);

		return(LoadFileFrom(RootFolder,lower_filename,data,size));
	}

/*	template<typename F>
	bool HacTemplate<F>::OpenFile(const char16_t *filename,FileStream **fp,int *offset,int *size)
	{
		LowerString(filename,lower_filename);

		return(OpenFileFrom(RootFolder,lower_filename,fp,offset,size));
	}*/

	template<typename F>
	void *HacTemplate<F>::GetFolder(const UTF16String &folder_name)
	{
		if(folder_name.Length<=0)
			return RootFolder;

		UTF16String lower_filename=folder_name;

		lower_filename.LowerCase();

		return RootFolder->FindFolder(lower_filename);
	}

	template<typename F>
	int	HacTemplate<F>::GetFileCount(void *folder)
	{
		if(folder)
			return ((HacFolder<F> *)folder)->File.Count;
		else
        	return RootFolder->File.Count;
	}

	template<typename F>
	bool HacTemplate<F>::GetFileInfo(void *folder,int index,UTF16String &filename,int &size)
	{
		HacFolder<F> *obj;

		if(folder)
			obj=(HacFolder<F> *)folder;
		else
			obj=RootFolder;

		if(index<0||index>=obj->File.Count)return(false);

		filename=obj->File[index]->FileName;
		size	=obj->File[index]->FileSize;

		return(true);
	}

	template<typename F>
	void *HacTemplate<F>::GetFile(void *folder,const UTF16String &filename,int *filesize)
	{
		HacFolder<F> *obj;

		if(folder)
			obj=(HacFolder<F> *)folder;
		else
			obj=RootFolder;

		if(obj)
		{
			F *f=obj->FindFile(filename);

			if(f)
			{
				if(filesize)
					*filesize=f->FileSize;

				return(f);
			}
		}

		if(*filesize)
			*filesize=0;

		return NULL;
	}

	template<typename F>
	void *HacTemplate<F>::GetFile(const UTF16String &filename,int *filesize)
	{
		if(RootFolder)
		{
			F *f=RootFolder->FindFile(filename);

			if(f)
			{
				if(filesize)
					*filesize=f->FileSize;

				return(f);
			}
		}

		if(*filesize)
			*filesize=0;

		return NULL;
	}
}//namespace hgl
#endif//HAC_TEMPLATE_CPP
