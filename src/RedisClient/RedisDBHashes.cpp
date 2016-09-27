#include<hgl/db/RedisDB.h>
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
		bool RedisDB::HSet(const redis_string &key,const redis_string &field,const char *value,const int size)
		{
			const char *argv[]=
			{
				"HSET",
				key.c_str(),
				field.c_str(),
				value
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length(),
				(size_t)field.Length(),
				(size_t)size
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),4,argv,argvlen);

			return_bool;
		}

		bool RedisDB::HSetNX(const redis_string &key,const redis_string &field,const redis_string &value)
		{
			const char *argv[]=
			{
				"HSETNX",
				key.c_str(),
				field.c_str(),
				value.c_str()
			};

			const size_t argvlen[]=
			{
				6,
				(size_t)key.Length(),
				(size_t)field.Length(),
				(size_t)value.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),4,argv,argvlen);

			return_bool;
		}

		bool RedisDB::HSet(const redis_string &key,const redis_string_list &field,const redis_string_list &value)
		{
			if(!key||!(*key))return(false);

			const int count=field.GetCount();

			if(count<=0)return(true);

			if(count!=value.GetCount())return(false);

			const char hmset[]="HMSET";

			const int argc=(count*2)+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmset;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.Length();

			int index=2;

			UTF8String **fp=field.GetDataList();
			UTF8String **vp=value.GetDataList();

			for(int i=0;i<count;i++)
			{
				argv[index]=(*fp)->c_str();
				argvlen[index]=(*fp)->Length();

				++index;

				argv[index]=(*vp)->c_str();
				argvlen[index]=(*vp)->Length();

				++index;

				++fp;
				++vp;
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;
			return_status;
		}

		bool RedisDB::HSet(const redis_string &key,const redis_hashs_item_list &item_list)
		{
			if(!key||!(*key))return(false);

			const int count=item_list.GetCount();

			if(count<=0)return(true);

			const char hmset[]="HMSET";

			const int argc=(count*2)+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmset;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.Length();

			int index=2;

			auto **idp=item_list.GetDataList();

			for(int i=0;i<count;i++)
			{
				argv[index]=(*idp)->left.c_str();
				argvlen[index]=(*idp)->left.Length();

				++index;

				argv[index]=(const char *)((*idp)->right->mos.GetData());
				argvlen[index]=(*idp)->right->mos.Tell();

				++index;
				++idp;
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;
			return_status;
		}

		int RedisDB::HSetUpdate(const redis_string &key,const redis_hashs_item_list &item_list)
		{
			int update_count=0;

			const int count=item_list.GetCount();

			auto **idp=item_list.GetDataList();

			for(int i=0;i<count;i++)
			{
				if((*idp)->right->update)
					++update_count;

				++idp;
			}

			if(update_count<=0)
				return(update_count);

			const char hmset[]="HMSET";

			const int argc=(update_count*2)+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmset;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.Length();

			int index=2;

			idp=item_list.GetDataList();

			for(int i=0;i<count;i++)
			{
				if((*idp)->right->update)
				{
					argv[index]=(*idp)->left.c_str();
					argvlen[index]=(*idp)->left.Length();

					++index;

					argv[index]=(const char *)((*idp)->right->mos.GetData());
					argvlen[index]=(*idp)->right->mos.Tell();

					++index;
				}

				++idp;
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;

			if(!status_result(r))
				return(-update_count);

			idp=item_list.GetDataList();

			for(int i=0;i<count;i++)
			{
				(*idp)->right->update=false;
				++idp;
			}

			return(update_count);
		}

		bool RedisDB::HGet(const redis_string &key,const redis_string &field,redis_string &value)
		{
			const char *argv[]=
			{
				"HGET",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length(),
				(size_t)field.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			value.Set(r->str,r->len);
			return(true);
		}

		int RedisDB::HGet(const redis_string &key,const redis_string_list &field,redis_string_list &result_list)
		{
			const char hmget[]="HMGET";

			const int count=field.GetCount();

			const int argc=count+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmget;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.Length();

			int index=2;

			auto **sp=field.GetDataList();

			for(int i=0;i<count;i++)
			{
				argv[index]=(*sp)->c_str();
				argvlen[index]=(*sp)->Length();

				++index;
				++sp;
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;

			return_str_array(r,result_list);
		}

		int RedisDB::HGetAll(const redis_string &key,redis_string_list &field,redis_string_list &value)
		{
			const char *argv[]=
			{
				"HGETALL",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_pair_str_array(r,field,value);
		}

		int RedisDB::HGetAll(const redis_string &key,redis_hashs_item_list &result)
		{
			const char *argv[]=
			{
				"HGETALL",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_hashs_pair(r,result);
		}

		int RedisDB::HVals(const redis_string &key,redis_string_list &result_list)
		{
			const char *argv[]=
			{
				"HVALS",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_str_array(r,result_list);
		}

		bool RedisDB::HIncr(const char *key,const char *field,int &value)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"HINCRBY %s %s %d",key,field,value);

			if(r->type!=REDIS_REPLY_INTEGER)
				return(false);

			value=r->integer;
			return(true);
		}

		bool RedisDB::HIncrByFloat(const char *key,const char *field,double &value)
		{
			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),"HINCRBY %s %s " HGL_FMT_DOUBLE,key,field,value);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			return stof(r->str,value);
		}

		bool RedisDB::HDel(const redis_string &key,const redis_string &field)
		{
			const char *argv[]=
			{
				"HDEL",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length(),
				(size_t)field.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_bool;
		}

		bool RedisDB::HDel(const redis_string &key,const redis_string_list &field)
		{
			const char hdel[]="HDEL";

			const int count=field.GetCount();

			const int argc=count+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hdel;
			argvlen[0]=4;

			argv[1]=key.c_str();
			argvlen[1]=key.Length();

			int index=2;

			auto **sp=field.GetDataList();

			for(int i=0;i<count;i++)
			{
				argv[index]=(*sp)->c_str();
				argvlen[index]=(*sp)->Length();

				++index;
				++sp;
			}

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;

			return_bool;
		}

		bool RedisDB::HExists(const redis_string &key,const redis_string &field)
		{
			const char *argv[]=
			{
				"HEXISTS",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)key.Length(),
				(size_t)field.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),3,argv,argvlen);

			return_bool;
		}

		bool RedisDB::HKeys(const redis_string &key,redis_string_list &result_list)
		{
			const char *argv[]=
			{
				"HKEYS",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_str_array(r,result_list);
		}

		int RedisDB::HLen(const redis_string &key)
		{
			const char *argv[]=
			{
				"HLEN",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.Length()
			};

			REPLY r(REDIS_REPLY_DEBUG_HEADER(con),2,argv,argvlen);

			return_integer;
		}
	}//namespace redis
}//namespace hgl
