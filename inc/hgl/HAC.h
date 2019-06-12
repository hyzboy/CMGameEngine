#ifndef HGL_HAC_INCLUDE
#define HGL_HAC_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/type/List.h>
#include<hgl/type/BaseString.h>
#include<hgl/CompOperator.h>
namespace hgl
{
    class Semaphore;
    class Stream;
    class MemStream;
    class FileStream;

    enum HacStatus //: uint
    {
        hsNone=0,       //未知

        hsNotFind,      //没有找到
        hsOpenError,    //打开错误
        hsRead,         //正在读取
        hsReadError,    //读取错误

        hsEnd           //完成结束
    };

    /**
    * HAC包裹文件支持类,可以设定一个子目录以HAC方式访问,用以方便开发。<br>
    * 在制作发行版时直接将整个目录打包为.HAC即可.<br>
    * 另外，它不支持HAC-2的HAC文件。<br>
    */
    class HAC:public Thread                                                                         ///HAC-3包裹文件支持类
    {
        friend HAC *OpenHAC(const u16char *);

    private:

        struct HacLoadItem
        {
            ThreadMutex lock;

            void *folder;
            UTF16String filename;

            void *file;
            int64 start;
            int64 length;

            bool join_data;

            void *data;

            HacStatus status;

            HacLoadItem()
            {
                data=nullptr;
            }

            ~HacLoadItem()
            {
                lock.Unlock();

                if(!join_data&&data)
                    delete[] data;
            }

            void ChangeStatus(HacStatus hs)
            {
                lock.Lock();
                status=hs;
                lock.Unlock();
            }
        };//struct HacLoadItem

        Semaphore *sem;
        ThreadMutex quit_lock;

        bool all_clear;

        ObjectList<HacLoadItem> load_queue;     //加载队列
        ObjectList<HacLoadItem> fin_queue;      //完成队列

        bool Execute();

        void *_LoadAcquire(void *,const UTF16String &,void *,void *,int64,int64);

        virtual bool LoadFilePart(void *,int64,int64,void *)=0;

    protected:

        void CloseThread();

    public:

        HAC();                                                                                      ///<本类构造函数
        virtual ~HAC();                                                                             ///<本类析构函数

        /**
        * 查找一个文件，确定其是否存在
        * @param filename 要查找的文件名称
        * @return 文件是否存在
        */
        virtual bool            FindFile(const UTF16String &filename)=0;                                        ///<查找一个文件

        /**
        * 加载一个文件到流中，并返回这个流
        * @param filename 要加载的文件名称
        * @param load_to_memory 是否加载到内存
        * @return 保存文件数据的流，如果加载失败返回NULL
        */
        virtual InputStream *   LoadFile(const UTF16String &filename,bool load_to_memory=false)=0;          ///<加载一个文件到流

        /**
        * 加载一个文件到内存中
        * @param filename 要加载的文件名称
        * @param data 要用来保存数据的内存块
        * @param size 内存块的大小，以及加载成功后文件的大小
        * @return 是否加载成功
        */
        virtual bool            LoadFile(const UTF16String &filename,void **data,int64 *size)=0;                ///<加载一个文件到指定内存块

    public:

        /**
        * 取得一个目录
        * @param pathname 目录名称
        * @return 目录指针,返回NULL表示失败
        */
        virtual void *          GetFolder(const UTF16String &pathname)=0;                                   ///<取得一个目录

        /**
        * 取得目录内的文件数量
        * @param path_pointer 目录指针
        * @return 文件数量
        */
        virtual int64           GetFileCount(void *path_pointer)=0;                                         ///<取得目录下的文件数量

        /**
        * 取得目录内指定的第index个文件的信息
        * @param path_pointer 目录指针
        * @param index 要取的文件索引
        * @param name 取出的文件名
        * @param size 取出的文件长度
        * @return 是否成功
        */
        virtual bool            GetFileInfo(void *path_pointer,int64 index,UTF16String &name,int64 &size)=0;    ///<取得文件信息

        /**
        * 在指定目录内取得一个文件
        * @param path_pointer 目录指针
        * @param filename 文件名
        * @return 文件指针
        */
        virtual void *          GetFile(void *path_pointer,const UTF16String &filename,int64 *filelength=0)=0;

        /**
        * 取得一个文件
        * @param filename 文件名
        * @return 文件指针
        */
        virtual void *          GetFile(const UTF16String &filename,int64 *filelength=0)=0;

