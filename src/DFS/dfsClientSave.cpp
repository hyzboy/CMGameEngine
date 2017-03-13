#include<hgl/dfs/dfsClientFile.h>
#include<hgl/dfs/dstCmd.h>
#include<hgl/dfs/tdsCmd.h>
#include<hgl/network/TCPClient.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/thread/ThreadMutex.h>
#include"dfsClientSaveThread.h"
#include"dfsClientFileConnect.h"

namespace hgl
{
    namespace dfs
    {
        namespace
        {
            class dfsOutputStream:public OutputStream
            {
                dfsClientSaveThread *cst;

                dfs::FileBlock *block;
                MemoryOutputStream *mos;

                uint32 attrib;

            public:

                dfsOutputStream(dfsClientSaveThread *c,dfs::FileBlock *fb,const uint32 a)
                {
                    cst=c;
                    block=fb;
                    attrib=a;

                    mos=new MemoryOutputStream;
                }

                ~dfsOutputStream()
                {
                    SAFE_CLEAR(mos);
                }

                void    Close()                                                                    ///<关闭输出流
                {
                    dfsClientSaveItem csi;

                    csi.block=block;
                    csi.mos=mos;
                    csi.attrib=attrib;

                    cst->Add(csi);
                    mos=nullptr;            //mos由保存线程delete，这里设为NULL防止析构时被delete
                }

                int64    Write(const void *buf,int64 size){return mos->Write(buf,size);}            ///<写入数据

                bool    CanRestart()const{return mos->CanRestart();}                            ///<是否可以复位
                bool    CanSeek()const{return mos->CanSeek();}                                    ///<是否可以定位
                bool    CanSize()const{return mos->CanSize();}                                    ///<是否可以取得尺寸

                bool    Restart(){return mos->Restart();}                                        ///<复位访问指针
                int64    Seek(int64 off,SeekOrigin so){return mos->Seek(off,so);}                ///<移动访问指针
                int64    Tell()const{return mos->Tell();}                                        ///<返回当前访问位置
                int64    GetSize()const{return mos->GetSize();}                                    ///<取得流长度
                int64    Available()const{return mos->Available();}                                ///<剩下的可以不受阻塞写入的字节数
            };//class dfsOutputStream:public OutputStream
        }//namespace

        /**
         * 保存一个文件
         * @param file_id 文件索引
         * @param attrib 属性
         * @return 保存文件用的输出流
         */
        OutputStream *dfsClientFile::SaveFile(const int64 file_id,const uint32 attrib)
        {
            FileTeam *ft=file_team+(file_id%team_number);

            ft->lock.Lock();

            FileBlock *fb=ft->file_list[file_id];

            if(!fb)
            {
                fb=new FileBlock(file_id);

                ft->file_list.Add(file_id,fb);
            }

            ft->lock.Unlock();

            return(new dfsOutputStream(save_thread,fb,attrib));
        }
    }//namespace dfs
}//namespace hgl
