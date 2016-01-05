#ifndef HGL_DFS_FILE_INCLUDE
#define HGL_DFS_FILE_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/MemBlock.h>
#include<hgl/type/Map.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/dfs/dfsFileAttrib.h>
#include<hgl/dfs/dfs.h>
#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/thread/RWLock.h>
namespace hgl
{
	namespace dfs
	{
		class dfsClientFileConnectPool;
		class dfsClientSaveThread;

		class FileBlock
		{
			int64 filename;
			atom_int64 version;

			char *file_data;
			int64 file_length;

			RWLock lock;

		public:

			HGL_RWLOCK(lock);
			RWLock *GetLock(){return &lock;}

			FileBlock(const int64 fn)
			{
				filename=fn;
				version=-1;
				file_data=0;
				file_length=0;
			}

			~FileBlock()
			{
				Clear();
			}

			void Clear()
			{
				lock.WriteLock();
					hgl_free(file_data);
					file_length=0;
					version=-1;
				lock.WriteUnlock();
			}

			void Update(char *data,int64 size,int64 ver)
			{
				hgl_free(file_data);

				version		=ver;
				file_data	=data;
				file_length	=size;
			}

			const int64 GetFilename()const{return filename;}
			const int64 GetVersion()const{return version;}

			const char *GetData()const{return file_data;}
			const int64 GetLength()const{return file_length;}
		};//struct FileBlock

		class dfsClientFileConnect;

		/**
		 * DFS文件服务客户端
		 */
		class dfsClientFile:public dfsClientConnect													///DFS文件服务客户端
		{
			UTF8String set_name;

			int team_number;

			struct FileTeam
			{
				ThreadMutex lock;
				MapObject<int64,FileBlock> file_list;
			};

			FileTeam *file_team;

			dfsClientFileConnectPool *conn_pool;
			dfsClientSaveThread *save_thread;

		private:

			bool SendPrivateLogin(DataOutputStream *dos);

		public:

			dfsClientFile(const UTF8String &,int=8);
			~dfsClientFile();

			bool Init(const IPAddress *,const UTF8String &,const int64 _node_id=-1);			///<初始化dfs客户端连接

			InputStream *	LoadFile	(const int64);											///<加载一个文件
			OutputStream *	SaveFile	(const int64,const uint32 attrib=0);						///<保存一个文件
			bool			DeleteFile	(const int64);											///<删除文件
		};//class dfsClientFile

		/**
		 * 从dfs加载文件为对象模板函数,对象必须有实现bool Read(DataInputStream *)函数
		 * @param dfs 指定的dfsClientFile指针
		 * @param filename 文件名
		 * @return 创建好的对象指针
		 */
		template<typename T> T *LoadObjectFromDFS(dfsClientFile *dfs,const int64 filename)
		{
			SharedPtr<InputStream> is=dfs->LoadFile(filename);

			if(is->Available()<=0)
				return(nullptr);

			SharedPtr<DataInputStream> dis=new LEDataInputStream(is);

			T *obj=new T;

			if(obj->LoadFromDB(dis))
				return obj;

			delete obj;
			return(nullptr);
		}

		/**
		 * 保存指定对像到dfs
		 * @param dfs 指定的dfsClientFile指针
		 * @param filename 文件名
		 * @param obj 要保存的对象，对象必须有实现bool Write(DataOutputStream *)函数
		 * @return 是否保存成功
		 */
		template<typename T> bool SaveObjectToDFS(dfsClientFile *dfs,const int64 filename,const T *obj)
		{
			if(!obj)
				return(false);

			SharedPtr<OutputStream> os=dfs->SaveFile(filename);
			SharedPtr<DataOutputStream> dos=new LEDataOutputStream(os);

			if(!obj->SaveToDB(dos))
				return(false);

			os->Close();
			return(true);
		}

		/**
		 * DFS文件读取访问
		 */
		class dfsLoadFile
		{
			InputStream *is;

		public:

			dfsLoadFile(dfsClientFile *dfs,const int64 filename)
			{
				is=dfs->LoadFile(filename);
			}

			~dfsLoadFile()
			{
				if(is)delete is;
			}

			operator InputStream *(){return is;}
			InputStream *operator ->(){return is;}
		};//class dfsLoadFile

		/**
		 * DFS文件保存访问
		 */
		class dfsSaveFile
		{
			OutputStream *os;

		public:

			dfsSaveFile(dfsClientFile *dfs,const int64 filename,const uint32 attrib)
			{
				os=dfs->SaveFile(filename,attrib);
			}

			~dfsSaveFile()
			{
				if(os)
					delete os;
			}

			operator OutputStream *(){return os;}
			OutputStream *operator ->(){return os;}
		};//class dfsSaveFile
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_FILE_INCLUDE
