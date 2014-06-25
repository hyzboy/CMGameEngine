#include"HacMapping.H"
#include<hgl/File.h>
#include<hgl/loginfo.h>
#include<hgl/stream/MemStream.h>
#include<hgl/stream/FileStream.h>
#include<hgl/other.h>
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	HacMapping::HacMapping(const char16_t *folder_name)
	{                              
		if(!folder_name||!*folder_name||IsDirectory(folder_name))
		{
			FolderName=folder_name;
		}
		else
		{
			PutError(u"初始化HAC所映射的子目录<%s>并不存在，请查证！",folder_name);
		}
	}

	HacMapping::~HacMapping()
	{
    	CloseThread();
	}

	bool HacMapping::FindFile(const char16_t *filename)
	{
//		if(FolderName.Length==0)
//		{
//			ErrorHint(u"子目录映射为空，不可查找！");
//			return(false);
//		}

		strcpy(full_filename,FolderName.wc_str());

		if(*filename==u'\\'||*filename==u'/')
			strcat(full_filename,filename);
		else
		{
			strcat(full_filename,HGL_DIRECTORY_SEPARATOR);

			strcat(full_filename,filename);
		}

		return hgl::FileConfirm(full_filename);
	}

	bool HacMapping::FindFile(void *folder,const char16_t *filename)
	{
		if(folder)
		{            
			FolderMapping *fm=(FolderMapping *)folder;

			int n=fm->File.Count;

			while(n--)
				if(fm->File[n]->filename==filename)
					return true;

			return(false);
		}
		else
        	return this->FindFile(filename);
	}
	
	bool HacMapping::LoadFilePart(void *file,uint start,uint length,void *data)
	{
    	if(!file||!data)return(false);

		FileMapping *fm=(FileMapping *)file;

		if(start==0&&length==0)
        	length=fm->filesize;

		if(start+length>fm->filesize)
			return(false);
                             
		UTF16String full_filename;
		FileStream fp;
		int result;

		full_filename=fm->folder->full_filename+HGL_DIRECTORY_SEPARATOR+fm->filename;

		if(!fp.Open(full_filename,fomRead))
			return(false);

		fp.Position=start;
		result=fp.Read(data,length);
		fp.Close();

		return(result==length);
	}

	Stream *HacMapping::LoadFileFrom(void *folder,const char16_t *filename,bool load_to_memory)
	{
		if(folder)
		{           
			FolderMapping *fm=(FolderMapping *)folder;

			UTF16String full_filename;

			full_filename=fm->full_filename+HGL_DIRECTORY_SEPARATOR;
			full_filename+=filename;

			return this->LoadFile(full_filename,load_to_memory);        	
		}
		else
        	return this->LoadFile(filename,load_to_memory);
	}

	bool 	HacMapping::LoadFileFrom(void *folder,const char16_t *filename,void **data,int *size)
	{
		if(folder)
		{                        
			FolderMapping *fm=(FolderMapping *)folder;

			UTF16String full_filename;

			full_filename=fm->full_filename+HGL_DIRECTORY_SEPARATOR;
			full_filename+=filename;

			return this->LoadFile(full_filename,data,size);        
		}
		else
        	return this->LoadFile(filename,data,size);
	}

	Stream *HacMapping::LoadFile(const char16_t *filename,bool load_to_memory)
	{
		if(this->FindFile(filename))
		{
			if(load_to_memory)
			{
				MemStream *ms=new MemStream;

				ms->LoadFromFile(full_filename);

				return(ms);
			}
			else
			{				
				return(OpenFileStream(full_filename));
			}
		}
		else
			return(nullptr);
	}

	bool HacMapping::LoadFile(const char16_t *filename,void **data,int *size)
	{
		if(this->FindFile(filename))
		{
			*size=hgl::LoadFileToMemory(full_filename,data);
			return(true);
		}
		else
			return(false);
	}

/*	bool HacMapping::OpenFile(const char16_t *filename,FileStream **stream,int *offset,int *size)
	{
		if(this->FindFile(filename))
		{
			*stream=new FileStream(full_filename,fomOpenRead);

			*offset=0;
			*size=(*stream)->Size;

			return(true);
		}
		else
			return(false);
	}*/

	void __cdecl HacMappingEnumFile(void *fp,hgl::FileInfo &fi)
	{
		if(fi.directory)return;						//不添加目录

		FolderMapping *folder=(FolderMapping *)fp;

		ObjectList<FileMapping> *file=&(folder->File);

		FileMapping *fm=file->Append();

		fm->folder	=folder;

		fm->filename=fi.name;
		fm->filesize=fi.size;

		fm->filename.LowerCase();
	}

	void *HacMapping::GetFolder(const char16_t *folder_name)
	{
//		if(FolderName.Length==0)
//		{
//			ErrorHint(u"子目录映射为空，不可查找！");
//			return(false);
//		}

		strcpy(full_filename,GetString(hfsStartPath));
		strcat(full_filename,HGL_DIRECTORY_SEPARATOR);

		if(FolderName.Length)
		strcat(full_filename,FolderName.wc_str());

		if(!folder_name||!*folder_name)
		{
		}
		else
		if(*folder_name==HGL_DIRECTORY_SEPARATOR)
			strcat(full_filename,folder_name);
		else
		{
			strcat(full_filename,HGL_DIRECTORY_SEPARATOR);
			strcat(full_filename,folder_name);
		}

        LowerString(full_filename);

		int n=Folder.Count;

		while(n--)
			if(Folder[n]->full_filename==full_filename)
				return Folder[n];

		FolderMapping *folder=Folder.Append();

		folder->full_filename=full_filename;

		EnumFile(full_filename,u"*.*",folder,HacMappingEnumFile);

		return(folder);				
	}

	int HacMapping::GetFileCount(void *folder)
	{
		if(!folder)folder=GetFolder(nullptr);	//RootFolder
		
		FolderMapping *fm=(FolderMapping *)folder;

		return fm->File.Count;
	}

	bool HacMapping::GetFileInfo(void *folder,int index,UTF16String &filename,int &filesize)
	{
		if(index<0)return(false);

		if(!folder)folder=GetFolder(nullptr);

		FolderMapping *fm=(FolderMapping *)folder;

		if(index>=fm->File.Count)return(false);

		filename=fm->File[index]->filename;
		filesize=fm->File[index]->filesize;

		return(true);
	}

	void *HacMapping::GetFile(void *folder,const char16_t *filename,int *filesize)
	{
		if(!filename||!(*filename))return(nullptr);
		      
		if(!folder)folder=GetFolder(nullptr);
                                         
		FolderMapping *fm=(FolderMapping *)folder;
		
		int n=fm->File.Count;
		UTF16String fn=filename;

		fn.LowerCase();

		while(n--)
		{
			if(fm->File[n]->filename==fn)
			{
				if(filesize)
					*filesize=fm->File[n]->filesize;
					
				return fm->File[n];
			}
		}

		return(nullptr);    	
	}

	void *HacMapping::GetFile(const char16_t *filename,int *filesize)
	{               
		if(!filename||!(*filename))return(nullptr);

		const char16_t *end=strrchr(filename,HGL_DIRECTORY_SEPARATOR);
        void *folder=nullptr;

		if(end)
		{
			char16_t *pathname=new char16_t[end-filename+1];

			strcpy(pathname,filename,end-filename);

			folder=GetFolder(pathname);

			return(GetFile(folder,end+1,filesize));
		}
		else
		{
			return(GetFile(nullptr,filename,filesize));
		}
	}
}//namespace hgl

