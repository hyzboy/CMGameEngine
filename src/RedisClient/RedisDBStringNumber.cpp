#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		bool RedisDB::Incr(const char *key,int32 &result)
		{
			REPLY r(con,"INCR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const char *key,const int32 value,int32 &result)
		{
			REPLY r(con,"INCRBY %s %d",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const char *key,int64 &result)
		{
			REPLY r(con,"INCR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const char *key,const int64 value,int64 &result)
		{
			REPLY r(con,"INCRBY %s " HGL_FMT_I64,key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const char *key,const float value,float &result)
		{
			REPLY r(con,"INCRBYFLOAT %s %f",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

			return stof(r->str,r->len,result);
		}

		bool RedisDB::Decr(const char *key,int32 &result)
		{
			REPLY r(con,"DECR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const char *key,const int32 value,int32 &result)
		{
			REPLY r(con,"DECRBY %s %d",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const char *key,int64 &result)
		{
			REPLY r(con,"DECR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const char *key,const int64 value,int64 &result)
		{
			REPLY r(con,"DECRBY %s " HGL_FMT_I64,key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}
	}//namespace redis
}//namespace hgl
