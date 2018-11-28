#ifndef HGL_MAP_INCLUDE
#define HGL_MAP_INCLUDE

#include<hgl/type/List.h>
#include<hgl/type/Pair.h>
#include<hgl/type/Pool.h>
#include<hgl/thread/RWLock.h>
namespace hgl
{
	/**
	* 索引数据模板
	*/
	template<typename F,typename T,typename DataPair> class _Map
	{
	public:

        using IDItem=DataPair;

	protected:

		ObjectPool<IDItem> data_pool;
		List<IDItem *> data_list;

        using this_calss=_Map<F,T,IDItem>;

	public:	//方法

		_Map()=default;
		virtual ~_Map()=default;

		const	int		GetCount()const{return data_list.GetCount();}								///<取得数据总量

		IDItem *		Add(const F &,const T &);													///<添加一个数据，数果索引已存在，返回nullptr
				T &  	Add(const F &);																///<添加一个数据
				void    Add(IDItem *);																///<添加一个数据
				bool	FindPos(const F &,int &)const;												///<查找数据如果插入后，会所在的位置，返回是否存在这个数据
				int		FindPos(const F &flag)const{int pos;FindPos(flag,pos);return(pos);}			///<查找数据如果插入后，会所在的位置
				int		Find(const F &)const;														///<查找数据是否存在，返回-1表示数据不存在
				int		FindByValue(const T &)const;												///<查找数据是否存在，返回-1表示数据不存在
				bool    KeyExist(const F &key)const{return(Find(key)!=-1);}                         ///<确认这个数据是否存在
				bool    ValueExist(const T &value)const{return(FindByValue(value)!=-1);}            ///<确认这个数据是否存在
		virtual bool	Get(const F &,T &)const;													///<取得数据
		virtual bool	Delete(const F &,T &);														///<将指定数据从列表中移除，并获得这个数据
		virtual bool	DeleteByKey(const F &);													    ///<根据索引将指定数据从列表中移除
		virtual int		DeleteByKey(const F *,const int);											///<根据索引将指定数据从列表中批量移除
		virtual bool	DeleteByValue(const T &);													///<根据数据将指定数据从列表中移除
		virtual bool	DeleteBySerial(int);														///<根据序号将指定数据从列表中移除
		virtual bool	DeleteBySerial(int,int);													///<根据序号将指定数据从列表中移除
		virtual void	Update(const F &,const T &);												///<更新一个数据的内容(如不存在则添加)
		virtual	bool 	Change(const F &,const T &);												///<更改一个数据的内容(如不存在则更改失效)
		virtual void 	Clear();																	///<清除所有数据
		virtual void	ClearData();																///<清除所有数据，但不释放内存

		List<IDItem *> &GetList(){return data_list;}												///<取得线性列表
			IDItem **	GetDataList()const{return data_list.GetData();}								///<取得纯数据线性列表

                template<typename IT>
				int		GetKey(IT &il_list)                                                        ///<取得所有索引合集
				{
					const int count=data_list.GetCount();

					if(count<=0)
						return count;

					IDItem **idp=data_list.GetData();

					for(int i=0;i<count;i++)
					{
						il_list.Add((*idp)->left);
						++idp;
					}

					return count;
				}

				template<typename IT>
				int		GetValue(IT &il_list)                                                      ///<取得所有数值合集
				{
					const int count=data_list.GetCount();

					if(count<=0)
						return count;

					IDItem **idp=data_list.GetData();

					for(int i=0;i<count;i++)
					{
						il_list.Add((*idp)->right);
						++idp;
					}

					return count;
				}

				IDItem *GetItem(int n)const{return GetObject(data_list,n);}							///<取指定序号的数据
				bool 	Get(int,F &,T &)const;														///<取指定序号的数据
				bool	GetKey(int,F &)const;														///<取指定序号的索引
				bool	GetValue(int,T &)const;														///<取指定序号的数据

				bool	SetValueBySerial(int,T &);													///<根据序号设置数据

				void	operator=(const _Map<F,T,IDItem> &);									    ///<操作符重载，复制一个列表

                void    Enum(void (*enum_func)(const F &,T));                                       ///<枚举所有数据项
                void    EnumKey(void (*enum_func)(const F &))const;                                 ///<枚举所有索引
                void    EnumValue(void (*enum_func)(T));                                            ///<枚举所有数值

                void    EnumValue(bool (*enum_func)(T));                                            ///<枚举所有数值
	};//class _Map

	template<typename F,typename T> class Map:public _Map<F,T,Pair<F,T> >
	{
	public:

		Map()=default;
		virtual ~Map()=default;
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
				void	DeleteObject(DataPair *ds)
                {
                    if(!ds)return;
                    DeleteObject(ds->left,ds->right);
                }

				void	DeleteObject(int index)
                {
                    DeleteObject(GetObject(this->data_list,index));
                }

	public:

		_MapObject()=default;
		virtual ~_MapObject()
		{
			if(SuperClass::GetCount()>0)
			{
				//LOG_ERROR(u"这是一个严重的程序设计错误，会产生纯虚函数调用，请在派生类的析构函数中调用Clear函数以清除数据。");
				//LOG_ERROR(OS_TEXT("This is a serious design errors, will produce the pure virtual function call, please call in the destructor of the derived class the <Clear> function to clear the data."));
			}
		}

		/**
		* 断开一个数据
		* @param flag 要断开的数据标识
		* @return 是否断开成功
		*/
		bool UnlinkByKey(const F &flag)
		{
			return UnlinkBySerial(SuperClass::Find(flag));
		}

		/**
		* 断开一个数据
		* @param tp 要断开的数据
		* @return 是否断开成功
		*/
		bool UnlinkByValue(T *tp)
		{
			return UnlinkBySerial(this->FindByValue(tp));
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
		bool DeleteByKey(const F &flag)
		{
			return DeleteBySerial(SuperClass::Find(flag));
		}

		/**
		* 删除一个数据
		* @param tp 要删除的数据
		* @return 是否删除成功
		*/
		bool DeleteByValue(T *tp)
		{
			return DeleteBySerial(this->FindByValue(tp));
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

                DataPair *dp=GetObject(this->data_list,index);

                if(dp)
                    dp->right=data;
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

                DataPair *dp=GetObject(this->data_list,index);

                if(!dp)
                    return(false);

                dp->right=data;
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

		CusMapObject()=default;
		virtual ~CusMapObject()
		{
			_MapObject<F,T,DataPair>::Clear();
		}
	};//class CusMapObject

	template<typename F,typename T> class MapObject:public CusMapObject<F,T,Pair<F,T *> >
	{
	public:

		MapObject()=default;
		virtual ~MapObject()
		{
			CusMapObject<F,T,Pair<F,T *> >::Clear();
		}

		T *operator[](const F &index)const
		{
            auto *obj=GetObject(this->data_list,this->Find(index));

            if(obj)
                return obj->right;
            else
                return nullptr;
		};
	};//class MapObject
}//namespace hgl
#include<hgl/type/Map.cpp>
#endif//HGL_MAP_INCLUDE
