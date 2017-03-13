#include<hgl/dfs/dfsClientFile.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include"dfsClientFileConnect.h"
namespace hgl
{
    namespace dfs
    {
        namespace
        {
            dfs::ErrorCode dfsDeleteFile(DataInputStream *dis,DataOutputStream *dos,const int64 filename)
            {
                if(!dos->WriteInt32(tdsDeleteFile))RETURN_ERROR(dfs::ecSocketSendError);
                if(!dos->WriteInt64(filename))RETURN_ERROR(dfs::ecSocketSendError);

                int32 cmd;

                if(!dis->ReadInt32(cmd))RETURN_ERROR(dfs::ecSocketRecvError);

                if(cmd!=dstDeleteFile)RETURN_ERROR(dfs::ecLoginError);

                bool result;

                if(!dis->ReadBool(result))RETURN_ERROR(dfs::ecSocketRecvError);

                if(!result)
                    RETURN_ERROR(dfs::ecDeleteFileFailed);

                return(dfs::ecNone);
            }
        }//namespace

        /**
         * 删除一个文件
         * @param file_id 文件索引
         * @return 是否成功
         */
        bool dfsClientFile::DeleteFile(const int64 file_id)
        {
            FileTeam *ft=file_team+(file_id%team_number);

            ThreadMutexLock tml(&(ft->lock));

            dfsClientFileConnect *con=conn_pool->SafeAcquire();

            if(!con->Use())
            {
                conn_pool->SafeRelease(con);
                RETURN_FALSE;
            }

            dfs::ErrorCode ec=dfsDeleteFile(con->GetDIS(),con->GetDOS(),file_id);

            if(ec!=dfs::ecNone)
                con->Disconnect();
            
            conn_pool->SafeRelease(con);
            return(ec==dfs::ecNone);
        }
    }//namespace dfs
}//namespace hgl
