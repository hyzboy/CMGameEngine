#ifndef HGL_TABLE_CPP
#define HGL_TABLE_CPP

#include<hgl/type/Table.h>
#include<hgl/stream/Stream.h>
namespace hgl
{
    template<typename T>
    void Table<T>::InitPrivate(int w,int h)
    {
        Data=nullptr;

        OnSave=nullptr;
        OnLoad=nullptr;

        Create(w,h);

        hglSetProperty(RowCount,this,Table<T>::GetRowCount,Table<T>::SetRowCount);
        hglSetProperty(ColCount,this,Table<T>::GetColCount,Table<T>::SetColCount);
    }

    template<typename T>
    Table<T>::Table()
    {
        InitPrivate();
    }

    template<typename T>
    Table<T>::Table(int w,int h)
    {
        InitPrivate(w,h);
    }

    template<typename T>
    Table<T>::~Table()
    {
        Clear();
    }

    template<typename T>
    bool Table<T>::Create(int w,int h)
    {
        Clear();

        if(w<=0||h<=0)return(false);

        col_count=w;
        row_count=h;

        Data=hgl_aligned_malloc<T *>(row_count);

        memset(Data,0,row_count*sizeof(T *));

        return(true);
    }

    template<typename T>
    void Table<T>::Clear()
    {
        if(!Data)return;

        int n=row_count;

        while(n--)
            if(Data[n])
                hgl_free(Data[n]);

        hgl_free(Data);
        Data=nullptr;

        row_count=0;
        col_count=0;
    }

    template<typename T>
    void Table<T>::SetRowCount(int h)
    {
        if(h>row_count)
        {
            Data=(T **)hgl_realloc(Data,h*sizeof(T *));

            for(int i=row_count;i<h;i++)
                Data[i]=nullptr;
        }
        else if(h<row_count)
        {
            for(int i=h;i<row_count;i++)
                if(Data[i])
                    hgl_free(Data[i]);

            Data=(T **)hgl_realloc(Data,h*sizeof(T *));
        }

        row_count=h;
    }

    template<typename T>
    void Table<T>::SetColCount(int w)
    {
        if(w>col_count)
        {
            int n=row_count;

            while(n--)
                if(Data[n])
                {
                    Data[n]=(T *)hgl_realloc(Data[n],w*sizeof(T));

                    for(int i=col_count;i<w;i++)
                        memcpy(Data[n]+i,&DefaultData,sizeof(T));
                }
        }
        else if(w<col_count)
        {
            int n=row_count;

            while(n--)
                if(Data[n])
                    Data[n]=(T *)hgl_realloc(Data[n],w*sizeof(T));
        }

        col_count=w;
    }

    template<typename T>
    void Table<T>::ClearDefaultData()
    {
        if(!Data)return;

        int row=row_count;

        while(row--)
            if(Data[row])
            {
                int total=col_count;
                int col=col_count;

                while(col--)
                {
                    if(memcmp(Data[row]+col,&DefaultData,sizeof(T))==0)
                        total--;
                }

                if(total==0)    //全部是缺省数据
                {
                    hgl_free(Data[row]);
                    Data[row]=nullptr;
                }
            }
    }

    template<typename T>
    int Table<T>::Count(const T &comp)
    {
        if(!Data)return(0);

        int row=row_count;
        int total=0;

        while(row--)
            if(Data[row])
            {
                int col=col_count;

                while(col--)
                {
                    if(memcmp(Data[row]+col,&comp,sizeof(T))==0)
                        total++;
                }
            }

        return(total);
    }

    template<typename T>
    int Table<T>::CountNotDefault()
    {
        if(!Data)return(0);

        int row=row_count;
        int total=0;

        while(row--)
            if(Data[row])
            {
                int col=col_count;

                while(col--)
                {
                    if(memcmp(Data[row]+col,&DefaultData,sizeof(T)))
                        total++;
                }
            }

        return(total);
    }

    template<typename T>
    bool Table<T>::Resize(int w,int h)
    {
        if(row_count==w
         &&col_count==h)return(true);

        if(!Data)
            return Create(w,h);

        if(w<=0||h<=0)
            Clear();

        SetRowCount(h);
        SetColCount(w);

        return(true);
    }


    template<typename T>
    T *Table<T>::operator[](int row)
    {
        if(!Data)return(nullptr);
        if(row<0||row>=row_count)return(nullptr);

        return Data[row];
    }

