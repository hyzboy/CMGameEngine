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

		bool RedisDB::Info(char *result)
		{
			REPLY r(con,"INFO");

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

			strcpy(result,r->str);
			return(true);
		}

		/**
		* 选择一个数据库
		* @param db_index 数据库索引,从0开始
		*/
		bool RedisDB::SelectDB(int db_index)
		{
			REPLY r(con,"SELECT %d",db_index);

			if(!r)return(false);

			return(true);
		}

		int RedisDB::DBSize()
		{
			REPLY r(con,"DBSIZE");

			return_integer;
		}

		bool RedisDB::Ping(char *result)
		{
			REPLY r(con,"PING");

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STATUS)return(false);

			strcpy(result,r->str);
			return(true);
		}

		void RedisDB::Save		(){REPLY r(con,"SAVE");}
		void RedisDB::BGSave	(){REPLY r(con,"BGSAVE");}

		void RedisDB::FlushDB	(){REPLY r(con,"FLUSHDB");}
		void RedisDB::FlushAll	(){REPLY r(con,"FLUSHALL");}

		void RedisDB::Multi		(){REPLY r(con,"MULTI");}
		void RedisDB::Discard	(){REPLY r(con,"DISCARD");}
		void RedisDB::Exec		(){REPLY r(con,"EXEC");}

		int RedisDB::MultiParam	(char *str,int number,const char **keys)
		{
			if(number<=0||!keys)return(-1);

			char *p=str+strlen(str);
			const char *s;

			for(int i=0;i<number;i++)
			{
				*p++=' ';
				s=keys[i];
				while(*s)
					*p++=*s++;
			}

			*p++=0;

			REPLY r(con,str);

			return_integer;
		}

		int RedisDB::Keys(const char *str, redis_string_list &key_list)
		{
			if(!str||!*str)return(-1);

			REPLY r(con,"keys %s",str);

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
