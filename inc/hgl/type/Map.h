#ifndef HGL_MAP_INCLUDE
#define HGL_MAP_INCLUDE

#include<hgl/type/List.h>
#include<hgl/type/Pair.h>
#include<hgl/type/Pool.h>
#include<hgl/LogInfo.h>
#include<hgl/thread/RWLock.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
namespace hgl
{
	/**
	* 索引数据模板
	*/
	template<typename F,typename T,typename DataPair> class _Map:public _Object
	{
	public:

		typedef DataPair IDItem;

	protected:

		ObjectPool<IDItem> data_pool;
		List<IDItem *> data_list;

		typedef _Map<F,T,IDItem> this_calss;

    public: //事件

		bool (*OnSaveToStream)(io::DataOutputStream *,const F &,T &);								///<保存到流事件
		bool (*OnLoadFromStream)(io::DataInputStream *,F &,T &);									///<从流加载事件

	public:	//方法

		_Map();
		virtual ~_Map()HGL_DEFAULT_MEMFUNC;

		const	int		GetCount()const{return data_list.GetCount();}								///<取得数据总量

		IDItem *		Add(const F &,const T &);													///<添加一个数据，数果索引已存在，返回nullptr
				T &  	Add(const F &);																///<添加一个数据
				void    Add(IDItem *);																///<添加一个数据
				bool	FindPos(const F &,int &)const;												///<查找数据如果插入后，会所在的位置，返回是否存在这个数据
				int		FindPos(const F &flag)const{int pos;FindPos(flag,pos);return(pos);}			///<查找数据如果插入后，会所在的位置
				int		Find(const F &)const;														///<查找数据是否存在，返回-1表示数据不存在
				int		FindByData(const T &)const;													///<查找数据是否存在，返回-1表示数据不存在
		virtual bool	Get(const F &,T &)const;													///<取得数据
		virtual bool	Delete(const F &,T &);														///<将指定数据从列表中移除
		virtual bool	DeleteByIndex(const F &);													///<根据索引将指定数据从列表中移除
		virtual int		DeleteByIndex(const F *,const int);											///<根据索引将指定数据从列表中批量移除
		virtual bool	DeleteByData(const T &);													///<根据数据将指定数据从列表中移除
		virtual bool	DeleteBySerial(int);														///<根据序号将指定数据从列表中移除
		virtual bool	DeleteBySerial(int,int);													///<根据序号将指定数据从列表中移除
		virtual void	Update(const F &,const T &);												///<更新一个数据的内容(如不存在则添加)
		virtual	bool 	Change(const F &,const T &);												///<更改一个数据的内容(如不存在则更改失效)
		virtual void 	Clear();																	///<清除所有数据
		virtual void	ClearData();																///<清除所有数据，但不释放内存

		List<IDItem *> &GetList(){return data_list;}												///<取得线性列表
			IDItem **	GetDataList()const{return data_list.GetData();}								///<取得纯数据线性列表

			template<typename IT>
				int		GetAllIndex(IT &il_list)
				{
					const int count=data_list.GetCount();

					if(count<=0)
						return count;

					IDItem **idp=data_list.GetData();

					for(int i=0;i<count;i++)
					{
						il_list.Add((*idp)->first);
						++idp;
					}

					return count;
				}

				template<typename IT>
				int		GetAllData(IT &il_list)
				{
					const int count=data_list.GetCount();

					if(count<=0)
						return count;

					IDItem **idp=data_list.GetData();

					for(int i=0;i<count;i++)
					{
						il_list.Add((*idp)->second);
						++idp;
					}

					return count;
				}

				IDItem *GetItem(int n)const{return data_list[n];}									///<取指定序号的数据
				bool 	Get(int,F &,T &)const;														///<取指定序号的数据
				bool	GetIndex(int,F &)const;														///<取指定序号的索引
				bool	GetData(int,T &)const;														///<取指定序号的数据

				bool	SetDataBySerial(int,T &);													///<根据序号设置数据

				bool	SaveToStream(io::DataOutputStream *);										///<保存到流
				bool	LoadFromStream(io::DataInputStream *);										///<从流加载

				bool	SaveToFile(const os_char *);												///<保存到文件
				bool	LoadFromFile(const os_char *);                                         		///<从文件加载

				void	operator=(const _Map<F,T,IDItem> &);									///<操作符重载，复制一个列表
	};//class _Map

	template<typename F,typename T> class Map:public _Map<F,T,Pair<F,T> >
	{
	public:

		Map()HGL_DEFAULT_MEMFUNC;
		virtual ~Map()HGL_DEFAULT_MEMFUNC;
	};//class Map

	template<typename T_ID,typename T_U> T_U *GetObject(Map<T_ID,T_U *> &list,const T_ID &id)
	{
		T_U *result;

		if(!list.Get(id,result))
			return(nullptr);

		return result;
	}

