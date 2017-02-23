#include"dfsClientSaveThread.h"
#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/io/MemoryOutputStream.h>

namespace hgl
{
    namespace dfs
    {
        dfsClientSaveThread::dfsClientSaveThread(const UTF8String &gn,const IPAddress *_addr,const uint64 n)
        {
            group_name=gn;

            addr = _addr->CreateCopy();
            node_id=n;

            con=new dfsClientConnect;

            con->Init(addr,"dfsClientSaveThread",node_id);

            fb_sem=new Semaphore(1024);
        }

        dfsClientSaveThread::~dfsClientSaveThread()
        {
            SAFE_CLEAR(addr);
            delete con;
        }

        void dfsClientSaveThread::Add(dfsClientSaveItem &csi)
        {
            bool result=false;
            int index;

            fb_list.Lock();
                index=fb_list->Find(csi);

                if(index!=-1)
                {
                    dfsClientSaveItem old_csi;

                    fb_list->GetItem(index,old_csi);

                    delete old_csi.mos;

                    fb_list->Delete(index);

//                     std::cout<<"dfsClientSaveThread["<<group_name.c_str()<<"] Update Add:"<<csi.block->GetFilename()<<std::endl;
                }
                else
                {
//                     std::cout<<"dfsClientSaveThread["<<group_name.c_str()<<"] Add:"<<csi.block->GetFilename()<<std::endl;
                }

                fb_list->Push(csi);
            fb_list.Unlock();

            fb_sem->Release();
        }

        dfs::ErrorCode dfsClientSaveThread::SaveTo(DataInputStream *dis,DataOutputStream *dos,FileBlock *block,MemoryOutputStream *mos,uint32 attrib)
        {
            int32 cmd;
            int64 server_version;

            ThreadMutexLock tml(con->GetLock());
            OnlyWriteLock owl(block->GetLock());

            if(!con->Use())RETURN_ERROR(dfs::ecConnectFailed);

            if(!dos->WriteInt32(tdsSaveFile))RETURN_ERROR(dfs::ecSocketSendError);
            if(!dos->WriteInt64(block->GetFilename()))RETURN_ERROR(dfs::ecSocketSendError);
            if(!dos->WriteInt64(block->GetVersion()))RETURN_ERROR(dfs::ecSocketSendError);            //基础版本号
            if(!dos->WriteInt32(mos->Tell()))RETURN_ERROR(dfs::ecSocketSendError);
            if(!dos->WriteUint32(attrib))RETURN_ERROR(dfs::ecSocketSendError);

            if(!dis->ReadInt32(cmd))RETURN_ERROR(dfs::ecSocketRecvError);

            if(cmd!=dstSaveFile)RETURN_ERROR(dfs::ecLoginError);

            if(!dis->ReadInt64(server_version))RETURN_ERROR(dfs::ecSocketRecvError);

            if(!(attrib&DFS_FILE_NO_VERSION))
            {
                if(server_version>block->GetVersion())
                    RETURN_ERROR(dfs::ecBaseVersionOutdate);
            }

            if(dos->WriteFully(mos->GetData(),mos->Tell())!=mos->Tell())
                RETURN_ERROR(dfs::ecSocketRecvError);

            tml.Clear();

            block->Update((char *)(mos->GetData()),mos->Tell(),++server_version);

            LOG_INFO(U8_TEXT("dfsClientSaveThread[")+group_name+UTF8String("] savefile:")+UTF8String(block->GetFilename()));
//             std::cout<<"dfsClientSaveThread["<<group_name.c_str()<<"] savefile:"<<block->GetFilename()<<std::endl;
            mos->Unlink();                                        //退开关联

            return(dfs::ecNone);
        }

        bool dfsClientSaveThread::Execute()
        {
            fb_sem->Acquire();

            bool result;
//             int count;
            dfsClientSaveItem csi;

            fb_list.Lock();
                result=fb_list->Pop(csi);
//                 count=fb_list->GetCount();
            fb_list.Unlock();

            if(!result)
                return(true);

//             std::cout<<"dfsClientSaveThread["<<group_name.c_str()<<"] left file "<<count<<"."<<std::endl;

            SaveTo(con->GetDIS(),con->GetDOS(),csi.block,csi.mos,csi.attrib);
            delete csi.mos;
            return(true);
        }
    }//namespace dfs
}//namespace hgl
