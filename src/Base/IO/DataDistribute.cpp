#include<hgl/io/DataDistribute.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace io
    {
        /**
        * 数据包自动分发处理类构造函数
        * @param _dis 数据输入流
        * @param s 起始数据包格式
        * @param e 结束数据包格式
        */
        DataDistribute::DataDistribute(DataInputStream *_dis,HGL_DATA_PACKET_TYPE s,HGL_DATA_PACKET_TYPE e)
        {
            dis=_dis;
            
            type_start=s;
            type_end=e;
            
            type_count=type_end-type_start+1;
            
            proc_list=new DataPacketProc *[type_count];
            
            Clear();
        }
        
        DataDistribute::~DataDistribute()
        {
            SAFE_CLEAR_OBJECT_ARRAY(proc_list,type_count);
        }
        
        /**
        * 注册一个数据包自动分发
        */
        bool DataDistribute::Registry(DataPacketProc *dpp)
        {
            if(!dpp)
            {
                LOG_ERROR(OS_TEXT("Error: DataDistribute::Registry(nullptr)"));
                return(false);
            }
            
            HGL_DATA_PACKET_TYPE type=dpp->GetPacketType();
            
            if(type<type_start||type>type_end)
            {
                LOG_ERROR(OS_TEXT("Error: DataDistribute::Registry(),packet type =")+OSString(type));
                return(false);
            }
                
            if(proc_list[type-type_start])
            {
                LOG_ERROR(OS_TEXT("Error: DataDistribute::Registry(),proc func repeat set,packet type =")+OSString(type));
                return(false);
            }
            
            proc_list[type-type_start]=dpp;
            return(true);
        }
        
        /**
        * 删除一个数据包自动分发
        */
        bool DataDistribute::Unregistry(HGL_DATA_PACKET_TYPE type)
        {
            if(type<type_start||type>type_end)
            {
                LOG_ERROR(OS_TEXT("Error: DataDistribute::Unregistry(),packet type =")+OSString(type));
                return(false);
            }
            
            if(!proc_list[type-type_start])
            {
                LOG_ERROR(OS_TEXT("Error: DataDistribute::Registry(),proc func is NULL,packet type =")+OSString(type));
                return(false);
            }
            
            proc_list[type-type_start]=nullptr;
            return(true);
        }
        
        /**
        * 清除所有的自动分发处理
        */
        void DataDistribute::Clear()
        {
            hgl_zero(proc_list,type_count);
        }
    }//namespace io
    
    namespace io
    {
        namespace
        {
            class BlockDataDistribute:public DataDistribute
            {
            public:
                
                BlockDataDistribute(DataInputStream *_dis,HGL_DATA_PACKET_TYPE s,HGL_DATA_PACKET_TYPE e):DataDistribute(_dis,s,e)
                {
                }
                
                ErrorCode Update(int max_count)
                {
                    if(!this->dis||this->type_count<=0)
                        return(ddecInitError);
                    
                    HGL_DATA_PACKET_SIZE size;
                    HGL_DATA_PACKET_TYPE type;
                    
                    while(max_count--)
                    {
                        if(!this->dis->ReadInt32(size))
                            return(ddecRecvError);
                        
                        if(size==0)                        //无数据封包，视为心跳包
                            continue;

                        if(!this->dis->ReadInt32(type))
                            return(ddecRecvError);
                        
                        if(type<type_start||type>type_end)
                            return(ddecTypeOut);
                        
                        type-=type_start;
                        
                        if(!proc_list[type])
                            return(ddecTypeNull);
                        
                        if(!proc_list[type]->Parse(dis))
                            return(ddecParseError);
                        
                        if(!proc_list[type]->Process())
                            return(ddecProcError);
                    }
                    
                    return(ddecOK);
                }
            };//class BlockDataDistribute
            
            class NonBlockDataDistribute:public DataDistribute
            {
                HGL_DATA_PACKET_SIZE last_packet_size;                                                    ///<最后一个未解完的数据包长度
                
            public:
                
                NonBlockDataDistribute(DataInputStream *_dis,HGL_DATA_PACKET_TYPE s,HGL_DATA_PACKET_TYPE e):DataDistribute(_dis,s,e)
                {
                    last_packet_size=0;
                }
                
                ErrorCode Update(int max_count)
                {
                    if(!this->dis||this->type_count<=0)
                        return(ddecInitError);
                    
                    HGL_DATA_PACKET_SIZE total=this->dis->Available();
                    
                    if(total<0)
                        return(ddecRecvError);
                    
                    if(total<last_packet_size)
                        return(ddecOK);
                
                    HGL_DATA_PACKET_SIZE size;
                    HGL_DATA_PACKET_TYPE type;
                    
                    while(max_count--)
                    {
                        if(last_packet_size>0)
                        {
                            size=last_packet_size;
                            last_packet_size=0;
                        }
                        else
                        {
                            if(total<sizeof(HGL_DATA_PACKET_SIZE))                    //不足“包长度”这个数据的字节
                                return(ddecOK);

                            if(!this->dis->ReadInt32(size))                            //取得包长
                                return(ddecRecvError);
                            
                            total-=sizeof(HGL_DATA_PACKET_SIZE);
                        
                            if(size==0)                                                //无数据封包，视为心跳包
                                continue;
                            
                            if(total<size)
                            {
                                last_packet_size=size;
                                return(ddecOK);
                            }
                        }

                        if(!this->dis->ReadInt32(type))
                            return(ddecRecvError);
                        
                        total-=sizeof(HGL_DATA_PACKET_TYPE);
                        size-=sizeof(HGL_DATA_PACKET_TYPE);
                        
                        if(type<type_start||type>type_end)
                            return(ddecTypeOut);
                        
                        type-=type_start;
                        
                        if(!proc_list[type])
                            return(ddecTypeNull);
                        
                        if(!proc_list[type]->Parse(dis))
                            return(ddecParseError);
                        
                        total-=size;
                        
                        if(!proc_list[type]->Process())
                            return(ddecProcError);
                    }
                    
                    return(ddecOK);
                }
            };//class NonBlockDataDistribute
        }//namespace
        
        DataDistribute *CreateDataDistribute(DataInputStream *dis,HGL_DATA_PACKET_TYPE start,HGL_DATA_PACKET_TYPE end,bool block)
        {
            if(block)
            {
                return(new BlockDataDistribute(dis,start,end));
            }
            else
            {
                return(new NonBlockDataDistribute(dis,start,end));
            }
        }
    }//namespace io
}//namespace hgl
