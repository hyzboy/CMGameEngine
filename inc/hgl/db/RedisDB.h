#ifndef HGL_DB_REDIS_INCLUDE
#define HGL_DB_REDIS_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

struct redisContext;

namespace hgl
{
	using namespace io;

	namespace redis
	{
		typedef UTF8String redis_string;
		typedef UTF8StringList redis_string_list;
		#define redis_string_list_add(sl,str)	sl.Add(str)

		struct HashsItem
		{
			bool update;

			MemoryOutputStream mos;
			DataOutputStream *dos;

			MemoryInputStream mis;
			DataInputStream *dis;

		public:

			HashsItem()
			{
				update=false;

				dis=new LEDataInputStream(&mis);
				dos=new LEDataOutputStream(&mos);
			}

			HashsItem(const char *data,const int size):HashsItem()
			{
				mos.Write(data,size);
			}

			~HashsItem()
			{
				delete dos;
				delete dis;
			}

			void *GetData(){return mos.GetData();}
			int64 GetSize(){return mos.Tell();}

			const void *GetData()const{return mos.GetData();}
			const int64 GetSize()const{return mos.Tell();}

			template<typename T>	bool GetInteger	(T &v){return hgl::stoi<T>((const char *)mos.GetData(),mos.Tell(),v);}
			template<typename T>	bool GetUInteger(T &v){return hgl::stou<T>((const char *)mos.GetData(),mos.Tell(),v);}
			template<typename T>	bool GetFloat	(T &v){return hgl::stof<T>((const char *)mos.GetData(),mos.Tell(),v);}
									bool GetBool	(bool &v){return hgl::stob((const char *)mos.GetData(),v);}

			DataInputStream *GetDIS()
			{
				if(mos.Tell()<=0)
					return(nullptr);

				mis.Link(mos.GetData(),mos.Tell());

				dis->Restart();
				return dis;
			}

			DataOutputStream *GetDOS()
			{
				mos.ClearData();
				return dos;
			}

			template<typename T>
			bool Write(const T &obj)
			{
				mos.ClearData();
				update=true;

				return obj->Write(dos);
			}

			void Set(const UTF8String &str)
			{
				if(mos.Tell()==str.Length())
				{
					if(memcmp(mos.GetData(),str.c_str(),str.Length())==0)
						return;
				}

				update=true;
				mos.ClearData();

				mos.Write(str.c_str(),str.Length());
				return;
			}
		};//struct HashsItem

		typedef MapObject<UTF8String,HashsItem> redis_hashs_item_list;

		class RedisDB
		{
			redisContext *con;

		private:

			int MultiParam	(const UTF8String &cmd,int number,const char **keys);
			int MultiParam	(const UTF8String &cmd,const UTF8String &param,int number,const char **keys);

		public:

			RedisDB(redisContext *);
			~RedisDB();

			bool Info(UTF8String &);																///<取得数据库信息

			bool SelectDB(int);																		///<选择一个DB
			int	 DBSize();																			///<取得数据库中数据的数量
			bool Ping(UTF8String &);																///<ping
			void Save();																			///<保存数据到磁盘上
			void BGSave();																			///<异步保存数据到磁盘上

			void FlushDB();																			///<清空当前数据库
			void FlushAll();																		///<清空所有数据库

			void Multi();																			///<标记一个事件块的开始
			void Discard();																			///<放弃事件块
			void Exec();																			///<执行事件块中的所有指令

			int Keys(const char *,redis_string_list &);												///<取得符合条件的关键字

		public:	//字符串

			bool Set(const redis_string &,const redis_string &,int=0);								///<设置数据
			bool SetNx(const redis_string &,const redis_string &);									///<设置数据，如果已存在则不做处理
			bool Get(const redis_string &,redis_string &);											///<获取数据
			bool GetSet(const redis_string &,const redis_string &,redis_string &);					///<获取旧的数据，并写入新的数据
			int Del(const redis_string &);															///<删除一个数据
			int  Del(int,const char **);															///<删除数据

