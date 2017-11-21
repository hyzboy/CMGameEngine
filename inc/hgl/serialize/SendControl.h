#ifndef HGL_SERIALIZE_SEND_CONTROL_INCLUDE
#define HGL_SERIALIZE_SEND_CONTROL_INCLUDE

#include<hgl/serialize/serialize.h>
namespace hgl
{
    namespace serialize
    {
        /**
        * 发送控制
        */
        template<typename ID> class SendControl:public Serialize
        {
        public:

            typedef DefEvent(bool,SendControlSendFunc,(const ID id,void *,int));

            SendControlSendFunc SendData;

        protected:

            virtual bool SendToUser(const ID id)
            {
                if(!SendData)RETURN_FALSE;

                SafeCallEvent(SendData,(id,mb.data(),mb.bytes()));
            }

            virtual bool SendToUser(const ID *id_list,const int count)
            {
                for(int i=0;i<count;i++)
                {
                    SendToUser(*id_list);
                    ++id_list;
                }
            }

        public:

            using Serialize::Serialize;

            SendControl(SendControlSendFunc send_func):Serialize()
            {
                SendData=send_func;
            }

            virtual ~SendControl()=default;

            template<typename ...ARGS> bool OnlySend(ARGS...args)
            {
                return SendToUser(args...);
            }

            void Clear()
            {
                mb.ClearData();
            }

            template<typename ...ARGS> bool Send(ARGS...args)
            {
                bool result=SendToUser(args...);

                Clear();
                return result;
            }

            template<typename T,typename ...ARGS> bool SendMsg(const T *msg,ARGS...args)
            {
                if(!Append(msg))
                {
                    Clear();
                    return(false);
                }

                return Send(args...);
            }
        };//class SendControl
    }//namespace serialize
}//namespace hgl
#endif//HGL_SERIALIZE_SEND_CONTROL_INCLUDE
