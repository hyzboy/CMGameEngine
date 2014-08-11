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
		bool RedisDB::SetBinary(const char *key,const void *value,int size,int et)
		{
			if(et>0)
			{
				REPLY r(con,"SETEX %s %d %b",key,et,value,size);

				if(!r)return(false);
			}
			else
			{
				REPLY r(con,"SET %s %b",key,value,size);

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
		bool RedisDB::SetNxBinary(const char *key,const void *value,int size)
		{
			REPLY r(con,"SETNX %s %b",key,value,size);

			return_bool;
		}

		/**
		 * 获取一个数据
		 * @param key 键值
		 * @param result 结果存放内存
		 * @param max_size 内存最大长度
		 * @return 取出的数据长度
		 */
		int RedisDB::GetBinary(const char *key,void *result,int max_size)
		{
			REPLY r(con,"GET %s",key);

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
		void *RedisDB::GetBinary(const char *key,int &size)
		{
			REPLY r(con,"GET %s",key);

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
		int RedisDB::GetSetBinary(const char *key,const void *data,int size,void *result,int max_size)
		{
			REPLY r(con,"GETSET %s %b",key,data,size);

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
		int RedisDB::AppendBinary(const char *key,const void *data,int size)
		{
			REPLY r(con,"APPEND %s %b",key,data,size);

			return_integer;
		}
	}//namespace redis
}//namespace hgl