			bool RandomKey(redis_string &);															///<随机返回一个key
			bool Exists(const redis_string &);														///<检测一个key是否存在
			bool Rename(const redis_string &,const redis_string &,bool=true);						///<改名

			bool Expire(const char *,int);															///<设置过期时限,单位为秒
			bool Persist(const redis_string &);														///<删除过期时限
			int  TTL(const redis_string &);															///<查询数据过期时限,单位为秒

			int  Strlen(const redis_string &);														///<返回字符串数据的长度

			int Append(const redis_string &,const redis_string &);									///<追加一段数据到原本的数据后面
			bool Get(const char *,int,int,char *);													///<按范围获取数据

		public:	//二进制

			bool SetBinary(const redis_string &,const void *,int,int=0);							///<设置数据
			bool SetNxBinary(const redis_string &,const void *,int);								///<设置数据，如果已存在则不做处理
			int GetBinary(const redis_string &,void *,int);											///<获取数据
			void *GetBinary(const redis_string &,int &);											///<获取数据
			int GetSetBinary(const redis_string &,const void *,int,void *,int);						///<获取旧的数据，并写入新的数据
			int AppendBinary(const redis_string &,const void *,int);								///<追加一段数据到原本的数据后面

		public:	//数字

			bool Incr(const redis_string &,int32 &);												///<原子+1
			bool Incr(const redis_string &,const int32,int32 &);									///<增加一个整数

			bool Incr(const redis_string &,int64 &);												///<原子+1
			bool Incr(const redis_string &,const int64,int64 &);									///<增加一个整数

			bool Incr(const redis_string &,const float,float &);									///<增加一个浮点数

			bool Decr(const redis_string &,int32 &);												///<原子-1
			bool Decr(const redis_string &,const int32,int32 &);									///<减少一个整数

			bool Decr(const redis_string &,int64 &);												///<原子-1
			bool Decr(const redis_string &,const int64,int64 &);									///<减少一个整数

		public:	//集合

			bool SAdd(const redis_string &,const redis_string &);									///<添加一个元素到集合
			bool SAdd(const redis_string &,int,const char **);										///<添加多个元素到集合
			bool SRem(const redis_string &,const redis_string &);									///<从集合中删除一个元素
			bool SRem(const redis_string &,int,const char **);										///<从集合中删除多个元素

			bool SRandMember(const char *,redis_string &,int count=-1);								///<从集合中随机取出一个元素
			bool SPop(const char *,redis_string &);													///<从集合中随机取出一个元素，并从合集中移除它

			bool SMove(const redis_string &,const redis_string &,const redis_string &);				///<将一个元素从一个合集移到另一个合集

			int SCard(const redis_string &);														///<取得集合里的元素数量

			bool SIsMember(const redis_string &,const redis_string &);								///<确认这个元素是否在集合内

			bool SMembers(const redis_string &,redis_string_list &);										///<取得集合里的所有元素

			bool SInter(const redis_string_list &,redis_string_list &);								///<取得指定所有集合的成员交集
			int SInterStore(const char *,const redis_string_list &);								///<将指定所有集合的成员交集存在一个新的集合中

			bool SUnion(const redis_string_list &,redis_string_list &);								///<返回给定的多个集合并集中的所有成员
 			int SUnionStore(const char *,const redis_string_list &);								///<将多个集合并集并存入指定合集

		public:	//有序集合

			bool ZAdd(const char *,int64,const char *);												///<添加一个元素到有序集合
			bool ZAdd(const char *,int,int64 *,const char **);										///<添加多个元素到有序集合
			bool ZRem(const redis_string &,const redis_string &);									///<从集合中删除一个元素
			bool ZRem(const redis_string &,int,const char **);										///<从集合中删除多个元素
			bool ZRemRangeByRank(const char *,int64,int64);											///<从集合中删除指定排名的元素
			bool ZRemRangeByScore(const char *,int64,int64);										///<从集合中删除指定分数的元素