    template<typename T>
    void Table<T>::UpDownFlip()
    {
        if(!Data)return;

        int u=0,d=row_count-1;

        while(u<d)
        {
            T *tmp;

            tmp=Data[u];
            Data[u]=Data[d];
            Data[d]=tmp;

            u++;
            d--;
        }
    }

    template<typename T>
    void Table<T>::LeftRightFlip()
    {
        if(!Data)return;

        for(int i=0;i<row_count;i++)
            if(Data[i])
            {
                int l=0;
                int r=col_count-1;

                while(l<r)
                {
                    T tmp;

                    memcpy(&tmp,Data[i]+l,sizeof(T));
                    memcpy(Data[i]+l,Data[i]+r,sizeof(T));
                    memcpy(Data[i]+r,&tmp,sizeof(T));

                    l++;
                    r--;
                }
            }
    }

    template<typename T>
    void Table<T>::LeftRotate()
    {
        if(!Data)return;

        Table<T> tar(row_count,col_count);

        for(int r=0;r<row_count;r++)
            for(int c=0;c<col_count;c++)
            {
                T tmp;

                Get(c,r,tmp);

                tar.Set(r,col_count-c-1,tmp);
            }

        tar.ClearDefaultData();

        operator=(tar);
    }

    template<typename T>
    void Table<T>::RightRotate()
    {
        if(!Data)return;

        Table<T> tar(row_count,col_count);

        for(int r=0;r<row_count;r++)
            for(int c=0;c<col_count;c++)
            {
                T tmp;

                Get(c,r,tmp);

                tar.Set(row_count-r-1,c,tmp);
            }

        tar.ClearDefaultData();
        operator=(tar);
    }

    template<typename T>
    void Table<T>::operator = (const Table<T> &src)
    {
        if(this==&src)return;       //同一个就不复制了

        Create(src.col_count,src.row_count);

        memcpy(&OutData     ,&(src.OutData),        sizeof(T));
        memcpy(&DefaultData ,&(src.DefaultData),    sizeof(T));

        for(int row=0;row<row_count;row++)
            if(src.Data[row])
            {
                Data[row]=hgl_aligned_malloc<T>(col_count);

                memcpy(Data[row],src.Data[row],col_count*sizeof(T));
            }
    }

    template<typename T>
    bool Table<T>::Get(int c,int r,T &result)
    {
        if(!Data)
        {
            memcpy(&result,&DefaultData,sizeof(T));
            return(false);
        }

        if(c<0||c>=col_count
         ||r<0||r>=row_count)
        {
            memcpy(&result,&OutData,sizeof(T));
            return(true);
        }

        if(Data[r])
            memcpy(&result,Data[r]+c,sizeof(T));
        else
            memcpy(&result,&DefaultData,sizeof(T));

        return(true);
    }

    template<typename T>
    bool Table<T>::Set(int c,int r,const T &value)
    {
        if(!Data)return(false);

        if(c<0||c>=col_count
         ||r<0||r>=row_count)return(false);

        if(!Data[r])
        {
            Data[r]=hgl_aligned_malloc<T>(col_count);

            int n=col_count;
            while(n--)
                memcpy(Data[r]+n,&DefaultData,sizeof(T));
        }

        memcpy(Data[r]+c,&value,sizeof(T));

        return(true);
    }

    template<typename T>
    bool Table<T>::Save(Stream *str)
    {
        if(!str||!Data)return(false);

        str->WriteInt32(col_count);
        str->WriteInt32(row_count);

        for(int row=0;row<row_count;row++)
            if(Data[row])
            {
                str->WriteInt32(col_count);

                if(OnSave==nullptr)
                {
                    str->Write(Data[row],col_count*sizeof(T));
                }
                else
                {
                    for(int col=0;col<col_count;col++)
                        CallEvent(OnSave,(str,Data[row][col]));
                }
            }
            else
            {
                str->WriteInt32(0);
            }

        return(true);
    }

    template<typename T>
    bool Table<T>::Load(Stream *str)
    {
        Clear();

        if(!str)return(false);

        int col,row;

        str->ReadInt32(col);
        str->ReadInt32(row);

        Create(col,row);

        for(int row=0;row<row_count;row++)
        {
            int size;

            str->ReadInt32(size);

            if(size)
            {
                Data[row]=hgl_aligned_malloc<T>(col_count);

                if(OnLoad==nullptr)
                {
                    str->Read(Data[row],col_count*sizeof(T));
                }
                else
                {
                    for(int col=0;col<col_count;col++)
                        CallEvent(OnLoad,(str,Data[row][col]));
                }
            }
        }

        return(true);
    }
}
#endif//HGL_TABLE_CPP
