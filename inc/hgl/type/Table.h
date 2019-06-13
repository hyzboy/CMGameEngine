#ifndef HGL_TABLE_INCLUDE
#define HGL_TABLE_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    class Stream;

    /**
    * 2维表模板
    */
    template<typename T> class Table                                                                ///2维表模板
    {
    protected:

        int row_count;
        int col_count;

        int GetRowCount(){return row_count;}
        int GetColCount(){return col_count;}

        void SetRowCount(int);
        void SetColCount(int);

        void InitPrivate(int=0,int=0);

        void ClearDefaultData();

    public:

        T **Data;

        T OutData;                              //圈外数据(用于返回超出表范围时的数据)
        T DefaultData;                          //空数据(用于填充未赋值的数据)

        Property<int> RowCount;                 //行数
        Property<int> ColCount;                 //列数

    public:

        Table();
        Table(int,int);
        virtual ~Table();

        virtual bool Create(int,int);                                                               ///<创建表
        virtual void Clear();                                                                       ///<清除表

        virtual bool Resize(int,int);                                                               ///<调整表的大小

//      virtual void MoveRow(int,int,int=1){};
//      virtual void MoveCol(int,int,int=1){};

//      virtual void DeleteRow(int,int=1){};
//      virtual void DeleteCol(int,int=1){};

        virtual bool Get(int,int,T &);                                                              ///<取得一个数据
        virtual bool Set(int,int,const T &);                                                        ///<设置一个数据

        virtual int Count(const T &);                                                               ///<统计某一种数据的数量
        virtual int CountNotDefault();                                                              ///<统计非缺省数据

        virtual void UpDownFlip();                                                                  ///<上下翻转
        virtual void LeftRightFlip();                                                               ///<左右翻转

        virtual void LeftRotate();                                                                  ///<左转90度
        virtual void RightRotate();                                                                 ///<右转90度

    public:

        virtual void operator = (const Table<T> &);

        virtual T *operator[](int);

        DefEvent(int,OnSave,(Stream *,const T &));                                                  ///<保存单个数据事件
        DefEvent(int,OnLoad,(Stream *,T &));                                                        ///<读取单个数据事件

        virtual bool Save(Stream *);                                                                ///<保存数据到流
        virtual bool Load(Stream *);                                                                ///<从流中加载数据
    };//template<typename T> class Table
}//namespace hgl
#include<hgl/type/Table.cpp>
#endif//HGL_TABLE_INCLUDE