			bool ZInc(const redis_string &,const redis_string &,const redis_string &,redis_string &);///<为集合中的一个元素增加指定的值

			int ZCard(const redis_string &);																///<取得集合里的元素数量
			int ZCount(const char *,int64,int64);													///<取得集合中指定分数范围的元素个数

			int ZRange(const char *,int64,int64,redis_string_list &);								///<取得指定排名间的成员列表
			int ZRangeByScore(const char *,int64,int64,redis_string_list &);						///<取得指定分数间的成员列表

			bool ZRange(const char *,int64,redis_string &);											///<取得指定排名间的成员列表
			bool ZRangeByScore(const char *,int64,redis_string &);									///<取得指定分数间的成员列表

			bool ZAllMember(const char *zname,redis_string_list &sl){return ZRange(zname,0,-1,sl);}	///<所得集合所有元素

			bool ZScore(const redis_string &,const redis_string &,int64 &);											///<取得指定成员的分数
			bool ZRank(const redis_string &,const redis_string &,int64 &);											///<取得指定成员的排名

		public:	//哈希

			bool HSet(const redis_string &,const redis_string &,const char *,const int);			///<设置一个哈希字段的值
			bool HSet(const redis_string &hash,const redis_string &field,const redis_string &value)	///<设置一个哈希字段的值
			{
				if(hash.IsEmpty())RETURN_FALSE;
				if(field.IsEmpty())RETURN_FALSE;

				return HSet(hash,field,value.c_str(),value.Length());
			}

			bool HSet(const redis_string &hash,const redis_string &field,const HashsItem *hi)
			{
				if(hash.IsEmpty())RETURN_FALSE;
				if(field.IsEmpty())RETURN_FALSE;
				if(!hi)RETURN_FALSE;

				return HSet(hash,field,(char *)(hi->GetData()),hi->GetSize());
			}

			bool HSetNX(const redis_string &,const redis_string &,const redis_string &);			///<设置一个哈希字段的值，如果已存在则不做处理
			bool HSet(const redis_string &,const redis_string_list &,const redis_string_list &);	///<设置一个哈希多个字段的值
			bool HSet(const redis_string &,const redis_hashs_item_list &);							///<设置一个哈希多个字段的值
			int HSetUpdate(const redis_string &,const redis_hashs_item_list &);						///<设置一个哈希变动过的多个字段的值

			bool HGet(const redis_string &,const redis_string &,redis_string &);					///<获取一个哈希指定字段的值
			int HGet(const redis_string &,const redis_string_list &,redis_string_list &);			///<获取一个哈希多个字段的值
			int HGetAll(const redis_string &,redis_string_list &,redis_string_list &);				///<获取一个哈希所有字段的值
			int HGetAll(const redis_string &,redis_hashs_item_list &);								///<获取一个哈希所有字段的值
			int HVals(const redis_string &,redis_string_list &);									///<获取一个哈希所有字段的值

			bool HIncr(const char *,const char *,int &);											///<增加一个哈希字段的值
			bool HIncrByFloat(const char *,const char *,double &);									///<增加一个哈希字段的值

			bool HDel(const redis_string &,const redis_string &);									///<删除一个哈希字段
			bool HDel(const redis_string &,const redis_string_list &);								///<删除多个哈希字段

			bool HExists(const redis_string &,const redis_string &);								///<确认一个哈希字段是否存在

			bool HKeys(const redis_string &,redis_string_list &);									///<获取一个哈希值的所有字段列表
			int HLen(const redis_string &);															///<获取一个哈希值的字段数量
		};//class RedisDB

		RedisDB *ConnectRedisDB(const char *,unsigned int/*6379*/);									///<使用ip与port连接一个RedisDB
		RedisDB *ConnectRedisDB(const char *);														///<使用unix path连接一个RedisDB
	}//namespace redis
}//namespace hgl
#endif//HGL_DB_REDIS_INCLUDE
