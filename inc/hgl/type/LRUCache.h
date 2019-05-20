#ifndef HGL_TYPE_LRU_CACHE_INCLUDE
#define HGL_TYPE_LRU_CACHE_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
    template<typename F,typename T> struct LRUCacheItem
    {
        F key;                          //识别标识

        T value;                        //数据

        LRUCacheItem<F,T>   *prev,      //前一数据
                            *next;      //后一数据
    };//template<typename F,typename T> struct LRUCacheItem

    /**
    * 最近使用数据缓冲区管理模板(当缓冲区满时，将最长时间没有使用的清除)<br>
    * 现这个模板使用双头链表，每次添加或使用的数据会被移到链表的最前端。<br>
    * 这样使用率最低的数据会被存在链表的最末端，当缓冲区满时，最末端的数据将会被清除。
    */
    template<typename F,typename T> class LRUCache                                                  ///缓冲区管理模板(以最终使用时间为基准)
    {
    public:

        using LruItem=LRUCacheItem<F,T>;

    protected:

        LruItem *start_item,        //首数据
                *end_item;          //尾数据

        int count,max_count;

    protected:

        virtual bool Create(const F &,T &);                                                         ///<创建数据
        virtual void Clear(const F &,T &);                                                          ///<清除数据

                void MoveToStart(LruItem *);                                                        ///<移动某一个数据到最前面

                void Delete(LruItem *);                                                             ///<清除某一个数据

                void ClearEnd();                                                                    ///<清除最后一个数据

    public:

                const   int     GetCount    ()const{return count;}                                  ///<取得当前有多少数据
                const   int     GetMaxCount ()const{return max_count;}                              ///<取得最大可以有多少数据
        virtual         void    SetMaxCount (int);                                                  ///<设置最大可以有多少数据
                        int     GetFreeCount()const{return max_count-count;}                        ///<取得当前缓冲区剩于量

    public:

        LRUCache(int);
        virtual ~LRUCache();

        virtual LruItem *       Add     (const F &,const T &);                                      ///<增加一个数据
        virtual bool            Find    (const F &,T &,bool=true);                                  ///<取得一个数据(如果没有不会自动创建)
        virtual bool            Get     (const F &,T &,bool=true);                                  ///<取得一个数据(如果没有会自动创建)
                void            Clear   ();                                                         ///<清除所有数据
                LruItem *       GetEnd  (bool mts=true)                                             ///<取最后一项
                {
                    LruItem *obj=end_item;

                    if(mts)
                        MoveToStart(obj);

                    return(obj);
                }

                void            DeleteByKey(const F &);
                void            DeleteByValue(T &);
//              bool            Update      (const F &,T &);                                        ///<更新数据
    };//template<typename F,typename T> class LRUCache
}//namespace hgl
#include<hgl/type/LRUCache.cpp>
#endif//HGL_TYPE_LRU_CACHE_INCLUDE
