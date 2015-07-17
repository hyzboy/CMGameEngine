#include<hgl/db/RedisDB.h>
#include"hiredis.h"
#include"RedisDBReply.h"
#include<string.h>
#include<stdlib.h>
#include<hgl/Str.h>

namespace hgl
{
	namespace redis
	{
		RedisDB::RedisDB(redisContext *rc)
		{
			con=rc;
		}

		RedisDB::~RedisDB()
		{
			if(con)
				redisFree(con);
		}

		bool RedisDB::Info(UTF8String &result)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"INFO");

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

            result.Set(r->str,r->len);
			return(true);
		}

		/**
		* 选择一个数据库
		* @param db_index 数据库索引,从0开始
		*/
		bool RedisDB::SelectDB(int db_index)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SELECT %d",db_index);

			if(!r)return(false);

			return(true);
		}

		int RedisDB::DBSize()
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"DBSIZE");

			return_integer;
		}

		bool RedisDB::Ping(UTF8String &result)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"PING");

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STATUS)return(false);

			result.Set(r->str,r->len);
			return(true);
		}

		void RedisDB::Save		(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SAVE");}
		void RedisDB::BGSave	(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"BGSAVE");}

		void RedisDB::FlushDB	(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"FLUSHDB");}
		void RedisDB::FlushAll	(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"FLUSHALL");}

		void RedisDB::Multi		(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"MULTI");}
		void RedisDB::Discard	(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"DISCARD");}
		void RedisDB::Exec		(){REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"EXEC");}

		int RedisDB::MultiParam	(const UTF8String &cmd,int number,const char **keys)
		{
			if(number<=0||!keys)return(-1);

			const char *argv[number+1];
			size_t argv_len[number+1];

			argv[0]=cmd.c_str();
			argv_len[0]=cmd.Length();

			const char *s;

			for(int i=0;i<number;i++)
			{
				argv[i+1]=keys[i];
				argv_len[i+1]=strlen(keys[i]);
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),number+1,argv,argv_len);

			return_integer;
		}

		int RedisDB::MultiParam	(const UTF8String &cmd,const UTF8String &param,int number,const char **keys)
		{
			if(number<=0||!keys)return(-1);

			const char *argv[number+2];
			size_t argv_len[number+2];

			argv[0]=cmd.c_str();
			argv_len[0]=cmd.Length();
			argv[1]=param.c_str();
			argv_len[1]=param.Length();

			const char *s;

			for(int i=0;i<number;i++)
			{
				argv[i+2]=keys[i];
				argv_len[i+2]=strlen(keys[i]);
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),number+2,argv,argv_len);

			return_integer;
		}

		int RedisDB::Keys(const char *str, redis_string_list &key_list)
		{
			if(!str||!*str)return(-1);

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"keys %s",str);

			return_str_array(r,key_list);
		}
	}//namespace redis

	namespace redis
	{
		RedisDB *CreateRedisConnect(redisContext *con)
		{
			if(!con)
				return(nullptr);

			if(con->err)
			{
				printf("redisConnect error:%s\n",con->errstr);
				return(nullptr);
			}

			return(new RedisDB(con));
		}

		RedisDB *ConnectRedisDB(const char *ip,uint port)
		{
			return CreateRedisConnect(redisConnect(ip,port));
		}

		RedisDB *ConnectRedisDB(const char *path)
		{
			return CreateRedisConnect(redisConnectUnix(path));
		}
	}//namespace redis
}//namespace hgl
