#ifndef HGL_REDISDB_REPLY_INCLUDE
#define HGL_REDISDB_REPLY_INCLUDE

#include"hiredis.h"
namespace hgl
{
	namespace redis
	{
		class REPLY
		{
			redisReply *reply;

		public:

			REPLY(redisContext *con,const char *format,...)
			{
				va_list ap;

// #ifndef NO_LOGINFO
// 				char tmp[1024];
// 				va_start(ap,format);
// 				vsprintf(tmp,format,ap);
// 				va_end(ap);
//
// 				LOG_INFO(tmp);
// #endif//NO_LOGINFO

				va_start(ap,format);
				reply = (redisReply *)redisvCommand(con,format,ap);
				va_end(ap);

				if(!reply)
				{
					printf("redisvCommand return NULL.\n");
					return;
				}

				if(reply->type==REDIS_REPLY_ERROR)
				{
					printf("redisvCommand return error: %s\n",reply->str);
				}
			}

			REPLY(redisContext *con,int argc,const char **argv,const size_t *argvlen)
			{
				reply=(redisReply *)redisCommandArgv(con,argc,argv,argvlen);

				if(!reply)
				{
					printf("redisvCommand return NULL.\n");
					return;
				}

				if(reply->type==REDIS_REPLY_ERROR)
				{
					printf("redisvCommand return error: %s\n",reply->str);
				}
			}

			~REPLY()
			{
				if(reply)
					freeReplyObject(reply);
			}

			const bool operator !()const
			{
				if(!reply)return(true);
				if(reply->type==REDIS_REPLY_ERROR)
					return(true);

				return(false);
			}

			const redisReply *operator ->()const{return reply;}
		};//class REPLY

		inline bool status_result(const REPLY &r)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STATUS)return(false);

// 			LOG_INFO("Redis return status");
			return true;
		}

		inline int integer_result(const REPLY &r)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_INTEGER)return(-1);

// 			LOG_INFO("Redis return integer: "+UTF8String(r->integer));
			return r->integer;
		}

		inline bool bool_result(const REPLY &r)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

// 			LOG_INFO("Redis return bool: "+UTF8String((r->integer?"true":"false")));
			return r->integer;
		}

		inline int string_array_result(const REPLY &r,redis_string_list &sl)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

// 			LOG_INFO("Redis return string array: "+UTF8String(r->elements));

			for(int i=0;i<r->elements;i++)
				redis_string_list_add(sl,redis_string(r->element[i]->str,r->element[i]->len));

			return r->elements;
		}

		inline bool string_array_first_result(const REPLY &r,redis_string &str)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_ARRAY)return(false);

// 			LOG_INFO("Redis return string array first: "+UTF8String(r->elements));

			if(r->elements<=0)
				return(false);

			str.Set(r->element[0]->str,r->element[0]->len);

			return true;
		}

		inline int pair_string_array_result(const REPLY &r,redis_string_list &field_list,redis_string_list &value_list)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

// 			LOG_INFO("Redis return string array: "+UTF8String(r->elements));

			int index=0;
			const int count=r->elements/2;

			for(int i=0;i<count;i++)
			{
				redis_string_list_add(field_list,redis_string(r->element[index]->str,r->element[index]->len));++index;
				redis_string_list_add(value_list,redis_string(r->element[index]->str,r->element[index]->len));++index;
			}

			return count;
		}

		inline int hashs_pair_result(const REPLY &r,redis_hashs_item_list &result)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

			int index=0;
			const int count=r->elements/2;

			for(int i=0;i<count;i++)
			{
				UTF8String key(r->element[index]->str,r->element[index]->len);

				++index;

				result.Add(key,new HashsItem(r->element[index]->str,r->element[index]->len));

				++index;
			}

			return count;
		}

		#define return_status					return(status_result(r))
		#define return_integer					return(integer_result(r))
		#define return_bool						return(bool_result(r))
		#define return_str_array(r,sl)			return(string_array_result(r,sl))
		#define return_str_array_first(r,str)	return(string_array_first_result(r,str))
		#define return_pair_str_array(r,fl,vl)	return(pair_string_array_result(r,fl,vl))
		#define return_hashs_pair(r,h)			return(hashs_pair_result(r,h))
	}//namespace redis
}//namespace hgl
#endif//HGL_REDISDB_REPLY_INCLUDE
