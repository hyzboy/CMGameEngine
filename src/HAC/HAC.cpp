#include<hgl/HAC.h>
#include<hgl/File.h>
#include<hgl/thread/Thread.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/LogInfo.h>
#include<hgl/thread/Semaphore.h>

#include"Hac3.h"
#include"HacMapping.h"

// HAC 4 开发要素
//  1.支持64位文件索引，支持超过4G的文件包
//  2.支持文件索引区压缩和加密
//	3.不再支持文件压缩和加密

namespace hgl
{
	const char HAC3Flag[]=		"HAC-3\x1A";

	/**
	* 打开一个.Hac文件或是指定一个目录，创建并返回一个HAC3包裹文件支持类。请注意它支持HAC-3的.HAC文件，并不支持HAC-2的HAC文件
	* @param filename 包裹文件名称或是目录名称
	* @return HAC3包裹文件支持类,返回NULL表示失败
	*/
	HAC *OpenHAC(const UTF16String &filename)
	{
		HAC *hac=nullptr;

		if(!filename||!*filename||IsDirectory(filename))                   //是目录
		{
			hac=new HacMapping(filename);       	//使用真实目录映射
		}
		else
		{
			FileStream *fp=new FileStream;

			if(fp->Open(filename,fomOpenRead)==false)
			{
				PutError(u"打开HAC-3包裹文件<%s>失败！",filename);

				delete fp;

				return(nullptr);
			}
			else
			{
				char flag[8];

				fp->Read(flag,8);

				if(memcmp(flag,HAC3Flag,6)==0)
				{
					PutInfo(u"打开HAC包<%s>完成！",filename);
					hac=new HAC3(fp);
				}
				else
				{
					PutError(u"无法支持或已损坏的HAC文件<%s>",filename);

					delete fp;
					return(nullptr);
				}
			}
		}

		hac->sem=new Semaphore(1024);

		hac->Start();

		return(hac);
	}

	HAC::HAC()
	{
    	quit_lock.Lock();

		sem=nullptr;

		all_clear=false;
	}

	HAC::~HAC()
	{
		load_queue.Clear();
		fin_queue.Clear();

		SAFE_CLEAR(sem);
	}

	void HAC::CloseThread()
	{
		quit_lock.Unlock();

		Wait();
	}

	bool HAC::Execute()
	{
		while(true)
		{
			if(quit_lock.TryLock())		//退出
				break;

			if(!sem->Acquire(1))				//等待1秒钟
			{
				WaitTime(1);
				continue;
			}

			HacLoadItem *hli=nullptr;

			Lock();

				if(all_clear)
				{
					all_clear=false;

					load_queue.Clear();
					fin_queue.Clear();
				}

				if(load_queue.Count>0)
					hli=load_queue[0];

			Unlock();

			if(!hli)continue;

			if(!hli->file)
			{
				int64 filelength;

				hli->file=this->GetFile(hli->folder,hli->filename,&filelength);

				if(hli->start==0&&hli->length==0)
                	hli->length=filelength;
			}

			if(hli->file)
			{
				hli->ChangeStatus(hsRead);

				if(!hli->join_data)
					hli->data=new char[hli->length];

				if(LoadFilePart(hli->file,hli->start,hli->length,hli->data))
					hli->ChangeStatus(hsEnd);
				else
				{
					if(!hli->join_data)
					{
						delete[] hli->data;
						hli->data=0;
					}

					hli->ChangeStatus(hsReadError);
				}
			}
			else
				hli->ChangeStatus(hsOpenError);

			Lock();

				load_queue.UnlinkByData(hli);

				fin_queue.Add(hli);

			Unlock();
		}

		return(false);
	}

	void *HAC::_LoadAcquire(void *path_pointer,const UTF16String &filename,void *file_pointer,void *data,int64 start,int64 length)
	{
		HacLoadItem *hli=new HacLoadItem;

		if(!path_pointer)
		{
			const u16char *p=filename.strrchr(HGL_DIRECTORY_SEPARATOR);

			if(p)
			{
				UTF16String pathname(filename,p-filename);

				path_pointer=GetFolder(pathname);

				int64 file_length;

				file_pointer=GetFile(path_pointer,p+1,&file_length);

				if(start==0&&length==0)
					length=file_length;

				filename=nullptr;
			}
		}

		hli->folder		=path_pointer;
		hli->filename	=filename;
		hli->file		=file_pointer;
		hli->start		=start;
		hli->length		=length;
		hli->status		=hsNone;

		if(data)
		{
			hli->data		=data;
			hli->join_data	=true;
		}
		else
		{
        	//分配内存也算是耗时的工作了，扔到另一个线程去做
			hli->join_data	=false;
		}

		Lock();
			load_queue.Add(hli);
		Unlock();

		sem->Release(1);	//释放一个信号

		return hli;
	}

	HacStatus HAC::GetStatus(void *file_pointer)
	{
		if(!file_pointer)
        	return hsNotFind;

		HacLoadItem *hli=(HacLoadItem *)file_pointer;

		return hli->status;		//仅仅是读取不需要锁定排斥
	}

	int HAC::GetSize(void *file_pointer)
	{
		if(!file_pointer)
			return -1;

		HacLoadItem *hli=(HacLoadItem *)file_pointer;

		return hli->length;
	}

	void *HAC::GetData(void *file_pointer)
	{
		if(!file_pointer)
			return NULL;

		HacLoadItem *hli=(HacLoadItem *)file_pointer;

		if(hli->status==hsEnd)
			return hli->data;
		else
        	return NULL;
	}

	void HAC::Clear(void *file_pointer)
	{
		if(!file_pointer)return;

		HacLoadItem *hli=(HacLoadItem *)file_pointer;

		hli->lock.Lock();

		Lock();

			if(!fin_queue.DeleteByData(hli))
				load_queue.DeleteByData(hli);

		Unlock();
	}

	void HAC::Clear()
	{
		Lock();
			all_clear=true;
		Unlock();
	}
}//namespace hgl
