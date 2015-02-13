#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		bool RedisDB::Incr(const redis_string &key,int32 &result)
		{
			const char *argv[]=
			{
				"INCR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const int32 value,int32 &result)
		{
			UTF8String val_str(value);

			const char *argv[]=
			{
				"INCRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)val_str.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,int64 &result)
		{
			const char *argv[]=
			{
				"INCR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const int64 value,int64 &result)
		{
			UTF8String val_str(value);

			const char *argv[]=
			{
				"INCRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)val_str.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const float value,float &result)
		{
			UTF8String val_str(value);

			const char *argv[]=
			{
				"INCRBYFLOAT",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				11,
				(size_t)key.Length(),
				(size_t)val_str.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

			return stof(r->str,r->len,result);
		}

		bool RedisDB::Decr(const redis_string &key,int32 &result)
		{
			const char *argv[]=
			{
				"DECR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,const int32 value,int32 &result)
		{
			UTF8String val_str(value);

			const char *argv[]=
			{
				"DECRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)val_str.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,int64 &result)
		{
			const char *argv[]=
			{
				"DECR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,const int64 value,int64 &result)
		{
			UTF8String val_str(value);

			const char *argv[]=
			{
				"DECRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)val_str.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}
	}//namespace redis
}//namespace hgl
