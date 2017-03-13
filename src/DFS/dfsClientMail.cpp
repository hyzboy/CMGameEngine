#include<hgl/dfs/dfsClientMail.h>
#include<hgl/dfs/dfs.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryOutputStream.h>

namespace hgl
{
    using namespace io;

    namespace dfs
    {
        namespace
        {
            dfs::ErrorCode dfsMailSend(DataOutputStream *dos,const int64 name,int32 index,char *data,int32 size)
            {
                if(!dos->WriteInt32(tdsMailSend))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(name))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(index))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(size))return(dfs::ecSocketSendError);
                if(dos->WriteFully(data,size)!=size)return(dfs::ecSocketSendError);

                return(dfs::ecNone);
            }

            dfs::ErrorCode dfsMailGetCount(DataInputStream *dis,DataOutputStream *dos,const int64 name,List<int32> &id_list)
            {
                if(!dos->WriteInt32(tdsMailGetCount))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(name))return(dfs::ecSocketSendError);

                int32 count;

                if(!dis->ReadInt32(count))return(dfs::ecSocketRecvError);

                id_list.SetCount(count);

                if(count<=0)
                    return(dfs::ecNone);

                if(dis->ReadInt32(id_list.GetData(),count)!=count)
                    return(dfs::ecSocketRecvError);

                return(dfs::ecNone);
            }

            dfs::ErrorCode dfsMailGet(DataInputStream *dis,dfsMailList &mail_list)
            {
                int32 count;

                if(!dis->ReadInt32(count))return(dfs::ecSocketRecvError);

                if(count<=0)
                    return(dfs::ecNone);

                int32 index;
                int32 size;

                for(int32 i=0;i<count;i++)
                {
                    if(!dis->ReadInt32(index))return(dfs::ecSocketRecvError);
                    if(!dis->ReadInt32(size))return(dfs::ecSocketRecvError);

                    MemBlock<char> *mb=new MemBlock<char>(size);

                    if(dis->ReadFully(mb->data(),size)!=size)
                    {
                        delete mb;
                        return(dfs::ecSocketRecvError);
                    }

                    mail_list.Add(index,mb);
                }

                return(dfs::ecNone);
            }

            dfs::ErrorCode dfsMailGet(DataInputStream *dis,DataOutputStream *dos,const int64 name,int32 start,int32 end,dfsMailList &mail_list)
            {
                if(!dos->WriteInt32(tdsMailAcquireScope))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(name))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(start))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(end))return(dfs::ecSocketSendError);

                return dfsMailGet(dis,mail_list);
            }

            dfs::ErrorCode dfsMailGet(DataInputStream *dis,DataOutputStream *dos,const int64 name,int32 count,int32 *index,dfsMailList &mail_list)
            {
                if(!dos->WriteInt32(tdsMailAcquire))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(name))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(count))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(index,count))return(dfs::ecSocketSendError);

                return dfsMailGet(dis,mail_list);
            }

            dfs::ErrorCode dfsMailDelete(DataOutputStream *dos,const int64 name,int32 count,int32 *index)
            {
                if(!dos->WriteInt32(tdsMailDelete))return(dfs::ecSocketSendError);
                if(!dos->WriteInt64(name))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(count))return(dfs::ecSocketSendError);
                if(!dos->WriteInt32(index,count))return(dfs::ecSocketSendError);

                return dfs::ecNone;
            }

            class MailSendStream:public OutputStream
            {
                dfsClientConnect *con;

                MemoryOutputStream mos;

                int64 name;
                int32 index;

            public:

                MailSendStream(dfsClientConnect *c,const int64 n,int32 i)
                {
                    con=c;

                    name=n;
                    index=i;
                }

                void    Close()                                                                    ///<关闭输出流
                {
                    if(con->Use())
                        dfsMailSend(con->GetDOS(),name,index,(char *)mos.GetData(),mos.Tell());
                }

                int64    Write(const void *buf,int64 size){return mos.Write(buf,size);}            ///<写入数据

                bool    CanRestart()const{return mos.CanRestart();}                                ///<是否可以复位
                bool    CanSeek()const{return mos.CanSeek();}                                    ///<是否可以定位
                bool    CanSize()const{return mos.CanSize();}                                    ///<是否可以取得尺寸

                bool    Restart(){return mos.Restart();}                                        ///<复位访问指针
                int64    Seek(int64 off,SeekOrigin so){return mos.Seek(off,so);}                    ///<移动访问指针
                int64    Tell()const{return mos.Tell();}                                            ///<返回当前访问位置
                int64    GetSize()const{return mos.GetSize();}                                    ///<取得流长度
                int64    Available()const{return mos.Available();}                                ///<剩下的可以不受阻塞写入的字节数
            };//class MailSendStream:public OutputStream
        }//namespace

        /**
         * 向指定成员发送一封邮件
         * @param key 成员key
         * @param index 邮件编号(新增邮件使用-1表示自动编号,否则代表修改过去已发送邮件)
         * @return 发送邮件输出流
         * @return NULL 失败
         */
        OutputStream *dfsClientMail::MailSend(const int64 key,int32 index)
        {
            ThreadMutexLock tml(lock);

            if(!Use())
                RETURN_ERROR_NULL;

            return(new MailSendStream(this,key,index));
        }

        /**
         * 获取指定成员的邮件id列表
         * @param key 成员key
         * @param id_list 邮件id列表
         * @return 是否成功
         */
        bool dfsClientMail::MailGetCount(const int64 key,List<int32> &id_list)
        {
            if(!lock->TryLock())
                return(false);

            if(!Use())
            {
                lock->Unlock();
                RETURN_FALSE;
            }

            const bool result=(dfsMailGetCount(GetDIS(),GetDOS(),key,id_list)==dfs::ecNone);

            lock->Unlock();
            return result;
        }

        /**
         * 获取指定成员的邮件列表
         * @param key 成员key
         * @param start 邮件起始id
         * @param end 邮件结束id
         * @param mail_list 获取到的邮件列表存放变量
         * @return 是否成功
         */
        bool dfsClientMail::MailGet(const int64 key,int32 start,int32 end,dfsMailList &mail_list)
        {
            ThreadMutexLock tml(lock);

            if(!Use())
                RETURN_FALSE;

            return(dfsMailGet(GetDIS(),GetDOS(),key,start,end,mail_list)==dfs::ecNone);
        }

        /**
         * 获取指定成员指定ID号的邮件
         * @param key 成员key
         * @param number 要获取的邮件数量
         * @param index 要获取的邮件id列表
         * @param mail_list 获取到的邮件列表存放变量
         * @return 是否成功
         */
        bool dfsClientMail::MailGet(const int64 key,int32 number,int32 *index,dfsMailList &mail_list)
        {
            ThreadMutexLock tml(lock);

            if(!Use())
                RETURN_FALSE;

            return(dfsMailGet(GetDIS(),GetDOS(),key,number,index,mail_list)==dfs::ecNone);
        }

        /**
         * 删除指定成员指定ID号的邮件
         * @param key 成员key
         * @param number 要删除的邮件数量
         * @param index 要删除的邮件id列表
         * @return 是否成功
         */
        bool dfsClientMail::MailDelete(const int64 key,int32 number,int32 *index)
        {
            ThreadMutexLock tml(lock);

            if(!Use())
                RETURN_FALSE;

            return(dfsMailDelete(GetDOS(),key,number,index)==dfs::ecNone);
        }
    }//namespace dfs
}//namespace hgl
