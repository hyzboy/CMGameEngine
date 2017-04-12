#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		/**
		* 设置一个数据
		* @param key 健值
		* @param value 内容
		* @param size 长度
		* @param et 过期时间(以秒为单位,0表示永不过期)
		*/
		bool RedisDB::SetBinary(const UTF8String &key,const void *value,int size,int et)
		{
			if(et>0)
			{
				UTF8String et_str(et);

				const char *argv[]=
				{
					"SETEX",
					key.c_str(),
					et_str.c_str(),
					(const char *)value
				};

				const size_t argvlen[]=
				{
					5,
					(size_t)key.Length(),
					(size_t)et_str.Length(),
					(size_t)size
				};

				REPLY r(REDIS_REPLY_DEBUG_HEADER(con),4,argv,argvlen);

				if(!r)return(false);
			}
			else
			{
				const char *argv[]=
				{
					"SET",
					key.c_str(),
					(const char *)value
				};

				const size_t argvlen[]=
				{
					3,
					(size_t)key.Length(),
					(size_t)size
				};

				REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

				if(!r)return(false);
			}

			return(true);
		}

		/**
		* 设置一个数据，如果key已经存在，则放弃操作
		* @param key 健值
		* @param value 内容
		* @param size 长度
		*/
		bool RedisDB::SetNxBinary(const redis_string &key,const void *value,int size)
		{
			const char *argv[]=
			{
				"SETNX",
				key.c_str(),
				(const char *)value
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)key.Length(),
				(size_t)size
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_bool;
		}

		/**
		 * 获取一个数据
		 * @param key 键值
		 * @param result 结果存放内存
		 * @param max_size 内存最大长度
		 * @return 取出的数据长度
		 */
		int RedisDB::GetBinary(const redis_string &key,void *result,int max_size)
		{
			const char *argv[]=
			{
				"GET",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
				(size_t)key.Length(),
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			const int len=r->len;

			if(len<max_size)
				memcpy(result,r->str,len);
			else
				memcpy(result,r->str,max_size);

			return(len);
		}

		/**
		 * 获取一个数据
		 * @param key 键值
		 * @param size 获取出的数据长度
		 * @return 取出的数据
		 */
		void *RedisDB::GetBinary(const redis_string &key,int &size)
		{
			const char *argv[]=
			{
				"GET",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
				(size_t)key.Length(),
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(nullptr);

			if(r->type!=REDIS_REPLY_STRING)
				return(nullptr);

			size=r->len;

			if(size<=0)
				return(nullptr);

			void *result=new char[size];

			memcpy(result,r->str,size);
			return(result);
		}

		/**
		* 取得旧的数据，并写入新的数据
		* @param key 要操作的键
		* @param data 要写入的新的值
		* @param size 要写入的新数据的长度
		* @param result 取出的旧值所放入的位置
		* @param max_size 最大存放长度
		*/
		int RedisDB::GetSetBinary(const redis_string &key,const void *data,int size,void *result,int max_size)
		{
			const char *argv[]=
			{
				"GETSET",
				key.c_str(),
				(const char *)data
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)size
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			if(r->len<max_size)
			{
				memcpy(result,r->str,r->len);
				return r->len;
			}
			else
			{
				memcpy(result,r->str,max_size);
				return max_size;
			}
		}

		/**
		* 追加一段数据到原本的数据上
		* @param key 要追加数据的键
		* @param value 要追加的数据
		* @param size 要追加的数据长度
		* @return 追加数据后新的数据长度,返回-1表示出错
		*/
		int RedisDB::AppendBinary(const redis_string &key,const void *data,int size)
		{
			const char *argv[]=
			{
				"APPEND",
				key.c_str(),
				(const char *)data
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)size
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_integer;
		}
	}//namespace redis
}//namespace hgl
