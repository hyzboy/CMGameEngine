#ifndef HGL_NETWORK_SYNC_INCLUDE
#define HGL_NETWORK_SYNC_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	namespace network
	{
		/**
		* 数据同步对象
		*/
		class SyncObject																			///同步对象
		{
		protected:

			uint64 tick_count;																		///<更新计数器

		public:

			SyncObject(){tick_count=0;}
			virtual ~SyncObject()=default;

			virtual bool Update(){return false;}
		};//class SyncObject

		/**
		* 原生数据同步对象<br>
		* 适用于C/C++中的原生数据类型，直接使用内存复制手段传输
		*/
		class SyncOrignData																			///原生数据同步对象
		{
		public:

			char *data;																				///<数据
			int size;																				///<数据长度

		public:

			SyncOrignData();
			virtual ~SyncOrignData();

			virtual bool Update();
		};//class SyncOrignData

		/**
		* 复合数据同步对象<br>
		* 适用于复杂数据，使用流式化处理手段
		*/
		class SyncComplexData																		///<复合数据同步对象
		{
		public:
		};//class SyncComplexData

		/**
		* 数据自动同步管理类
		*/
		class SyncManage
		{
		public:


		};//class SyncManage
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SYNC_INCLUDE
