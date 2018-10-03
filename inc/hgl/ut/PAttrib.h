#pragma once

#include<hgl/type/BaseString.h>
#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>

namespace hgl
{
    using namespace io;

    template<typename C> class PAttribBase
    {
    public:

        PAttribBase()=default;
        virtual ~PAttribBase()=default;

        virtual const bool ParseFromString(const BaseString<C> &str)=0;
        virtual BaseString<C> MakeToString()const=0;
    };

    template<typename C,typename T> class PAttrib:public PAttribBase<C>
    {
    protected:

        BaseString<C> name;
        T value;
        T default_value;

    public:

        PAttrib(const BaseString<C> &n,const T &v)
        {
            name=n;
            value=v;
            default_value=v;
        }

        virtual ~PAttrib()=default;

        virtual const bool ParseFromString(const BaseString<C> &str)=0;
        virtual BaseString<C> MakeToString()const=0;

        virtual const T &Get(){return value;}
        virtual void Set(const T &v){value=v;}
    };//template<typename C,typename T> class PAttrib

    template<typename C,typename T> class PNumberAttrib:public PAttrib<C,T>
    {
    protected:

        T min_value,max_value;

    public:

        PNumberAttrib(const BaseString<C> &n,const T &dv,const T &min_v,const T &max_v):PAttrib<C,T>(n,dv)
        {
            min_value=min_v;
            max_value=max_v;
        }

        virtual ~PNumberAttrib()=default;

        const bool ParseFromString(const BaseString<C> &str) override
        {
            if(ToNumber(str,this->value))
            {
                if(this->value>=min_value&&this->value<=max_value)
                    return(true);
            }

            this->value=this->default_value;
            return(false);
        }

        BaseString<C> MakeToString() const override
        {
            return BaseString<C>(this->value);
        }
    };//class PNumberAttrib:public PAttrib<C,uint>

    template<typename C> using PIntAttrib   =PNumberAttrib<C,int    >;
    template<typename C> using PUintAttrib  =PNumberAttrib<C,uint   >;
    template<typename C> using PFloatAttrib =PNumberAttrib<C,float  >;
    template<typename C> using PDoubleAttrib=PNumberAttrib<C,double >;

    template<typename C> class PBoolAttrib:public PAttrib<C,bool>
    {
    public:

        using PAttrib<C,bool>::PAttrib;

        const bool ParseFromString(const BaseString<C> &str)
        {
            if(str.ToBool(this->value))
                return(true);

            this->value=this->default_value;
            return(false);
        }

        BaseString<C> MakeToString() const override
        {
            return(this->value?"true":"false");
        }
    };

    template<typename C> class PStringAttrib:public PAttrib<C,BaseString<C>>
    {
    public:

        using PAttrib<C,BaseString<C>>::PAttrib;

        const bool ParseFromString(const BaseString<C> &str) override
        {
            this->value=str;
            return(true);
        }

        BaseString<C> MakeToString() const override
        {
            return this->value;
        }
    };

    template<typename C> class PAttribSet
    {
        using PString=BaseString<C>;
        using PStringList=StringList<PString>;

        Map<BaseString<C>,PAttribBase<C> *> pa_map;

    public:

        template<typename T>
        PNumberAttrib<C,T> *CreateNumberAttrib(const PString &name,const T &dv,const T &min_v,const T &max_v)
        {
            PNumberAttrib<C,T> *obj=new PNumberAttrib<C,T>(name,dv,min_v,max_v);

            pa_map.Add(name,obj);

            return obj;
        }

        PBoolAttrib<C> *CreateBoolAttrib(const PString &name,const bool &dv)
        {
            PBoolAttrib<C> *obj=new PBoolAttrib<C>(name,dv);

            pa_map.Add(name,obj);

            return obj;
        }

        PStringAttrib<C> *CreateStringAttrib(const PString &name,const PString &dv)
        {
            PStringAttrib<C> *obj=new PStringAttrib<C>(name,dv);

            pa_map.Add(name,obj);

            return obj;
        }

    public:

        bool Add(const PString &name,PAttribBase<C> *attr)
        {
            return pa_map.Add(name,attr);
        }

        PAttribBase<C> *Get(const PString &name){return GetObject(pa_map,name);}

        void Delete(const PString &name){pa_map.DeleteByKey(name);}

        void Clear(){pa_map.Clear();}
        void ClearData(){pa_map.ClearData();}

        void Enum(void (*enum_func)(const BaseString<C> &key,PAttribBase<C> *value))
        {
            pa_map.Enum(enum_func);
        };

    public:

        /**
         * 保存到文本文件中
         */
        template<ByteOrderMask BOM>
        bool SaveToTextFile(const OSString &filename,const PString &gap_ch=PString("\t"))        ///<保存列表到文件
        {
            FileOutputStream fos;
            EndianTextOutputStream<BOM> tos(&fos);

            if(!fos.CreateTrunc(filename))return(false);

            tos.WriteBOM();

            const int count=pa_map.GetCount();
            auto **pa_obj=pa_map.GetDataList();
            for(int i=0;i<count;i++)
            {
                tos.WriteString((*pa_obj)->left);
                tos.WriteString(gap_ch);
                tos.WriteString((*pa_obj)->right->MakeToString());
                tos.WriteLineEnd();

                ++pa_obj;
            }

            return(true);
        }

    private:

        bool Add(const PString &str)                                                  ///<向列表中增加一项
        {
            PString name;
            C *value;
            int off;

            if(str.Length()<2)return(false);

            if(((off=str.FindChar(C('\t')))==-1)
             &&((off=str.FindChar(C(' '))) ==-1)
             &&((off=str.FindChar(C('='))) ==-1)
             &&((off=str.FindChar(C(':'))) ==-1))
                return(false);

            name.Strcpy(str,off);
            off++;

            value=str.c_str()+off;

            while(true)
            {
                if(*value == C('\t')
                 ||*value == C('=')
                 ||*value == C(' ')
                 ||*value == C(':'))
                {
                    value++;
                    continue;
                }

                break;
            }

            PAttribBase<C> *attr=Get(name);

            if(attr)
                attr->ParseFromString(value);

            return(true);
        }

    public:

        /**
         * 从文本文件中加载
         */
        virtual bool LoadFromTextFile(const OSString &filename)                                         ///<从文件中加载列表
        {
            PStringList sl;

            if(LoadStringListFromTextFile(sl,filename)<=0)
                return(false);

            int n=sl.GetCount();

            while(n--)
                Add(sl[n]);

            return(true);
        }
    };//template<typename C> class PAttribSet

    using UTF8PAttribSet    =PAttribSet<char>;
    using UTF16PAttribSet   =PAttribSet<u16char>;
    using WidePAttribSet    =PAttribSet<wchar_t>;
    using OSPAttribSet      =PAttribSet<os_char>;
}//namespace hgl
