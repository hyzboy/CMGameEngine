#ifndef HGL_IO_DATA_DISTRIBUTE_INCLUDE
#define HGL_IO_DATA_DISTRIBUTE_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;

		typedef int32 HGL_DATA_PACKET_SIZE;
		typedef int32 HGL_DATA_PACKET_TYPE;

		typedef DefEvent(bool,DataPacketProcessFunc,(void *))										///<封包处理事件

		#define HGL_DATA_PACKET_TYPEDEF(data_struct,name)					typedef data_struct name##Packet;
		#define HGL_DATA_PACKET_NAME(name)									name##Packet

		#define HGL_DATA_PACKET_RECV_FUNC_DEF(type,name)					bool OnRecv##name(HGL_DATA_PACKET_NAME(type##name) *);
		#define HGL_DATA_PACKET_RECV_FUNC_REALIZE(type,obj_name,name)		bool obj_name::OnRecv##name(HGL_DATA_PACKET_NAME(type##name) *pack)

		#define HGL_DATA_PACKET_RECV_REGISTRY(obj,obj_name,type,name)	{	\
																			DataPacketProcessFunc dpf;	\
																			\
																			SetEventCall(dpf,obj,obj_name,OnRecv##name);	\
																			\
																			auto_data_dist->Registry(new DataPacketProc(new DataPacketTemplate<type##name,HGL_DATA_PACKET_NAME(type##name)>,dpf));	\
																		}
		/**
		 * 数据包基类，所有的数据包如果想自动分发，必须从本类派生
		 */
		struct DataPacket																			///数据包基类
		{
		public:

			virtual ~DataPacket()=default;

			virtual const HGL_DATA_PACKET_TYPE GetPacketType()const=0;								///<取得数据包类型

			virtual bool Parse(DataInputStream *)=0;												///<数据包的解晰处理函数

			virtual void *GetData()=0;																///<取得数据包
		};//struct DataPacket

		/**
		 * 数据包解释模板类
		 */
		template<HGL_DATA_PACKET_TYPE V,typename T> struct DataPacketTemplate:public DataPacket
		{
		public:

			T pack;

		public:

			~DataPacketTemplate()=default;

			const HGL_DATA_PACKET_TYPE GetPacketType()const											///<取得数据包类型
			{
				return V;
			}

			bool Parse(DataInputStream *dis)
			{
				return pack.Read(dis);
			}

			void *GetData()
			{
				return &pack;
			}
		};//template<HGL_DATA_PACKET_TYPE V,typename T> struct DataPacketTemplate:public DataPacket

		/**
		 * 数据包处理基类，用于数据包的处理执行
		 */
		class DataPacketProc																		///<数据包处理基类
		{
			DataPacket *data_pack;																	///<数据包

			DataPacketProcessFunc proc_func;														///<执行函数

		public:

			DataPacketProc(DataPacket *dp,DataPacketProcessFunc dpf)
			{
				data_pack=dp;
				proc_func=dpf;
			}

			virtual ~DataPacketProc()
			{
				SAFE_CLEAR(data_pack);
			}

			const HGL_DATA_PACKET_TYPE GetPacketType()const											///<取得数据包类型
			{
				return data_pack?data_pack->GetPacketType():-1;
			}

			bool Parse(DataInputStream *dis)														///<解晰数据包
			{
				return data_pack?data_pack->Parse(dis):false;
			}

			bool Process()																			///<数据包处理,需开发者针对具体包重载实现
			{
				if(proc_func==nullptr)return(false);

				return proc_func(data_pack->GetData());
			}
		};//class DataPacketProc

		/**
		 * 数据包自动分发处理类
		 */
		class DataDistribute																		///数据包自动分发处理类
		{
		protected:

			HGL_DATA_PACKET_TYPE type_start,type_end;												///<自动分发类型
			int type_count;																			///<类型数量
			DataPacketProc **proc_list;																///<自动分发处理对象
			DataInputStream *dis;																	///<数据输入流

		public:

			/**
			 * 数据自动分发错误
			 */
			enum ErrorCode
			{
				ddecOK			=0,		///<正确

				ddecInitError	=-1,	///<初始化错误
				ddecRecvError	=-2,	///<接收数据出错
				ddecTypeOut		=-3,	///<类型越界
				ddecTypeNull	=-4,	///<类型无处理函数
				ddecParseError	=-5,	///<解晰错误
				ddecProcError	=-6,	///<执行错误
			};//enum ErrorCode

		public:

			DataDistribute(DataInputStream *,HGL_DATA_PACKET_TYPE start,HGL_DATA_PACKET_TYPE end);	///<数据包自动分发处理类构造函数
			virtual ~DataDistribute();

			bool Registry(DataPacketProc *);														///<注册一个数据包自动分发
			bool Unregistry(HGL_DATA_PACKET_TYPE);													///<注销一个数据包自动分发

			void Clear();																			///<清除所有的自动分发处理

			virtual ErrorCode Update(int max_count=0)=0;											///<刷新处理所有的自动分发
		};//class DataDistribute

		/**
		* 创建一个数据包自动分发处理类
		* @param _dis 数据输入流
		* @param s 起始数据包格式
		* @param e 结束数据包格式
		* @param block 是否阻塞模式
		*/
		DataDistribute *CreateDataDistribute(DataInputStream *,HGL_DATA_PACKET_TYPE,HGL_DATA_PACKET_TYPE,bool block);
	}//namespace io
}//namespace hgl
#endif//HGL_IO_DATA_DISTRIBUTE_INCLUDE
