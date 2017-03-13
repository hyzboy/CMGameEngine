#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/dfs/dfs.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace dfs
    {
        struct dfsClientTCP
        {
            TCPClient *tcp;

            DataInputStream *dis;
            DataOutputStream *dos;

        public:

            dfsClientTCP()
            {
                tcp=new TCPClient();

                dis=new LEDataInputStream(tcp->GetInputStream());
                dos=new LEDataOutputStream(tcp->GetOutputStream());
            }

            ~dfsClientTCP()
            {
                delete dos;
                delete dis;
                delete tcp;
            }
        };//struct dfsClientTCP
    }//namespace dfs

    namespace dfs
    {
        DataInputStream *dfsClientConnect::GetDIS()
        {
            return tcp->dis;
        }

        DataOutputStream *dfsClientConnect::GetDOS()
        {
            return tcp->dos;
        }

        dfsClientConnect::dfsClientConnect()
        {
            ip_addr=nullptr;
            node_id=-1;

            lock=new ThreadMutex;

            tcp=nullptr;
        }

        dfsClientConnect::~dfsClientConnect()
        {
            SAFE_CLEAR(tcp);
            SAFE_CLEAR(lock);
            SAFE_CLEAR(ip_addr);
        }

        bool dfsClientConnect::Init(const IPAddress *ia,const UTF8String &n,const int64 id)
        {
            if(!ia)RETURN_FALSE;
            if(n.IsEmpty())RETURN_FALSE;

            if(tcp)    //重复初始化
            {
                if(!ip_addr->Comp(ia))RETURN_FALSE;
                if(node_id!=id)RETURN_FALSE;
                if(node_name!=n)RETURN_FALSE;

                return(true);
            }

            SAFE_CLEAR(ip_addr);
            ip_addr=ia->CreateCopy();

            node_id=id;
            node_name=n;

            tcp=new dfsClientTCP;

            Reconnect();

            return(true);
        }

        void dfsClientConnect::Disconnect()
        {
            tcp->tcp->Disconnect();
        }

        bool dfsClientConnect::Reconnect()
        {
            tcp->tcp->Disconnect();

            if(!tcp->tcp->Connect(ip_addr))RETURN_FALSE;

            if(!tcp->dos->WriteInt32(tdsLogin))RETURN_FALSE;
            if(!tcp->dos->WriteInt64(node_id))RETURN_FALSE;

            if(node_id==-1)        //第一次，还需要发送客户端版本号，以及节点名称
            {
                if(!tcp->dos->WriteInt32(DFS_VERSION))RETURN_FALSE;
                if(!tcp->dos->WriteUTF8String(node_name))RETURN_FALSE;

                if(!SendPrivateLogin(tcp->dos))RETURN_FALSE;
            }

            int32 cmd;

            if(!tcp->dis->ReadInt32(cmd))RETURN_FALSE;

            if(cmd!=dstLogin)RETURN_FALSE;

            if(node_id==-1)
            {
                if(!tcp->dis->ReadUTF8String(server_node_name))RETURN_FALSE;

                if(!tcp->dis->ReadInt64(node_id))RETURN_FALSE;

                LOG_INFO(U8_TEXT("Connect to DataServer \"")+server_node_name+U8_TEXT("\", my node id:")+UTF8String(node_id)+U8_TEXT(",Socket:")+UTF8String(tcp->tcp->ThisSocket));
            }

            tcp->tcp->SetBlock(true,5.0);

            return(true);
        }

        void dfsClientConnect::Lock()
        {
            lock->Lock();
        }

        void dfsClientConnect::Unlock()
        {
            lock->Unlock();
        }

        bool dfsClientConnect::Use()
        {
            if(!tcp)RETURN_FALSE;

            if(!tcp->dos->WriteInt32(0))        //心跳测试
            {
                if(!Reconnect())
                    RETURN_FALSE;
            }

            return(true);
        }
    }//namespace dfs
}//namespace hgl
