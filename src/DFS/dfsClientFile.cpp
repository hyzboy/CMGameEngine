#include<hgl/dfs/dfsClientFile.h>
#include"dfsClientSaveThread.h"
#include"dfsClientFileConnect.h"

namespace hgl
{
    namespace dfs
    {
        dfsClientFile::dfsClientFile(const UTF8String &gn,int tn)
        {
            set_name=gn;
            save_thread=nullptr;

            team_number=tn;

            file_team=new FileTeam[team_number];

            conn_pool=nullptr;
        }

        dfsClientFile::~dfsClientFile()
        {
            delete[] file_team;
            SAFE_CLEAR(conn_pool);
            SAFE_CLEAR(save_thread);
        }

        bool dfsClientFile::SendPrivateLogin(DataOutputStream *dos)
        {
            return dos->WriteUTF8String(set_name);
        }

        /**
         * 初始化dfs文件客户端
         * @param _addr 服务器地址
         * @param _node_name 当前节点名称
         * @param _node_id 当前节点id
         * @return 是否初始化成功
         */
        bool dfsClientFile::Init(const IPAddress *_addr,const UTF8String &_node_name,const int64 _node_id)
        {
            if(!dfsClientConnect::Init(_addr,_node_name,_node_id))
                RETURN_FALSE;

            conn_pool=new dfsClientFileConnectPool(set_name,_addr,_node_name,_node_id);

            if(!Use())
                RETURN_FALSE;

            save_thread=new dfsClientSaveThread(set_name,_addr,node_id);
            save_thread->Thread::Start();
            return(true);
        }
    }//namespace dfs
}//namespace hgl
