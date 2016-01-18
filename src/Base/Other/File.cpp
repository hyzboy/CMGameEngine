#include <hgl/File.h>
#include <hgl/LogInfo.h>
#include <hgl/Other.h>
#include <hgl/io/FileInputStream.h>
#include <hgl/io/FileOutputStream.h>

#if HGL_OS == HGL_OS_Windows
	#include <windows.h>
#else
	#include <unistd.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <dirent.h>
#endif//HGL_OS == HGL_OS_Windows

#include <sys/types.h>
#include <string.h>

namespace hgl
{
	const int FILE_PROC_BUF_SIZE=HGL_SIZE_1MB;

	/**
	 * 组合路径名与文件名
	 * @param pathname 路径名
	 * @param filename 文件名
	 * @return 完整路径文件名
	 */
	void MergeFilename(OSString &fullname,const OSString &pathname,const OSString &filename)
	{
		fullname=(	pathname.GetEndChar()==HGL_DIRECTORY_SEPARATOR?
					pathname+filename:
					pathname+HGL_DIRECTORY_SEPARATOR+filename);
	}

	/**
	* 复制一个文件
	* @param sourcename 源文件名
	* @param targetname 目标文件名
	* @return 文件是否复制成功
	*/
	bool FileCopy(const OSString &sourcename,const OSString &targetname)
	{
#if HGL_OS == HGL_OS_Windows
		return(::CopyFileW(sourcename,targetname,false));
#else
		io::FileInputStream fis;
		io::FileOutputStream fos;

		if(!fis.Open(sourcename))return(false);
		if(!fos.Create(targetname))return(false);

		size_t buf_size=FILE_PROC_BUF_SIZE;
		size_t length=fis.GetSize();

		size_t cur;

		if(buf_size>length)
			buf_size=length;

		SharedPtr<char> buf=new char[buf_size];

		while(length)
		{
			if(length>buf_size)
				cur=buf_size;
			else
				cur=length;

			if(fis.Read(buf,cur))
				if(fos.Write(buf,cur))
				{
					length-=cur;
					continue;
				}

			return(false);
		}

		return(true);
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 删除一个文件
	* @param filename 文件名
	* @return 文件是否成功删除
	*/
	bool FileDelete(const OSString &filename)
	{
#if HGL_OS == HGL_OS_Windows
		return(::DeleteFileW(filename));
#else
		return(unlink(filename.c_str())==0);
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 移动一个文件
	* @param sourcename 源文件名
	* @param targetname 目标文件名
	* @return 文件是否移动成功
	*/
	bool FileMove(const OSString &sourcename,const OSString &targetname)
	{
#if HGL_OS == HGL_OS_Windows
		return(::MoveFileW(sourcename,targetname));
#else
		if(FileCopy(sourcename,targetname))
			return FileDelete(sourcename);

		return(false);
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 修改文件名
	* @param oldname 旧的文件名
	* @param newname 新的文件名
	* @return 文件名是否修改成功
	*/
	bool FileRename(const OSString &oldname,const OSString &newname)
	{
#if HGL_OS == HGL_OS_Windows
		return(::MoveFileW(oldname,newname));
#else
		return(rename(oldname.c_str(),
					  newname.c_str())==0);
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 确认文件是否存在
	* @param filename 要查找的文件名称
	* @return 这个文件是否存在
	*/
	bool FileConfirm(const OSString &filename)
	{
#if HGL_OS == HGL_OS_Windows
		WIN32_FIND_DATAW wfd;
		HANDLE hFind;

		hFind=FindFirstFileW(filename, &wfd);

		if(hFind==INVALID_HANDLE_VALUE)
			return(false);

		FindClose(hFind);
		return(true);
#else
		return access(filename.c_str(),F_OK)>=0;
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 比较两个文件是否一样
	* @param filename1 第一个文件的名称
	* @param filename2 第二个文件的名称
	* @return 文件是否一样
	*/
	bool FileComp(const OSString &filename1,const OSString &filename2)
	{
		io::FileInputStream fp1,fp2;
		int64 fs1,fs2;

		if(!fp1.Open(filename1))return(false);
		if(!fp2.Open(filename2))return(false);

		fs1=fp1.GetSize();
		fs2=fp2.GetSize();

		if(fs1!=fs2)
			return(false);

		int64 pos=0,size;
		char *data1,*data2;

		data1=new char[FILE_PROC_BUF_SIZE];
		data2=new char[FILE_PROC_BUF_SIZE];

		while(pos<fs1)
		{
			size=FILE_PROC_BUF_SIZE;
			if(pos+size>fs1)size=fs1-pos;

			fp1.Read(data1,size);
			fp2.Read(data2,size);

			if(memcmp(data1,data2,size)==0)
			{
				pos+=size;
				continue;
			}
			else
			{
				delete[] data1;
				delete[] data2;
				return(false);
			}
		};

		delete[] data1;
		delete[] data2;
		return(true);
	}

	/**
	* 检测文件是否可读
	* @param filename 文件名
	*/
	bool FileCanRead(const OSString &filename)
	{
#if HGL_OS == HGL_OS_Windows
		const DWORD attr=GetFileAttributesW(filename);

		if(attr==INVALID_FILE_ATTRIBUTES)
			return(false);

		if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))	//不是个文件
			return(false);

		if(attr&FILE_ATTRIBUTE_OFFLINE)
			return(false);

		return(true);
#else
		return access(filename.c_str(),R_OK)>=0;
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 检测文件是否可写
	* @param filename 文件名
	*/
	bool FileCanWrite(const OSString &filename)
	{
#if HGL_OS == HGL_OS_Windows
		const DWORD attr=GetFileAttributesW(filename);

		if(attr==INVALID_FILE_ATTRIBUTES)
			return(false);

		if(attr&(FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY))	//不是个文件
			return(false);

		if(attr&FILE_ATTRIBUTE_OFFLINE)
			return(false);

		return (attr&FILE_ATTRIBUTE_READONLY)?false:true;
#else
		return access(filename.c_str(),W_OK)>=0;
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	 * 检测文件是否可执行
	* @param filename 文件名
	 */
	bool FileCanExec(const OSString &filename)
	{
#if HGL_OS==HGL_OS_Windows
		int index = filename.FindRightChar('.');

		if (index == -1)return(false);

		if (index > filename.Length() - 4)
			return(false);

		const os_char *ext = filename.c_str() + index + 1;

		if (!ext)return(false);

		if (stricmp(ext, "exe") == 0)return(true);
		if (stricmp(ext, "com") == 0)return(true);
		if (stricmp(ext, "bat") == 0)return(true);
		if (stricmp(ext, "msi") == 0)return(true);
		if (stricmp(ext, "msp") == 0)return(true);

		return(false);
#else
		return access(filename.c_str(),X_OK)>=0;
#endif//HGL_OS==HGL_OS_Windows
	}

	/**
	* 加载一个文件到内存,文件数据请自己delete[]掉
	* @param filename 要载入的文件名称
	* @param buf 用来保存数据的内存指针的指针
	* @return 文件长度
	*/
	int64 LoadFileToMemory(const OSString &filename,void **buf)
	{
		io::FileInputStream fs;

		if(!fs.Open(filename))
			return(-1);

		const int64 size=fs.GetSize();

		char *fb=new char[size];

		if(fs.Read(fb,size)==size)
		{
			*buf=fb;
			return(size);
		}

		delete[] fb;
		return(-1);
	}

	/**
	* 保存一块内存成文件
	* @param filename 要保存的文件名称
	* @param buf 要保存的内存指针
	* @param size 要保存的内存数据长度
	* @return 成功写入的字节数
	* @return -1 失败
	*/
	int64 SaveMemoryToFile(const OSString &filename,void *buf,int64 size)
	{
		io::FileOutputStream fs;

		if(!fs.CreateTrunc(filename))
			return(-1);

		return fs.Write(buf,size);
	}

	int ParseAnsi(u16char **buf,uchar *raw_str,int byte_size,const CharSet &cs)
	{
		return ansi_to_utf16(cs,buf,(char *)raw_str,byte_size);
	}

	//转换utf16/32
	template<typename T>
	int ParseUTF(u16char **buf,void *raw_str,int byte_size,bool is_little_endian)
	{
		const int size=byte_size/sizeof(T);

		if(size==0)return(0);

		*buf=new u16char[size];

		if(is_little_endian)
			LittleToCurrentEndian(*buf,(T *)raw_str,size);
		else
			BigToCurrentEndian(*buf,(T *)raw_str,size);

		return size;
	}

	int ParseUTF8(u16char **wide_str,const uchar *utf8_str,int size)
	{
		int wlen;

		*wide_str=u8_to_u16((char *)utf8_str,size,wlen);

		return wlen;
	}

	int64 LoadTxtToMemory(io::InputStream *str,const int64 str_length,u16char **buf,const CharSet &cs)
	{
		if(!str||!buf)
			return(-1);

		if(str_length<=0)
			return(str_length);

		SharedArray<uchar> raw_buf=new uchar[str_length];

		str->Read(raw_buf,str_length);

		if(str_length>=2)
		{
			if(raw_buf[0]==0xFF&&raw_buf[1]==0xFE)									//LE
			{
				if(str_length>=4)
					if(raw_buf[2]==0&&raw_buf[3]==0)
						return ParseUTF<uint32>(buf,raw_buf+4,str_length-4,true);	//utf32-le

				return ParseUTF<uint16>(buf,raw_buf+2,str_length-2,true);			//utf16-le
			}
			else
			if(raw_buf[0]==0&&raw_buf[1]==0&&raw_buf[2]==0xFE&&raw_buf[3]==0xFF)
			{
				return ParseUTF<uint32>(buf,raw_buf+4,str_length-4,false);			//utf32-be
			}
			else
			if(raw_buf[0]==0xFE&&raw_buf[1]==0xFF)
			{
				return ParseUTF<uint16>(buf,raw_buf+2,str_length-2,false);			//utf16-be
			}
			else
			if(raw_buf[0]==0xEF&&raw_buf[1]==0xBB&&raw_buf[2]==0xBF)
			{
				return ParseUTF8(buf,raw_buf+3,str_length-3);						//utf8
			}
		}

		return ParseAnsi(buf,raw_buf,str_length,cs);
	}

	/**
	* 加载一个文本文件到内存
	* @param filename 文件名
	* @param buf 文本缓冲区
	* @param cs 如果为ansi，将使用的转换代码页/字符集
	* @return 文本长度
	*/
	int64 LoadTxtToMemory(const OSString &filename,u16char **buf,const CharSet &cs)
	{
		io::FileInputStream fs;

		if(!fs.Open(filename))
			return(-1);

		return LoadTxtToMemory(&fs,fs.GetSize(),buf,cs);
	}

	/**
	* 判断当前名称是否是一个目录
	* @param name 名称
	*/
	bool IsDirectory(const os_char *name)
	{
#if HGL_OS == HGL_OS_Windows
		DWORD attrib = GetFileAttributesW(name);

		if (attrib == INVALID_FILE_ATTRIBUTES)
			return(false);

		return attrib&FILE_ATTRIBUTE_DIRECTORY;
#else
		struct_stat64 buf;

		memset(&buf,0,sizeof(struct_stat64));

		if(lstat64(name,&buf)==-1)
			return(false);				//错误，有可能是不能访问

		return S_ISDIR(buf.st_mode);
#endif//HGL_OS == HGL_OS_Windows
	}

	inline bool MakeDirectory(const OSString &name)
	{
#if HGL_OS == HGL_OS_Windows
		if(::CreateDirectoryW(name,nullptr))return(true);
#else
		if(!mkdir(name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))return(true);
#endif//HGL_OS == HGL_OS_Windows

		LOG_PROBLEM(OS_TEXT("创建目录<")+name+OS_TEXT(">失败！"));
		return(false);
	}

	/**
	* 创建一个子目录,该函数可自动创建多级目录。
	* @param dirname 目录名称
	* @return 目录是否创建成功
	*/
	bool MakePath(const OSString &dirname)
	{
		const os_char directory_separator=HGL_DIRECTORY_SEPARATOR;
		os_char *p;

		os_char str[HGL_MAX_PATH];
		os_char *sp;

#if HGL_OS == HGL_OS_Windows
		strcpy(str,HGL_MAX_PATH,dirname.c_str());

		if(str[1]==OS_TEXT(':'))sp=str+3;
				   else sp=str;
#else
		strcpy(str,HGL_MAX_PATH,dirname.c_str());

		sp=str;
#endif//HGL_OS == HGL_OS_Windows

		while(1)
		{
			p=hgl::strchr(sp+1,directory_separator);				// unix下有可能第一个字符就是分隔符，所以必须用sp+1开始查找

			if(p)
				*p=0;

            if(*sp==0)
                return(true);

			if(!IsDirectory(str))//没有找到
				if(!MakeDirectory(str))
					return(false);

            if(p)
                *p++=directory_separator;
            else
                return(true);

            sp=p;
		}
	}

	/**
	* 删除一个子目录
	* @param dirname 目录名称
	* @return 目录是否删除成功
	*/
	bool DeletePath(const OSString &name)
	{
#if HGL_OS == HGL_OS_Windows
		return(RemoveDirectoryW(name));
#else
		return rmdir(name.c_str());
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	* 删除一个子目录,包含所有文件和子目录
	* @param dirname 目录名称
	* @return 目录是否删除成功
	*/
	void DeleteTree(const OSString &dirname)
	{
	}

	/**
	* 取得当前所在目录<br>
	*/
	bool GetCurrentPath(OSString &path)
	{
#if HGL_OS == HGL_OS_Windows
		int len;
		u16char *dir;

		len=GetCurrentDirectoryW(0,nullptr);

		if(len==0)
			return(nullptr);

		dir=new u16char[len+1];

		if(GetCurrentDirectoryW(len,dir))
		{
			if(len==3&&dir[1]==OS_TEXT(':'))
				len=2;        //如果是"C:\"这种情况，去掉"\"

			dir[len]=0;

			path.Set(dir,len,true);
			return(true);
		}

		delete[] dir;
		return(false);
#else
		const int len=pathconf(".",_PC_PATH_MAX);

		if(len<=0)return(false);

		char *str=new char[len];

		memset(str,0,len);

		if(!getcwd(str,len))
		{
			delete[] str;
			return(false);
		}

		path.Set(str,strlen(str),true);
		return(true);
#endif//HGL_OS == HGL_OS_Windows
	}

	/**
	 * 取得当前程序完整路径名称
	 */
	bool GetCurrentProgram(OSString &result)
	{
		os_char *path=new os_char[HGL_MAX_PATH];

#if HGL_OS == HGL_OS_Windows
		GetModuleFileNameW(nullptr,path,HGL_MAX_PATH);

		result = path;
		delete[] path;
#else
		const int len=readlink("/proc/self/exe",path,HGL_MAX_PATH);

		if(len<=0)
		{
			delete[] path;
			return(false);
		}

		path[len]=0;
		result.Set(path,len,true);
#endif//HGL_OS == HGL_OS_Windows
		return(true);
	}

	/**
	* 读取文件的一部分到内存
	* @param filename 文件名
	* @param offset 起始地址
	* @param length 读取长度
	* @param buf 数据存放地址,为NULL表示要求函数分配
	* @return NULL 读取失败
	* @return 读取出来的数据存放的地址
	*/
	void *LoadFileToMemory(const OSString &filename,int64 offset,void *buf,int64 length)
	{
		if(!filename||!*filename||length==0)
			return(nullptr);

		io::FileInputStream fs;

		if(!fs.Open(filename))
			return(nullptr);

		const int64 file_length=fs.GetSize();

		if(offset+length>file_length)
		{
			LOG_PROBLEM(OS_TEXT("读取文件<")+filename+OS_TEXT("><")+OSString(offset)+OS_TEXT(",")+OSString(length)+OS_TEXT(">超出了范围，文件长度为<")+OSString(file_length));
			return(nullptr);
		}

		char *fb;

		if(buf)
			fb=(char *)buf;
		else
			fb=new char[length];

		if(fs.Read(offset,fb,length)==length)
		{
			LOG_INFO(OS_TEXT("加载文件<")+filename+OS_TEXT("><")+OSString(offset)+OS_TEXT(",")+OSString(length)+OS_TEXT(">到缓冲区成功."));

			return(buf);
		}
		else
		{
			if(fb!=buf)
				delete[] fb;

			return(nullptr);
		}
	}

	/**
	* 保存内存中的数据到文件中
	* @param filename 文件名
	* @param offset 起始地址
	* @param length 数据长度
	* @param data 数据长度
	* @return 是否成功
	*/
	bool SaveMemoryToFile(const OSString &filename,int64 offset,const void *data,int64 length)
	{
		io::FileOutputStream fs;

		if(!fs.Open(filename))
			return(false);

		return fs.Write(offset,data,length);
	}

	bool GetFileInfo(const OSString &filename,struct FileInfo &fi)
	{
		struct_stat64 file_state;

		memset(&file_state,0,sizeof(struct_stat64));

		if(lstat64(filename.c_str(),&file_state)==-1)
			return(false);

		memset(&fi,0,sizeof(FileInfo));

		if(file_state.st_mode&S_IFREG)
		{
			fi.is_file=true;
			fi.is_directory=false;
		}

		if(file_state.st_mode&S_IFDIR)
		{
			fi.is_file=false;
			fi.is_directory=true;
		}

		fi.size=file_state.st_size;
		return(true);
	}

	void AddToList(List<FileInfo> *lfi,hgl::FileInfo &fi)
	{
		lfi->Add(fi);
	}

	List<FileInfo> GetListFiles(const OSString &folder_name,bool proc_folder,bool proc_file,bool sub_folder)
	{
		List<FileInfo> fi_list;
		EnumFile(folder_name,&fi_list,folder_name,proc_file,sub_folder,(void (*)(void *,hgl::FileInfo &))AddToList);
		return fi_list;
	}

#if HGL_OS == HGL_OS_Windows
	/**
	* 枚举一个目录内的所有文件
	* @param folder_name 目录名称
	* @param find_name 查找用名称
	* @param data 自定义回传数据
	* @param proc_folder 处理目录
	* @param proc_file 处理文件
	* @param sub_folder 是否处理子目录
	* @param func 回调函数
	* @return 查找到文件数据,-1表示失败
	*/
	int EnumFile(const OSString &folder_name,const OSString &find_name,void *data,bool proc_folder,bool proc_file,bool sub_folder,EnumFileFunc func)
	{
		OSString full_name;
		int count=0;

		if(!func)return(-1);

		if(!find_name||!(*find_name))
			return(-1);

		if(!folder_name||!(*folder_name))
		{
			full_name=find_name;
		}
		else
		{
			full_name=folder_name;

			if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
                full_name+=HGL_DIRECTORY_SEPARATOR;

			full_name+=find_name;
		}

		WIN32_FIND_DATAW FindFileData;
		HANDLE hFind;

		hFind = FindFirstFileW(full_name, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return(-1);

		do
		{
			if(strcmp(FindFileData.cFileName,OS_TEXT("."))==0
			|| strcmp(FindFileData.cFileName,OS_TEXT("..")) == 0)
			{
				continue;
			}

			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if(proc_folder)
				{
					if(sub_folder)
					{
						OSString child_name=folder_name;

						if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
							child_name+=HGL_DIRECTORY_SEPARATOR;

						child_name+=FindFileData.cFileName;
						count+=EnumFile(child_name,find_name,data,proc_folder,proc_file,true,func);
					}
				}
				else
					continue;
			}
			else
			{
				if(!proc_file)continue;
			}

			count++;

			FileInfo fi;
			memset(&fi,0,sizeof(FileInfo));

			strcpy(fi.name,HGL_MAX_PATH,FindFileData.cFileName);

			if(!folder_name||!(*folder_name))
			{
            	strcpy(fi.fullname, HGL_MAX_PATH, fi.name);
			}
			else
			{
				strcpy(fi.fullname, HGL_MAX_PATH, folder_name);

				if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
					strcat(fi.fullname, HGL_MAX_PATH, HGL_DIRECTORY_SEPARATOR);

				const int rp = find_name.FindChar(HGL_DIRECTORY_SEPARATOR);//防止查询名称内仍有路径

				if(rp!=-1)
					strcat(fi.fullname, HGL_MAX_PATH, find_name.c_str(),rp);

				strcat(fi.fullname, HGL_MAX_PATH, fi.name, HGL_MAX_PATH);
			}
			fi.size_high=FindFileData.nFileSizeHigh;
			fi.size_low =FindFileData.nFileSizeLow;

			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				fi.is_file=false;
				fi.is_directory=true;
			}
			else
			{
				fi.is_file=true;
				fi.is_directory=false;
			}

			fi.can_read=true;
			fi.can_write=!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_READONLY);

			func(data,fi);
		}
		while(FindNextFileW(hFind, &FindFileData));

		FindClose(hFind);

		return(count);
	}
#else
	/**
	* 枚举一个目录内的所有文件
	* @param folder_name 目录名称
	* @param data 自定义回传数据
	* @param proc_folder 处理目录
	* @param proc_file 处理文件
	* @param sub_folder 是否处理子目录
	* @param func 回调函数
	* @return 查找到文件数据,-1表示失败
	*/
	int EnumFile(const OSString &folder_name,void *data,bool proc_folder,bool proc_file,bool sub_folder,EnumFileFunc func)
	{
		OSString fullname;
		int count=0;

		if(!func)return(-1);

		if(folder_name.IsEmpty())
        {
            fullname='.';
        }
		else
        {
			fullname=folder_name;
        }

		DIR *dir;
		struct_dirent64 *entry;
		struct_stat64 statbuf;
		FileInfo fi;
		char path_buf[HGL_MAX_PATH]={0};
		getcwd(path_buf,HGL_MAX_PATH);

		chdir(fullname);
		if((dir = opendir(fullname)) == NULL)
			return(-1);
		if((entry = readdir64(dir)) == NULL)
			return(-1);

		do
		{
			memset(&statbuf,0,sizeof(struct_stat64));
			memset(&fi,0,sizeof(FileInfo));

			if(strcmp(entry->d_name,".")==0
			 ||strcmp(entry->d_name,"..")==0)
			{
				continue;
			}

			if(lstat64(entry->d_name,&statbuf)==-1)
				continue;

			fi.size=statbuf.st_size;

			if(S_ISDIR(statbuf.st_mode))
			{
				fi.is_file=false;
				fi.is_directory=true;
			}
			else
			{
				fi.is_file=true;
				fi.is_directory=false;
			}

			fi.can_read	=statbuf.st_mode&S_IROTH;
			fi.can_write=statbuf.st_mode&S_IWOTH;
			if(S_ISDIR(statbuf.st_mode))
			{
				if(proc_folder)
				{
					if(sub_folder)
					{
						char child_name[HGL_MAX_PATH];

						strcpy(child_name,HGL_MAX_PATH,folder_name);

						if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
							strcat(child_name,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);

						const int child_len=strlen(child_name);

						strcpy(child_name+child_len,HGL_MAX_PATH-child_len,entry->d_name);

						count+=EnumFile(child_name,data,proc_folder,proc_file,true,func);
					}
				}
				else
					continue;
			}
			else
			{
				if(!proc_file)continue;
			}

			count++;

			strcpy(fi.name,HGL_MAX_PATH,entry->d_name);

			if(!folder_name||!(*folder_name))
			{
            	strcpy(fi.fullname,HGL_MAX_PATH,fi.name);
			}
			else
			{
				strcpy(fi.fullname,HGL_MAX_PATH,folder_name);

				if(folder_name.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
					strcat(fi.fullname,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);

				strcat(fi.fullname,HGL_MAX_PATH,fi.name,HGL_MAX_PATH);
			}

			func(data,fi);
		}
		while((entry=readdir64(dir)));

		closedir(dir);

		chdir(path_buf);
		return(count);
	}
#endif//HGL_OS == HGL_OS_Windows

	/**
	* 枚举一个目录内的所有文件
	* @param folder_name 目录名称
	* @param find_name 查找用名称
	* @param data 自定义回传数据
	* @param func 回调函数
	* @return 查找到文件数据,-1表示失败
	*/
#if HGL_OS == HGL_OS_Windows
	int EnumFile(const u16char *folder_name,const u16char *find_name,void *data,EnumFileFunc func)
	{
		return EnumFile(folder_name,find_name,data,true,true,false,func);
	}
#endif

#if HGL_OS == HGL_OS_Windows
	/**
	* 枚举当前计算机所有卷
	* @param data 用户自定义回传信息
	* @param func 回调函数
	* @param check_cd 检测光盘
	* @return 查找到的卷数量，-1表示失败
	*/
	int EnumVolume(void *data,void (__cdecl *func)(void *,hgl::VolumeInfo &),bool check_removable,bool check_remote,bool check_cd)
	{
		HANDLE handle;
		u16char volume_name[HGL_MAX_PATH+1];
		u16char path_name[HGL_MAX_PATH];
		int count=0;

		handle=FindFirstVolumeW(volume_name,HGL_MAX_PATH);

		if(handle==INVALID_HANDLE_VALUE)return(-1);

		do
		{
			hgl::VolumeInfo vi;

			memset(&vi,0,sizeof(hgl::VolumeInfo));

			DWORD length;

			GetVolumePathNamesForVolumeNameW(volume_name,path_name,HGL_MAX_PATH,&length);				//这个函数要win xp/2003才能用

			path_name[length]=0;

			strcpy(vi.name, HGL_MAX_PATH,volume_name);
			strcpy(vi.path, HGL_MAX_PATH,path_name);

			UINT type=GetDriveTypeW(path_name);

			if(type==DRIVE_REMOVABLE){if(!check_removable)continue;	vi.driver_type=hgl::VolumeInfo::dtRemovable;}else
			if(type==DRIVE_FIXED	){								vi.driver_type=hgl::VolumeInfo::dtFixed;	}else
			if(type==DRIVE_REMOTE	){if(!check_remote	 )continue;	vi.driver_type=hgl::VolumeInfo::dtRemote;	}else
			if(type==DRIVE_RAMDISK	){								vi.driver_type=hgl::VolumeInfo::dtRamDisk;	}else
			if(type==DRIVE_CDROM	){if(!check_cd		 )continue;	vi.driver_type=hgl::VolumeInfo::dtCDROM;	}else
				vi.driver_type=hgl::VolumeInfo::dtNone;

			uint32 file_system_flags;

			if(GetVolumeInformationW(path_name,
									vi.volume_label,
									255,
									(unsigned long *)&vi.serial,
									(unsigned long *)&vi.filename_max_length,
									(unsigned long *)&file_system_flags,
									vi.file_system,
									255))
			{
				vi.unicode=file_system_flags&FILE_UNICODE_ON_DISK;
			}
			else
				LOG_PROBLEM(U16_TEXT("取得卷<") + UTF16String(path_name) + U16_TEXT(">信息失败！Windows错误编号: ") + UTF16String((uint)GetLastError()));

			if(GetDiskFreeSpaceExW(	path_name,
									(ULARGE_INTEGER *)&vi.available_space,
									(ULARGE_INTEGER *)&vi.total_space,
									(ULARGE_INTEGER *)&vi.free_space))
			{
				func(data,vi);
			}
			else
				LOG_PROBLEM(U16_TEXT("取得驱动器<") + UTF16String(path_name) + U16_TEXT(">容量数据失败！"));

			count++;

		}while(FindNextVolumeW(handle,volume_name,HGL_MAX_PATH));

		FindVolumeClose(handle);

		return(count);
	}
#endif//HGL_OS == HGL_OS_Windows
}//namespace hgl