    public:

        /**
        * 查找一个文件，确定其是否存在
        * @param path_pointer 目录指针
        * @param filename 要查找的文件名称
        * @return 文件是否存在
        */
        virtual bool        FindFile(void *path_pointer,const UTF16String &filename)=0;                                    ///<查找一个文件

        /**
        * 加载一个文件到流中，并返回这个流
        * @param path_pointer 目录指针
        * @param filename 要加载的文件名称
        * @param load_to_memory 是否加载到内存
        * @return 保存文件数据的流，如果加载失败返回NULL
        */
        virtual InputStream *LoadFileFrom(void *path_pointer,const UTF16String &filename,bool load_to_memory=false)=0;          ///<加载一个文件到流

        /**
        * 加载一个文件到内存中
        * @param path_pointer 目录指针
        * @param filename 要加载的文件名称
        * @param data 要用来保存数据的内存块
        * @param size 内存块的大小，以及加载成功后文件的大小
        * @return 是否加载成功
        */
        virtual bool        LoadFileFrom(void *path_pointer,const UTF16String &filename,void **data,int64 *size)=0;              ///<加载一个文件到指定内存块

    public:

        /**
        * 异步请求读取一个文件的指定部分到内存
        * @param filename 要加载的文件名称
        * @param data 加载后数据存放地址
        * @param start 要加载数据在文件中的起始地址
        * @param length 要加载数据的长度
        * @return 文件访问指针
        */
        void *LoadAcquire(const UTF16String &filename,void *data=0,int64 start=0,int64 length=0)                            ///<异步请求取一个文件
        {   return _LoadAcquire(nullptr,filename,nullptr,data,start,length); }

        /**
        * 异步请求读取一个文件的指定部分到内存
        * @param path_pointer 目录指针
        * @param filename 要加载的文件名称
        * @param data 加载后数据存放地址
        * @param start 要加载数据在文件中的起始地址
        * @param length 要加载数据的长度
        * @return 文件访问指针
        */
        void *LoadAcquire(void *path_pointer,const UTF16String &filename,void *data=0,int64 start=0,int64 length=0)     ///<异步请求取一个文件
        {   return _LoadAcquire(path_pointer,filename,nullptr,data,start,length);}

        /**
        * 异步请求读取一个文件的指定部分到内存
        * @param file_pointer 要加载的文件指针
        * @param data 加载后数据存放地址
        * @param start 要加载数据在文件中的起始地址
        * @param length 要加载数据的长度
        * @return 文件访问指针
        */
        void *LoadAcquire(void *file_pointer,void *data=0,int64 start=0,int64 length=0)                             ///<异步请求取一个文件
        {   return _LoadAcquire(nullptr,nullptr,file_pointer,data,start,length);}

        /**
        * 取得一个文件读取的现有状态
        * @param file_pointer 文件访问指针
        * @return 状态
        */
        HacStatus GetStatus(void *);

        /**
        * 取得一个请求的数据长度
        * @param file_pointer 文件访问指针
        */
        int GetSize(void *);

        /**
        * 取得一个文件的数据(如果读取完成的话)
        * @param file_pointer 文件访问指针
        * @return 数据指针，如果未完成返回NULL
        */
        void *GetData(void *);

        /**
        * 清除文件
        * @param file_pointer 文件访问指针
        */
        void Clear(void *);

        /**
        * 清除所有正在加载的文件
        */
        void Clear();
    };//class HAC

    HAC *OpenHAC(const UTF16String &name=0);                                                            ///<打开一个.HAC文件,或是根据路径创建一个虚拟的HAC环境

    /**
    * HAC资源索引
    */
    struct HacResIndex
    {
        HAC *hac;
        UTF16String path;
        UTF16String file;

        HacResIndex()
        {
            hac=nullptr;
        }

        HacResIndex(HAC *h,const UTF16String &p,const UTF16String &f)
        {
            hac=h;
            path=p;
            file=f;
        }

        const int comp(const HacResIndex &sri)const
        {
            if(hac>sri.hac)return(1);
            if(hac<sri.hac)return(-1);

            if(path>sri.path)return(1);
            if(path<sri.path)return(-1);

            if(file>sri.file)return(1);
            if(file<sri.file)return(-1);

            return(0);
        }

        CompOperator(const HacResIndex &,comp);
    };//struct HacResIndex
}//namespace hgl
#endif//HGL_HAC_INCLUDE
