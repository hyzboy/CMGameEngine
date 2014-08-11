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
				set.Length(),
				member.Length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SADD %s %s",set,member);

			return_integer;
		}

		bool RedisDB::SAdd(const char *set,int count,const char **members)
		{
			if(!set||!(*set)||count<=0||!members)return(-1);

			char str[1024]="SADD ";

			strcpy(str+5,set);

			return MultiParam(str,count,members);
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
				set.Length(),
				member.Length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SREM %s %s",set,member);

			return_integer;
		}

		bool RedisDB::SRem(const char *set,int count,const char **members)
		{
			if(!set||!(*set)||count<=0||!members)return(-1);

			char str[1024]="SREM ";

			strcpy(str+5,set);

			return MultiParam(str,count,members);
		}

		bool RedisDB::SRandMember(const char *key,redis_string &result,int count)
		{
			if(!key||!(*key))return(false);

			if(count>0)
			{
				REPLY r(con,"SRANDMEMBER %s %d",key,count);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}
			else
			{
				REPLY r(con,"SRANDMEMBER %s",key);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}

			return(true);
		}

		bool RedisDB::SPop(const char *key,redis_string &result)
		{
			if(!key||!(*key))return(false);

			REPLY r(con,"SPOP %s",key);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			result=r->str;
			return(true);
		}

		bool RedisDB::SMove(const char *src,const char *dst,const char *member)
		{
			if(!src||!(*src)
			 ||!dst||!(*dst)
			 ||!member||!(*member))return(false);

			REPLY r(con,"SMOVE %s %s %s",src,dst,member);

			return_integer;
		}

		int RedisDB::SCard(const char *set)
		{
			if(!set||!(*set))return(-1);

			REPLY r(con,"SCARD %s",set);

			return_integer;
		}

		bool RedisDB::SIsMember(const char *set,const char *member)
		{
			if(!set||!(*set)||!member||!(*member))return(-1);

			REPLY r(con,"SISMEMBER %s %d",set,member);

			return_integer;
		}

		bool RedisDB::SMembers(const char *set,redis_string_list &result_list)
		{
			REPLY r(con,"SMEMBERS %s",set);

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

			REPLY r(con,str);
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

			REPLY r(con,str);
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

			REPLY r(con,str);
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

			REPLY r(con,str);
			return_integer;
		}
	}//namespace redis
}//namespace hgl
