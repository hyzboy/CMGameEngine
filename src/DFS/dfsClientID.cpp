#include<hgl/dfs/dfsClientID.h>
#include<hgl/type/Set.h>
#include<hgl/type/Pool.h>
#include<hgl/thread/RWLock.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/dfs/tisCmd.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

namespace hgl
{
    namespace dfs
    {
        struct IDLockTeam
        {
            RWLock lock;

            Set<int64> lock_set;

        public:

            bool Lock(const int64 id)
            {
                int index;

                lock.WriteLock();
                    index=lock_set.Add(id);
                lock.WriteUnlock();

                return(index!=-1);
            }

            bool Unlock(const int64 id)
            {
                bool result;

                lock.WriteLock();
                    result=lock_set.Delete(id);
                lock.WriteUnlock();

                return(result);
            }

            bool IsLock(const int64 id)
            {
                int index;

                lock.ReadLock();
                    index=lock_set.Find(id);
                lock.ReadUnlock();

                return(index!=-1);
            }
        };//struct IDLockTeam
    }//namespace dfs

    namespace dfs
    {
        class dfsClientIDConnect:public dfsClientConnect
        {
            UTF8String set_name;

        public:

            void SetName(const UTF8String &sn)
            {
                set_name=sn;
            }

            bool SendPrivateLogin(DataOutputStream *dos)
            {
                return dos->WriteUTF8String(set_name);
            }
        };//class dfsClientIDConnect

        class dfsClientIDConnectPool:public MTObjectPool<dfsClientIDConnect>
        {
            UTF8String set_name;

            IPAddress *ip_addr;
            UTF8String node_name;
            int64 node_id;

        private:

            dfsClientIDConnect *Create()
            {
                dfsClientIDConnect *obj=new dfsClientIDConnect();

                obj->SetName(set_name);

                if(!obj->Init(ip_addr,node_name,node_id))
                {
                    delete obj;
                    return NULL;
                }

                return obj;
            }

        public:

            dfsClientIDConnectPool(const UTF8String &sn,const IPAddress *addr,const UTF8String &nn,int64 ni)
            {
                set_name    =sn;

                ip_addr     =addr->CreateCopy();
                node_name    =nn;
                node_id        =ni;
            }
        };//class dfsClientIDConnectPool

        namespace
        {
            class dfsClientIDAccess
            {
                MTObjectPool<dfsClientIDConnect> *pool;

                dfsClientIDConnect *con;
                DataInputStream *dis;
                DataOutputStream *dos;

            public:

                dfsClientIDAccess(MTObjectPool<dfsClientIDConnect> *p)
                {
                    pool=p;

                    con=pool->SafeAcquire();

                    dis=con->GetDIS();
                    dos=con->GetDOS();
                }

                ~dfsClientIDAccess()
                {
                    pool->SafeRelease(con);
                }

                void Disconnect()
                {
                    con->Disconnect();
                }

                bool CreateID(int64 &result)
                {
                    ThreadMutexLock tml(con->GetLock());

                    if(!con->Use())
                        return(false);

                    if(!dos->WriteInt32(tisCreateID))return(false);

                    return dis->ReadInt64(result);
                }

                bool KeyAccess(int32 cmd,const dfsIDKey &key,int64 &result)
                {
                    ThreadMutexLock tml(con->GetLock());

                    if(!con->Use())
                        return(false);

                    if(!dos->WriteInt32(cmd))return(false);
                    if(!dos->WriteFully(key,DFS_ID_KEY_SIZE))return(false);

                    return dis->ReadInt64(result);
                }

                bool Delete(const dfsIDKey &key)
                {
                    ThreadMutexLock tml(con->GetLock());

                    if(!con->Use())
                        return(false);

                    if(!dos->WriteInt32(tisDelete))return(false);
                    if(!dos->WriteFully(key,DFS_ID_KEY_SIZE))return(false);

                    return(true);
                }

                bool LockAccess(int32 cmd,const int64 id,bool &result)
                {
                    ThreadMutexLock tml(con->GetLock());

                    if(!con->Use())
                        return(false);

                    if(!dos->WriteInt32(cmd))return(false);
                    if(!dos->WriteInt64(id))return(false);

                    if(!dis->ReadBool(result))return(false);

                    return(true);
                }

                bool GetAndLock(const dfsIDKey &key,int64 &id,bool &result)
                {
                    ThreadMutexLock tml(con->GetLock());

                    if(!con->Use())
                        return(false);

                    if(!dos->WriteInt32(tisGetAndLock))return(false);
                    if(!dos->WriteFully(key,DFS_ID_KEY_SIZE))return(false);

                    if(!dis->ReadBool(result))return(false);
                    if(!dis->ReadInt64(id))return(false);

                    return(true);
                }
            };//class dfsClientIDConnect
        }//namespace