	template<typename F,typename T,typename DataPair> class _MapObject:public _Map<F,T *,DataPair>
	{
	protected:

		typedef _Map<F,T *,DataPair> SuperClass;

		virtual void	DeleteObject(const F &,T *)=0;												///<删除一个数据
				void	DeleteObject(DataPair *ds){DeleteObject(ds->left,ds->right);}
				void	DeleteObject(int index){DeleteObject(this->data_list[index]);}

	public:

		_MapObject()HGL_DEFAULT_MEMFUNC;
		virtual ~_MapObject()
		{
			if(SuperClass::GetCount()>0)
			{
				//LOG_ERROR(u"这是一个严重的程序设计错误，会产生纯虚函数调用，请在派生类的析构函数中调用Clear函数以清除数据。");
				LOG_ERROR(OS_TEXT("This is a serious design errors, will produce the pure virtual function call, please call in the destructor of the derived class the Clear function to clear the data."));
			}
		}

		/**
		* 断开一个数据
		* @param flag 要断开的数据标识
		* @return 是否断开成功
		*/
		bool UnlinkByIndex(const F &flag)
		{
			return UnlinkBySerial(SuperClass::Find(flag));
		}

		/**
		* 断开一个数据
		* @param tp 要断开的数据
		* @return 是否断开成功
		*/
		bool UnlinkByData(T *tp)
		{
			return UnlinkBySerial(this->FindByData(tp));
		}

		/**
		* 断开一个数据关联
		* @param index 要断开的数据的序列号
		* @return 是否断开成功
		*/
		bool UnlinkBySerial(int index)
		{
			if(index<0||index>=this->data_list.GetCount())return(false);

			SuperClass::DeleteBySerial(index);

			return(true);
		}

		/**
		* 断开所有数据关联
		*/
		void UnlinkAll()
		{
			SuperClass::Clear();
		}

		/**
		* 删除一个数据
		* @param flag 要删除的数据标识
		* @return 是否删除成功
		*/
		bool DeleteByIndex(const F &flag)
		{
			return DeleteBySerial(SuperClass::Find(flag));
		}

		/**
		* 删除一个数据
		* @param tp 要删除的数据
		* @return 是否删除成功
		*/
		bool DeleteByData(T *tp)
		{
			return DeleteBySerial(this->FindByData(tp));
		}

		/**
		* 删除一个数据
		* @param index 要删除的数据的序列号
		* @return 是否删除成功
		*/
		bool DeleteBySerial(int index)
		{
			if(index<0||index>=this->data_list.GetCount())return(false);

			DeleteObject(index);
			SuperClass::DeleteBySerial(index);

			return(true);
		}

		/**
		* 清除所有数据
		*/
		void DeleteAll()
		{
			int n=this->data_list.GetCount();

			while(n--)
				DeleteObject(n);

			_Map<F,T *,DataPair>::Clear();
		}

		/**
		* 更新数据,如果数据不存在就添加一个新的
		* @param flag 数据标识
		* @param data 新的数据内容
		*/
		void Update(const F &flag,T *data)
		{
			int index=this->Find(flag);

			if(index!=-1)
			{
				DeleteObject(index);

				this->data_list[index]->second=data;
			}
			else
			{
				this->Add(flag,data);
			}
		}

		/**
		* 更改数据,这个更改和Set不同,它要求指定标识的数据必须存在，则否就会更改失败
		* @param flag 数据标识
		* @param data 新的数据内容
		* @param return 是否更改成功
		*/
		bool Change(const F &flag,T *data)
		{
			int index=this->Find(flag);

			if(index!=-1)
			{
				DeleteObject(index);

				this->data_list[index]->second=data;

				return(true);
			}
			else
				return(false);
		}

		void Clear(){DeleteAll();}
	};//class _MapObject

	template<typename F,typename T,typename DataPair> class CusMapObject:public _MapObject<F,T,DataPair>
	{
	protected:

		//virtual T *		CreateObject(const F &){return(new T);}										///<创建一个数据
		virtual void	DeleteObject(const F &,T *obj){delete obj;}									///<删除一个数据

	public:

		CusMapObject()HGL_DEFAULT_MEMFUNC;
		virtual ~CusMapObject()
		{
			_MapObject<F,T,DataPair>::Clear();
		}
	};//class CusMapObject

	template<typename F,typename T> class MapObject:public CusMapObject<F,T,Pair<F,T *> >
	{
	public:

		MapObject()HGL_DEFAULT_MEMFUNC;
		virtual ~MapObject()
		{
			CusMapObject<F,T,Pair<F,T *> >::Clear();
		}

		T *operator[](const F &index)const
		{
			const int pos=this->Find(index);

			if(pos==-1)return(nullptr);

			return this->data_list[pos]->right;
		};
	};//class MapObject
}//namespace hgl
#include<hgl/type/Map.cpp>
#endif//HGL_MAP_INCLUDE
