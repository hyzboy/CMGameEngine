#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		bool RedisDB::SAdd(const redis_string &set,const redis_string &member)
		{
			if(set.IsEmpty()||member.IsEmpty())RETURN_FALSE;

			const char *argv[]=
			{
				"SADD",
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

		bool RedisDB::SAdd(const UTF8String &set,int count,const char **members)
		{
			if(!set||!(*set)||count<=0||!members)return(-1);

			return MultiParam("SADD",set,count,members);
		}

		bool RedisDB::SRem(const redis_string &set,const redis_string &member)
		{
			if(set.IsEmpty()||member.IsEmpty())RETURN_FALSE;

			const char *argv[]=
			{
				"SREM",
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

		bool RedisDB::SRem(const UTF8String &set,int count,const char **members)
		{
			if(!set||!(*set)||count<=0||!members)return(-1);

			return MultiParam("SREM",set,count,members);
		}

		bool RedisDB::SRandMember(const char *key,redis_string &result,int count)
		{
			if(!key||!(*key))return(false);

			if(count>0)
			{
				REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SRANDMEMBER %s %d",key,count);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}
			else
			{
				REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SRANDMEMBER %s",key);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}

			return(true);
		}

		bool RedisDB::SPop(const char *key,redis_string &result)
		{
			if(!key||!(*key))return(false);

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SPOP %s",key);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			result=r->str;
			return(true);
		}

		bool RedisDB::SMove(const redis_string &src,const redis_string &dst,const redis_string &member)
		{
// 			if(!src||!(*src)
// 			 ||!dst||!(*dst)
// 			 ||!member||!(*member))return(false);
//
// 			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SMOVE %s %s %s",src,dst,member);

            if(src.IsEmpty()
             ||dst.IsEmpty()
             ||member.IsEmpty())RETURN_FALSE;

            const char *argv[]=
            {
                "SMOVE",
                src.c_str(),
                dst.c_str(),
                member.c_str()
            };

            const size_t argvlen[]=
            {
                5,
                (size_t)src.Length(),
                (size_t)dst.Length(),
                (size_t)member.Length()
            };

            REPLY r(REDIS_REPLY_DEBUG_HEADER(con),4,argv,argvlen);

			return_integer;
		}

		int RedisDB::SCard(const redis_string &set)
		{
            if(set.IsEmpty())return(-1);

			//REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SCARD %s",set);

            const char *argv[]=
            {
                "SCARD",
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

		bool RedisDB::SIsMember(const redis_string &set,const redis_string &member)
		{
			if(set.IsEmpty()||member.IsEmpty())RETURN_FALSE;

			const char *argv[]=
			{
				"SISMEMBER",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				9,
				(size_t)set.Length(),
				(size_t)member.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_integer;
		}

		bool RedisDB::SMembers(const redis_string &set,redis_string_list &result_list)
		{
			//REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"SMEMBERS %s",set);

            const char *argv[]=
            {
                "SMEMBERS",
                set.c_str(),
            };

            const size_t argvlen[]=
            {
                8,
                (size_t)set.Length(),
            };

            REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_str_array(r,result_list);
		}

		bool RedisDB::SInter(const redis_string_list &set_list,redis_string_list &result_list)
		{
			if(set_list.GetCount()<=1)return(false);

			UTF8String str="SINTER ";

			const int count=set_list.GetCount();

			for(int i=0;i<set_list.GetCount();i++)
			{
				str+=set_list[i];
				str+=' ';
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),str);
			return_str_array(r,result_list);
		}

		int RedisDB::SInterStore(const char *dst_set,const redis_string_list &set_list)
		{
			if(!dst_set||!(*dst_set))return(-1);
			if(set_list.GetCount()<=1)return(-2);

			UTF8String str="SINTERSTORE ";

			str+=dst_set;
			str+=' ';

			const int count=set_list.GetCount();

			for(int i=0;i<set_list.GetCount();i++)
			{
				str+=set_list[i];
				str+=' ';
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),str);
			return_integer;
		}

		bool RedisDB::SUnion(const redis_string_list &set_list,redis_string_list &result_list)
		{
			if(set_list.GetCount()<=1)return(false);

			UTF8String str="SUNION ";

			const int count=set_list.GetCount();

			for(int i=0;i<set_list.GetCount();i++)
			{
				str+=set_list[i];
				str+=' ';
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),str);
			return_str_array(r,result_list);
		}

		int RedisDB::SUnionStore(const char *dst_set,const redis_string_list &set_list)
		{
			if(!dst_set||!(*dst_set))return(-1);
			if(set_list.GetCount()<=1)return(-2);

			UTF8String str="SUNIONSTORE ";

			str+=dst_set;
			str+=' ';

			const int count=set_list.GetCount();

			for(int i=0;i<set_list.GetCount();i++)
			{
				str+=set_list[i];
				str+=' ';
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),str);
			return_integer;
		}
	}//namespace redis
}//namespace hgl