        dfsClientID::dfsClientID(const UTF8String &sn)
        {
            set_name=sn;

            lock_list=new IDLockTeam[0x100];

            conn_pool=nullptr;
        }

        dfsClientID::~dfsClientID()
        {
            delete[] lock_list;
            SAFE_CLEAR(conn_pool);
        }

        bool dfsClientID::Init(const IPAddress *addr,const UTF8String &n,const int64 id)
        {
            if(!dfsClientConnect::Init(addr,n,id))RETURN_FALSE;

            conn_pool=new dfsClientIDConnectPool(set_name,addr,n,id);

            return(true);
        }

        bool dfsClientID::SendPrivateLogin(DataOutputStream *dos)
        {
            return dos->WriteUTF8String(set_name);
        }

        /**
         * 单纯产生一个ID
         * @return 产生的ID号
         * @return -1 增加失败
         */
        int64 dfsClientID::CreateID()
        {
            dfsClientIDAccess cid(conn_pool);

            int64 result;

            if(!cid.CreateID(result))
            {
                cid.Disconnect();
                return(-1);
            }

            return(result);
        }

        /**
         * 增加一个Key
         * @return 这个Key对应的ID号
         * @return -1 增加失败
         */
        int64 dfsClientID::Add(const dfsIDKey &key)
        {
            dfsClientIDAccess cid(conn_pool);

            int64 result;

            if(!cid.KeyAccess(tisAdd,key,result))
            {
                cid.Disconnect();
                return(-1);
            }

            return(result);
        }

        /**
         * 取得一个key对应的ID
         * @return 这个Key对应的ID号
         * @return -1 获取失败
         */
        int64 dfsClientID::Get(const dfsIDKey &key)
        {
            dfsClientIDAccess cid(conn_pool);

            int64 result;

            if(!cid.KeyAccess(tisGet,key,result))
            {
                cid.Disconnect();
                return(-1);
            }

            return(result);
        }

        /**
         * 删除一个Key
         * @return 是否成功
         */
        bool dfsClientID::Delete(const dfsIDKey &key)
        {
            dfsClientIDAccess cid(conn_pool);

            if(!cid.Delete(key))
            {
                cid.Disconnect();
                return(false);
            }

            return(true);
        }

        /**
         * 锁定一个ID
         * @param id ID号
         * @return 是否成功
         */
        bool dfsClientID::Lock(const int64 id)
        {
            if(!lock_list[id&0xFF].Lock(id))
                return(false);

            dfsClientIDAccess cid(conn_pool);
            bool result;

            if(!cid.LockAccess(tisLock,id,result))
            {
                lock_list[id&0xFF].Unlock(id);
                cid.Disconnect();
                return(false);
            }

            return(result);
        }

        /**
         * 解锁一个ID
         * @param id ID号
         * @return 是否成功
         */
        bool dfsClientID::Unlock(const int64 id)
        {
            lock_list[id&0xFF].Unlock(id);

            dfsClientIDAccess cid(conn_pool);
            bool result;

            if(!cid.LockAccess(tisUnlock,id,result))
            {
                cid.Disconnect();
                return(false);
            }

            return(result);
        }

        /**
         * 获取指定ID是否加锁
         * @param id ID号
         * @param result 是否加锁的结构保存变量
         * @return 是否获取成功
         */
        bool dfsClientID::IsLock(const int64 id,bool &result)
        {
            if(lock_list[id&0xFF].IsLock(id))
                return(true);

            dfsClientIDAccess cid(conn_pool);

            if(cid.LockAccess(tisIsLock,id,result))
            {
                if(result)
                    lock_list[id&0xFF].Lock(id);        //本地也加锁做为缓冲

                return true;
            }

            cid.Disconnect();
            return(false);
        }

        /**
         * 获取一个Key对应的ID并加锁
         * @param id 获取的Key对应ID保存变量
         * @return 是否成功
         */
        bool dfsClientID::GetAndLock(const dfsIDKey &key,int64 &id)
        {
            dfsClientIDAccess cid(conn_pool);

            bool result;

            if(cid.GetAndLock(key,id,result))
            {
                if(result)
                {
                    lock_list[id&0xFF].Lock(id);
                    return(true);
                }

                return(false);
            }

            cid.Disconnect();
            return(false);
        }
    }//namespace dfs
}//namespace hgl
