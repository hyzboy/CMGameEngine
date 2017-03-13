#include<hgl/dfs/dfs.h>
#include<hgl/dfs/dfsClientRank.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/trsCmd.h>
#include<hgl/type/DateTime.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/thread/ThreadMutex.h>
namespace hgl
{
    namespace dfs
    {
        bool ReadRankItem(RankItem *ri,DataInputStream *dis)
        {
            if(!dis->ReadInt64(ri->id))RETURN_FALSE;
            if(!dis->ReadInt64(ri->value))RETURN_FALSE;
            if(!dis->ReadInt32(ri->size))RETURN_FALSE;

            if (ri->size>RANK_ITEM_DATA_MAX_SIZE)RETURN_FALSE;

            return(dis->ReadFully(ri->data,ri->size)==ri->size);
        }

        bool ReadRankItemRecord(RankItem *ri,DataInputStream *dis)
        {
            if(!dis->ReadInt32(ri->no))RETURN_FALSE;

            if(ri->no==-1)
                return(true);

            if(!dis->ReadInt64(ri->value))RETURN_FALSE;
            if(!dis->ReadInt32(ri->size))RETURN_FALSE;

            if(ri->size<=0)
                return(true);

            if (ri->size>RANK_ITEM_DATA_MAX_SIZE)RETURN_FALSE;

            return(dis->ReadFully(ri->data,ri->size)==ri->size);
        }

        bool RankList::Read(DataInputStream *dis)
        {
            int32 count;
            int32 start;

            if(!dis->ReadInt32(count))RETURN_FALSE;

            ri_list.SetCount(count);

            if(count<=0)
                return(true);

            if(!dis->ReadInt32(start))RETURN_FALSE;

            RankItem *p=ri_list.GetData();
            for(int i=0;i<count;i++)
            {
                if(!ReadRankItem(p,dis))RETURN_FALSE;

                p->no=start++;

                p++;
            }

            return(true);
        }

        namespace
        {
            dfs::ErrorCode dfsRankUpdate(DataOutputStream *dos,const UTF8String &name,int64 key,int64 value,char *data,int32 size)
            {
                if(!dos->WriteInt32(trsUpdateRecord))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(key))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(value))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(size))return(dfs::ecSocketSendError);

                if(data>0)
                    if(dos->WriteFully(data,size)!=size)return(dfs::ecSocketSendError);

                return(dfs::ecNone);
            }

            dfs::ErrorCode dfsRankAcquireList(DataInputStream *dis,DataOutputStream *dos,const UTF8String &name,int32 start,int32 number,RankList &rl)
            {
                int32 cmd;

                if(!dos->WriteInt32(trsAcquireList))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(start))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(number))return(dfs::ecSocketSendError);

                if(!dis->ReadInt32(cmd))return(dfs::ecSocketRecvError);

                if(cmd!=dstRankList)return(dfs::ecLoginError);

                if(!rl.Read(dis))
                    return(dfs::ecSocketRecvError);

                return(dfs::ecNone);
            }

            dfs::ErrorCode dfsRankAcquireRecord(DataInputStream *dis,DataOutputStream *dos,const UTF8String &name,int64 key,RankItem &ri)
            {
                int32 cmd;

                if(!dos->WriteInt32(trsAcquireRecord))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(key))return(dfs::ecSocketSendError);

                if(!dis->ReadInt32(cmd))return(dfs::ecSocketRecvError);

                if(cmd!=dstRankRecord)return(dfs::ecLoginError);

                if(!ReadRankItemRecord(&ri,dis))
                    return(dfs::ecSocketRecvError);

                return(dfs::ecNone);
            }
        }//namespace

        dfsClientRank::dfsClientRank(const UTF8String &rn)
        {
            rank_name=rn;
        }

        dfsClientRank::~dfsClientRank()
        {
        }

        bool dfsClientRank::SendPrivateLogin(DataOutputStream *dos)
        {
            return dos->WriteUTF8String(rank_name);
        }

        /**
         * 向排行榜提交成绩
         * @param key 要提交成绩的key
         * @param score Key的成绩得分
         * @param data 自定义数据
         * @param size 自定义数据长度
         */
        void dfsClientRank::RankUpdateRecord(int64 key,int64 score,char *data,int32 size)
        {
            ThreadMutexLock tml(lock);

            if(Use())
                dfsRankUpdate(GetDOS(),rank_name,key,score,data,size);
        }

        /**
         * 请求一个排行榜列表
         * @param start 起始者排名
         * @param number 请求的数量
         * @return 排行榜列表数据
         * @return NULL 请求失败
         */
        RankList *dfsClientRank::RankAcquireList(int32 start,int32 number)
        {
            if(start<0||number<=0)return(nullptr);

            ThreadMutexLock tml(lock);

            if(!Use())
                return(nullptr);

            rank_list.WriteLock();

            dfsRankAcquireList(GetDIS(),GetDOS(),rank_name,start,number,rank_list);

            rank_list.WriteUnlock();
            rank_list.ReadLock();

            return &rank_list;
        }

        /**
         * 请求一个成员的排行成绩
         * @param key 成员key
         * @param ri 排行成绩数据结构
         */
        void dfsClientRank::RankAcquireRecord(int64 key,RankItem &ri)
        {
            ThreadMutexLock tml(lock);

            if(Use())
                dfsRankAcquireRecord(GetDIS(),GetDOS(),rank_name,key,ri);
        }
    }//namespace dfs
}//namespace hgl
