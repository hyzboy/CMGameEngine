#ifndef HGL_PROTOCOLBUF_MESSAGE_PROCESS_CREATOR_INCLUDE
#define HGL_PROTOCOLBUF_MESSAGE_PROCESS_CREATOR_INCLUDE

#include<hgl/type/Map.h>
#include<google/protobuf/message_lite.h>

namespace hgl
{
	using namespace google;
	using namespace google::protobuf;

	typedef DefEvent(bool,PBCmdFunc,(MessageLite *));

	/**
	* 消息执行对象创建工具类
	*/
	class MessageProcessCreator
	{
		MapObject<int,MessageLite> ml_list;

	public:

		MessageProcessCreator()
		{
		}

		bool Add(int id,MessageLite *ml)
		{
			if(ml_list.Add(id,ml))
				return(true);

			LOG_ERROR(OS_TEXT("repeat add MessageLite,ID:")+OSString(id));
			RETURN_FALSE;
		}

		MessageLite *Parse(int32 msg_id,void *data,int size)
		{
			MessageLite *ml=ml_list[msg_id];

			if(!ml)
			{
				LOG_ERROR(OS_TEXT("MsgID error:")+OSString(msg_id));
				return nullptr;
			}

			MessageLite *obj=ml->New();

			if(obj->ParseFromArray(data,size))
				return obj;

			LOG_ERROR(OS_TEXT("Msg Parse error,MsgID:")+OSString(msg_id));
			delete obj;
			return nullptr;
		}
	};//class MessageProcessCreator
}//namespace hgl
#endif//HGL_PROTOCOLBUF_MESSAGE_PROCESS_CREATOR_INCLUDE
