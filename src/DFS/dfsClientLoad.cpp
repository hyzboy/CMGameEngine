#include<hgl/dfs/dfsClientFile.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/thread/ThreadMutex.h>
#include"dfsClientFileConnect.h"
#include<iostream>
namespace hgl
{
    namespace dfs
    {
        namespace
        {
            class dfsClientLoad
            {
                char *block;
                int32 block_size;

            public:

                dfsClientLoad()
                {
                    block=nullptr;
                    block_size=0;
                }

                ~dfsClientLoad()
                {
                    if(block)
                        hgl_free(block);
                }

                dfs::ErrorCode LoadFromRemote(DataInputStream *dis,DataOutputStream *dos,dfs::FileBlock *fb)
                {
                    if(!dos->WriteInt32(tdsLoadFile))RETURN_ERROR(dfs::ecSocketSendError);
                    if(!dos->WriteInt64(fb->GetFilename()))RETURN_ERROR(dfs::ecSocketSendError);
                    if(!dos->WriteInt64(fb->GetVersion()))RETURN_ERROR(dfs::ecSocketSendError);

                    int32 cmd;
                    int64 server_version;

                    if(!dis->ReadInt32(cmd))RETURN_ERROR(dfs::ecSocketRecvError);

                    if(cmd!=dstLoadFile)RETURN_ERROR(dfs::ecLoginError);

                    if(!dis->ReadInt64(server_version))RETURN_ERROR(dfs::ecSocketRecvError);

                    if(server_version<=fb->GetVersion())        //服务器的版本更旧
                        return(dfs::ecNone);

                    int32 file_length;

                    if(!dis->ReadInt32(file_length))return(dfs::ecSocketRecvError);

                    if(file_length<=0)return(dfs::ecNone);

                    if(block)
                    {
                        if(block_size<file_length)
                            block=(char *)hgl_realloc(block,file_length);
                    }
                    else
                    {
                        block=(char *)hgl_malloc(file_length);
                        block_size=file_length;
                    }

                    if(fb->GetLength()>0
                     &&fb->GetLength()<file_length/10)
                    {
                        std::cout<<"old fb length "<<fb->GetLength()<<",new length "<<file_length<<",problem"<<std::endl;
                    }

                    if(dis->ReadFully(block,file_length)!=file_length)
                        RETURN_ERROR(dfs::ecSocketRecvError);

                    fb->Update(block,file_length,server_version);

                    block=nullptr;                        //将这个类的这块标记干掉，以再它放掉block

                    return(dfs::ecNone);
                }
            };//class dfsClientLoad

            class dfsInputStream:public MemoryInputStream
            {
                dfs::FileBlock *fb;

            public:

                dfsInputStream(dfs::FileBlock *f)
                {
                    fb=f;

                    fb->ReadLock();

                    LOG_INFO(OS_TEXT("dfsClient LoadFile<")+OSString(fb->GetFilename())+OS_TEXT("> : ReadLock"));

                    Link(fb->GetData(),fb->GetLength());
                }

                ~dfsInputStream()
                {
                    LOG_INFO(OS_TEXT("dfsClient LoadFile<")+OSString(fb->GetFilename())+OS_TEXT("> : ReadUnlock"));
                    fb->ReadUnlock();
                }
            };//class dfsInputStream
        }//namespace

        /**
         * 加载一个文件
         * @param file_id 文件索引
         * @return 文件输入流
         * @return NULL 加载失败
         */
        InputStream *dfsClientFile::LoadFile(const int64 file_id)
        {
            FileTeam *ft=file_team+(file_id%team_number);

            ft->lock.Lock();

            FileBlock *fb=ft->file_list[file_id];

            if(!fb)
            {
                fb=new FileBlock(file_id);

                ft->file_list.Add(file_id,fb);
            }

            fb->WriteLock();

            dfsClientFileConnect *con=conn_pool->SafeAcquire();
            if(!con->Use())
            {
                conn_pool->SafeRelease(con);
                fb->WriteUnlock();
                ft->lock.Unlock();
                return(nullptr);
            }

            dfsClientLoad cl;

            dfs::ErrorCode ec=cl.LoadFromRemote(con->GetDIS(),con->GetDOS(),fb);

            if(ec!=ecNone)
                con->Disconnect();

            conn_pool->SafeRelease(con);
            fb->WriteUnlock();
            ft->lock.Unlock();

            if(ec==ecNone)
                return(new dfsInputStream(fb));
            else
                return(nullptr);
        }
    }//namespace dfs
}//namespace hgl
