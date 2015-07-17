#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		bool RedisDB::ZAdd(const char *set,int64 score,const char *member)
		{
			if(!set||!(*set)||!member||!(*member))return(-1);

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZADD %s " HGL_FMT_I64 " %s",set,score,member);

			return_integer;
		}

		bool RedisDB::ZAdd(const char *set,int count,int64 *scores,const char **members)
		{
			if(!set||!(*set)||count<=0||!scores||!members)return(-1);

			char str[1024]="ZADD ";
			char tmp[32];
			char *p;
			int len;

			strcpy(str+5,1024-5,set);

			p=str+strlen(str);

			for(int i=0;i<count;i++)
			{
				*p++=' ';
				len=itos_rl(tmp,32,scores[i]);
				memcpy(p,tmp,len);
				p+=len;
				*p++=' ';
				len=strlen(members[i]);
				memcpy(p,members[i],len);
				p+=len;
			}

			*p=0;

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),str);
			return_integer;
		}

		bool RedisDB::ZRem(const redis_string &set,const redis_string &member)
		{
			if(set.IsEmpty()||member.IsEmpty())return(false);

			const char *argv[]=
			{
				"ZREM",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)set.Length(),
				(size_t)member.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_integer;
		}

		bool RedisDB::ZRem(const UTF8String &set,int count,const char **members)
		{
			if(!set||!(*set)||count<=0||!members)return(-1);

			return MultiParam("ZREM",set,count,members);
		}

		bool RedisDB::ZRemRangeByRank(const char *set,int64 start,int64 top)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZREMRANGEBYRANK %s " HGL_FMT_I64 " " HGL_FMT_I64,set,start,top);

			return_integer;
		}

		bool RedisDB::ZRemRangeByScore(const char *set,int64 min_score,int64 max_score)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZREMRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_integer;
		}

		bool RedisDB::ZInc(const UTF8String &set,const UTF8String &member,const UTF8String &value,UTF8String &result)
		{
			if(set.IsEmpty()||member.IsEmpty()||value==0)return(false);

			const char *argv[]=
			{
				"ZINCRBY",
				set.c_str(),
				value.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)set.Length(),
				(size_t)value.Length(),
				(size_t)member.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),4,argv,argvlen);

			result=r->str;
			return(true);
		}

		int RedisDB::ZCard(const redis_string &set)
		{
			if(set.IsEmpty())return(false);

			const char *argv[]=
			{
				"ZCARD",
				set.c_str(),
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)set.Length(),
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_integer;
		}

		int RedisDB::ZCount(const char *set,int64 min_score,int64 max_score)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZCOUNT %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_integer;
		}

		int RedisDB::ZRange(const char *set,int64 start,int64 stop,redis_string_list &sl)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZRANGE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,start,stop);

			return_str_array(r,sl);
		}

		int RedisDB::ZRangeByScore(const char *set,int64 min_score,int64 max_score,redis_string_list &sl)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_str_array(r,sl);
		}

		bool RedisDB::ZRange(const char *set,int64 score,redis_string &str)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZRANGE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,score,score);

			return_str_array_first(r,str);
		}

		bool RedisDB::ZRangeByScore(const char *set,int64 score,redis_string &str)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"ZRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,score,score);

			return_str_array_first(r,str);
		}

		bool RedisDB::ZScore(const redis_string &set,const redis_string &member,int64 &score)
		{
			if(set.IsEmpty()||member.IsEmpty())return(false);

			const char *argv[]=
			{
				"ZSCORE",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)set.Length(),
				(size_t)member.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

			stoi(r->str,score);
			return(true);
		}

		bool RedisDB::ZRank(const redis_string &set,const redis_string &member,int64 &rank)
		{
			if(set.IsEmpty()||member.IsEmpty())return(false);

			const char *argv[]=
			{
				"ZRANK",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)set.Length(),
				(size_t)member.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			rank=r->integer;
			return(true);
		}
	}//namespace redis
}//namespace hgl
