#ifndef HGL_TYPE_BASE_STRING_INCLUDE
#define HGL_TYPE_BASE_STRING_INCLUDE

#include<hgl/type/StringInstance.h>
#include<hgl/CompOperator.h>
#include<hgl/Str.h>
#include<hgl/algorithm/VectorMath.h>

using namespace hgl::algorithm;

namespace hgl
{
    /**
    * 字符串基类
    */
    template<typename T> class BaseString															///字符串基类
    {
    protected:

        typedef BaseString<T>			SelfClass;
        typedef StringInstance<T>		InstClass;
        typedef SharedPtr<InstClass>	SharedClass;

        SharedClass data;																			///<字符串数据实例

    public:

        BaseString()=default;

        BaseString(const T *str)
        {
            Set(str);
        }

        /**
        * 根据一个C指针风格字符串设置当前字符串内容
        * @param str 字符串内容，在len<0的情况下，需以0为结尾
        * @param len 字符串长度，如果str以0为结尾，可以为负值，将启用自动计算长度
        * @param one_instance 是否仅有这一份实例，如果是将不会产生复岓是而是直接使用此值，最终delete[]释放
        */
        BaseString(const T *str,int len,bool one_instance=false)
        {
            Set(str,len,one_instance);
        }

        BaseString(const InstClass &si)
        {
            if((&si)==nullptr)return;

            data=si;
        }

        BaseString(const SelfClass &bs)
        {
            if((&bs)==nullptr)return;

            Set(bs);
        }

        BaseString(const T ch)
        {
            Set(&ch,1,false);
        }

#define BASE_STRING_NUMBER_CONSTRUCT(type,func)	BaseString(const type num)	\
        {	\
            Set(func(new T[8*sizeof(type)],8*sizeof(type),num),-1,true);	\
        }

        BASE_STRING_NUMBER_CONSTRUCT(int,	itos);
        BASE_STRING_NUMBER_CONSTRUCT(uint,	utos);
        BASE_STRING_NUMBER_CONSTRUCT(int64,	itos);
        BASE_STRING_NUMBER_CONSTRUCT(uint64,utos);

        BASE_STRING_NUMBER_CONSTRUCT(float,	ftos);
        BASE_STRING_NUMBER_CONSTRUCT(double,ftos);

#undef BASE_STRING_NUMBER_CONSTRUCT

        BaseString(const int *value,int N)
        {
            const int size=N*sizeof(int)*8;
            int len;

            T tmp_str[size];

            itos(tmp_str,size,*value);
            ++value;

            for(int i=1;i<N;i++)
            {
                strcat(tmp_str,(const T)(','));
                len=strlen(tmp_str);
                itos(tmp_str+len,size-len,*value);
                ++value;
            }

            Set(tmp_str);
        }

        BaseString(const float *value,int N)
        {
            const int size=N*sizeof(float)*16;
            int len;

            T tmp_str[size];

            ftos(tmp_str,size,*value);
            ++value;

            for(int i=1;i<N;i++)
            {
                len=strlen(tmp_str);
                tmp_str[len++]=',';
                ftos(tmp_str+len,size-len,*value);
                ++value;
            }

            Set(tmp_str);
        }

        BaseString(const Vector2f &v,int n=2):BaseString((const float *)&v,n){}
        BaseString(const Vector3f &v,int n=3):BaseString((const float *)&v,n){}
        BaseString(const Vector4f &v,int n=4):BaseString((const float *)&v,n){}

        virtual ~BaseString()=default;

        const T GetEndChar()const																	///<取得当前字符串最后一个字符
        {
// 			if(!this)return(0);
            return(data.valid()?data->GetEndChar():0);
        }

        const int Length()const																		///<当前字符串长度
        {
// 			if(!this)return(0);
            return(data.valid()?data->GetLength():0);
        }

        const bool IsEmpty()const																	///<当前字符串是否空的
        {
// 			if(!this)return(true);
            return(data.valid()?data->GetLength()<=0:true);
        }

        /**
        * 取得一个C风格的字符串指针,失败返回NULL
        */
        T *c_str()const
        {
// 			if(!this)return(nullptr);
            return(data.valid()?data->c_str():nullptr);
        }

        /**
        * 找到指定字符，并返回一个C风格的字符串指针
        */
        T *strchr(T ch)const
        {
// 			if(!this)return(nullptr);
            return(data.valid()?data->c_str()+FindChar(ch):nullptr);
        }

        /**
        * 从末尾查找字符，并返回一个C风格的字符串指针
        */
        T *strrchr(T ch)const
        {
// 			if(!this)return(nullptr);
            return(data.valid()?data->c_str()+FindRightChar(ch):nullptr);
        }

        /**
        * 根据一个C指针风格字符串设置当前字符串内容
        * @param str 字符串内容，在len<0的情况下，需以0为结尾
        * @param len 字符串长度，如果str以0为结尾，可以为负值，将启用自动计算长度
        * @param one_instance 是否仅有这一份实例，如果是将不会产生复岓是而是直接使用此值，最终delete[]释放
        */
        void Set(const T *str,int len=-1,bool one_instance=false)
        {
            if(!str||!*str||!len)		//len=-1为自检测,为0不处理
            {
                Clear();
                return;
            }

            data=new InstClass(str,len,one_instance);
        }

        void Strcpy(const T *str,int len=-1,bool one=false)
        {
            Set(str,len,one);
        }

        /**
        * 设置当前字符串的内容
        * @param bs 内容来源字符串
        */
        void Set(const SelfClass &bs)
        {
            if((&bs)==nullptr)
            {
                Clear();
                return;
            }

            data=bs.data;
        }

        /**
        * 设置当前字符中的内容
        */
        void Set(const InstClass &si)
        {
            if((&si)==nullptr)
            {
                Clear();
                return;
            }

            data=si;
        }

        /**
        * 设置当前字符串中的内容
        */
        void Set(const SharedClass &spsi)
        {
            if((&spsi)==nullptr)
            {
                Clear();
                return;
            }

            data=spsi;
        }

        /**
        * 设置字符串的内容
        */
        bool Set(const SelfClass &bs,int start,int count)
        {
            if(count<=0)return(false);

            if((&bs)==nullptr)
                return(false);

            data=bs.data->CreateCopy(start,count);
            return(true);
        }

        /**
         * 复制一个字符串
         */
        bool Copy(const SelfClass &bs)
        {
            if(bs.IsEmpty())
                return(false);

            data=bs.data->CreateCopy();
            return(true);
        }

        /**
        * 断开与其它BaseString共用的情况，创建一个独有的实例
        */
        bool Unlink()
        {
            if(!data.valid())
                return(false);

            if(data.only())		//自己独有
                return(true);

            data=data->CreateCopy();
            return(true);
        }

        /**
         * 放弃当前数据，将当前字符串清空
         */
        T *Discard()
        {
            if(!data.valid())
                return(nullptr);

            if(data.only())
                return data->Discard();

            return(nullptr);
        }

        /**
        * 取指定索引处的字符
        * @param n 索引偏移
        * @param ch 字符存放变量
        * @return 是否获取成功
        */
        bool GetChar(int n,T &ch)const
        {
            if(n<0)return(false);

            if(!data.valid())
                return(false);

            return data->GetChar(n,ch);
        }

        /**
        * 设定指定索引处的字符
        * @param n 索引偏移
        * @param ch 字符
        * @return 是否设置成功
        */
        bool SetChar(int n,const T ch)
        {
            if(n<0)return(false);

            if(!Unlink())
                return(false);

            return data->SetChar(n,ch);
        }

        /**
        * 在指定偏移处插入指定长度的字符串
        * @param pos 要插入的位置
        * @param str 要插入的字符串
        * @param len 要插入的字符个数,如果为-1则自动检测字符串长度
        */
        bool Insert(int pos,const T *str,int len=-1)
        {
            if(len==0)return(false);

            if(data.valid()&&Unlink())
            {
                if(len<0)
                    len=strlen(str);

                return data->Insert(pos,str,len);
            }
            else
            {
                Set(str,len);
                return(true);
            }
        }

        /**
        * 在指定偏移处插入字符串
        * @param pos 要插入的位置
        * @param str 要插入的字符串
        */
        bool Insert(int pos,const SelfClass &str)
        {
            if((&str)==nullptr)
                return(false);

            return Insert(pos,str.c_str(),str.Length());
        }

        bool Strcat(const T *str,int len)
        {
            if(!str||!*str||len==0)return(false);

            return Insert(Length(),str,len);
        }

        /**
        * 追加一个字符串到当前字符串后面
        */
        bool Strcat(const SelfClass &bs)
        {
            if((&bs)==nullptr)
                return(false);

            return Insert(Length(),bs);
        }

        /**
        * 从指定位置删除指定个字符
        * @param pos 要删除的起始位置
        * @param num 要删除的字符个数
        * @return 是否成功
        */
        bool Delete(int pos,int num)
        {
            if(pos<0||num<=0)return(false);

            if(data.valid()&&Unlink())
                return data->Delete(pos,num);

            return(false);
        }

        /**
        * 清除类中的字符串数据
        */
        void Clear()
        {
            data.unref();
        }

        /**
        * 和一个字符串进行比较
        * @param bs 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const SelfClass &bs)const
        {
            if(!data.valid())
                return(bs.Length());

            if(bs.Length()<=0)
                return Length();

            return data->Comp(bs.data.get());
        }

        /**
        * 和一个字符串进行比较
        * @param str 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const T *str)const
        {
            if(!data.valid())
            {
                if(!str)
                    return 0;
                else
                    return *str;
            }

            return data->Comp(str);
        }

        /**
        * 和一个字符串进行比较
        * @param pos 起始位置
        * @param bs 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const int pos,const SelfClass &bs)const
        {
            if(!data.valid())
                return(bs.Length());

            if(bs.Length()<=0)
                return Length();

            return data->Comp(pos,bs.data.get());
        }

        /**
        * 和一个字符串进行比较
        * @param str 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const int pos,const T *str)const
        {
            if(!data.valid())
            {
                if(!str)
                    return 0;
                else
                    return *str;
            }

            return data->Comp(pos,str);
        }

        /**
        * 和那一个字符串进行比较,英文不区分大小写
        * @param bs 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int CaseComp(const SelfClass &bs)const
        {
            if(!data.valid())
                return(bs.Length());

            if(bs.Length()<=0)
                return 1;

            return data->CaseComp(bs.data->c_str());
        }

        /**
        * 和那一个字符串进行比较,英文不区分大小写
        * @param str 比较字符串
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int CaseComp(const T *str)const
        {
            if(!data.valid())
            {
                if(!str)
                    return 0;

                return *str;
            }

            return data->CaseComp(str);
        }

        /**
        * 和那一个字符串比较指字长度的字符
        * @param bs 比较字符串
        * @param num 比较字数
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const SelfClass &bs,const int num)const
        {
            if(!data.valid())
            {
                if(num<=0)return 0;

                return bs.Length();
            }

            if(bs.Length()<=0)
                return 1;

            return data->Comp(bs.data->c_str(),num);
        }

        /**
        * 和那一个字符串比较指字长度的字符
        * @param str 比较字符串
        * @param num 比较字数
        * @return <0 自身小
        * @return 0 等同
        * @return >0 自身大
        */
        int Comp(const T *str,const int num)const
        {
            if(!data.valid())
            {
                if(!str||num<=0)
                    return 0;

                return *str;
            }

            return data->Comp(str,num);
        }

        /**
         * 和那一个字符串比较指字长度的字符,英文不区分大小写
         * @param bs 比较字符串
         * @param num 比较字数
         * @return <0 自身小
         * @return 0 等同
         * @return >0 自身大
         */
        int CaseComp(const SelfClass &bs,const int num)const
        {
            if(!data.valid())
            {
                if(num<=0)return 0;

                return bs.Length();
            }

            if(bs.Length<=0)
                return 1;

            return data->CaseComp(bs.data->c_str(),num);
        }

        /**
         * 和那一个字符串比较指字长度的字符,英文不区分大小写
         * @param str 比较字符串
         * @param num 比较字数
         * @return <0 自身小
         * @return 0 等同
         * @return >0 自身大
         */
        int CaseComp(const T *str,const int num)const
        {
            if(!data.valid())
            {
                if(!str||num<=0)
                    return 0;

                return *str;
            }

            return data->CaseComp(str,num);
        }
    public:

        bool ToBool(bool &result)const																///<将本类中的字符串转换成布尔数值并返回
        {
            return data.valid()?stob(data->c_str(),result):false;
        }

        template<typename I>
        bool ToInt(I &result)const																	///<将本类中的字符串转换成整型数值并返回
        {
            return data.valid()?etof(data->c_str(),result):false;
        }

        template<typename U>
        bool ToUint(U &result)const																	///<将本类中的字符串转换成整型数值并返回
        {
            return data.valid()?etof(data->c_str(),result):false;
        }

        template<typename F>
        bool ToFloat(F &result)const																///<将本类中的字符串转换成浮点数值并返回
        {
            return data.valid()?etof(data->c_str(),result):false;
        }

        /**
        * 将当前字符串全部转为小写
        * @return 转换后的当前字符串
        */
        SelfClass &LowerCase()																		    ///<将本类中的字母全部转为小写
        {
            if(data.valid()&&Unlink())
                tolower(data->c_str());

            return(*this);
        }

        /**
        * 将当前字符串全部转为小写
        * @return 转换后的字符串
        */
        SelfClass ToLowerCase()const																///<将本类中的字母全部转为小写
        {
            if(!data.valid())
                return SelfClass();

            return SelfClass(data->c_str()).LowerCase();
        }

        /**
        * 将当前字符串全部转为大写
        * @return 转换后的当前字符串
        */
        SelfClass &UpperCase()																		///<将本类中的字母全部转为大写
        {
            if(data.valid()&&Unlink())
                toupper(data->c_str());

            return(*this);
        }

        /**
        * 将当前字符串全部转换为大写
        * @return 转换后的字符串
        */
        SelfClass ToUpperCase()const																///<将本类中的字母全部转为大写
        {
            if(!data.valid())
                return SelfClass();

            return SelfClass(data->c_str()).UpperCase();
        }

        /**
        * 填充当前字符串的部分内容为指定字符
        * @param ch 要填充的字符
        * @param start 填充的起始位置
        * @param len 填充的个数
        * @return 是否成功
        */
        bool FillChar(const T ch,int start=0,int len=-1)
        {
            if(!data.valid())
                return(false);

            if(start<0||data->GetLength()<start+len)
                return(false);

            if(Unlink())
            {
                hgl_set(data->c_str()+start,ch,len);
                return(true);
            }

            return(false);
        }

    protected:

        typedef T *(*ConvFunc)(const T *,int &);

        bool StrConv(ConvFunc conv)
        {
            if(!data.valid()||data->GetLength()<=0)
                return(false);

            int new_len=data->GetLength();

            T *new_str=conv(data->c_str(),new_len);

            if(new_len>0)
            {
                Set(new_str,new_len,true);
                return(true);
            }
            else
            {
                Clear();
                return(false);
            }
        }

    public:

        bool TrimLeft(){return StrConv(trimleft);}													///<删除字符串前端的空格、换行等不可视字符串
        bool TrimRight(){return StrConv(trimright);}												///<删除字符串后端的空格、换行等不可视字符串
        bool Trim(){return StrConv(trim);}															///<删除字符串两端的空格、换行等不可视字符串

        bool TrimLeft(int n){return Delete(0,n);}													///<删除字符串前端的指定个字符
        bool TrimRight(int n){return Unlink()?data->TrimRight(n):false;}							    ///<删除字符串后端的指定个字符

        bool ClipLeft(int n){return Unlink()?data->ClipLeft(n):false;}								///<截取字符串前端的指定个字符,等同TrimRight(lengths-n))
        bool ClipRight(int n){return Delete(0,Length()-n);}											///<截取字符串后端的指定个字符,等同TrimLeft(length-n)
        bool Clip(int pos,int num)																	///<从指定位置删除指定个字符
        {
            if(!Unlink())
                return(false);

            return data->Clip(pos,num);
        }

        /**
         * 从字符串中取指定子串为新的内容
         * @param start 起始字符索引
         * @param n 字符数量
         * @return 成否成功
         */
        bool SubString(int start,int n)																///<取字符串指定段的字符
        {
            if(!Unlink())
                return(false);

            return data->SubString(start,n);
        }

        /**
         * 从字符串中取指定子串为新的内容
         * @param sc 新的字符串
         * @param start 起始字符索引
         * @param n 字符数量
         * @return 成否成功
         */
        bool SubString(SelfClass &sc,int start,int n)												///<取字符串指定段的字符
        {
            if(Length()<start+n)
                return(false);

            sc.Set(data->c_str()+start,n);
            return(true);
        }

        bool Resize(int n)
        {
            if(!data.valid())
                return(false);

            if(n==0)
            {
                Clear();
                return(true);
            }

            return data->Resize(n);
        }

        int StatChar(const T ch)const{return data.valid()?StatChar(data->c_str(),ch):-1;}			///<统计字符串中某个字符的个数
        int StatLine()const{return data.valid()?StatLine(data->c_str()):-1;}						///<统计字符串行数

        int FindChar(int pos,const T ch)const														///<返回当前字符串中指定字符开始的索引(从左至右)
        {
            if(!data.valid())
                return(-1);

            const T *result=hgl::strchr(data->c_str()+pos,ch);

            if(result)
                return result-(data->c_str()+pos);

            return(-1);
        }

        int FindChar(const T ch)const{return FindChar(0,ch);}										///<返回当前字符串中指定字符开始的索引(从左至右)
        int FindRightChar(const T ch)const															///<返回当前字符串中指定字符开始的索引(从右至左)
        {
            if(!data.valid())
                return(-1);

            const T *result=hgl::strrchr(data->c_str(),ch);

            if(result)
                return result-(data->c_str());

            return(-1);
        }

        /**
        * 在整个字符串内，查找指定字符串
        * @param str 要查找的字符串
        * @param start 从第几个字符开始查找，默认0
        * @return 指定子串所在的偏移
        * @return -1 出错
        */
        int FindString(const SelfClass &str,int start=0)const										///<返回当前字符串中指定子串开始的索引
        {
            if(!data.valid())
                return(-1);

            if(str.Length()<=0)
                return(-1);

            if(start<0||start>data->GetLength()-str.Length())
                return(-1);

            const T *result=strstr(data->c_str()+start,str.c_str());

            if(result)
                return result-(data->c_str());

            return(-1);
        }

        /**
        * 在整个字符串内，清除指定字符串
        * @param sub 要清除的字符串
        * @return 总计清除的个数
        * @return -1 出错
        */
        int ClearString(const SelfClass &sub)													    ///<清除当前字符串中指定子串
        {
            if(!Unlink())
                return(-1);

            if(sub.Length()<=0)
                return(false);

            T *sp=data->c_str();
            T *tp=sub.c_str();
            T *p;
            int len=data->GetLength();
            int sub_len=sub.Length;
            int count=0;

            while((p=strstr(sp,tp)))
            {
                len-=(p-sp)+sub_len;
                sp=p;
                memcpy(p,p+sub_len,len*sizeof(T));
                count++;
            }

            return(count);
        }

        /**
        * 在指定位置写入字符串
        * @param pos 开始写入的位置
        * @param str 要写入的字符串
        * @return 是否成功
        */
        bool WriteString(int pos,const SelfClass &str)
        {
            if(!Unlink())
                return(false);

            if(str.Length()<=0)
                return(false);

            return data->Write(pos,str);
        }

        /**
        * 替换当前字符串中指定字符到另一个字符
        * @param sch 要替换的字符
        * @param tch 替换后的字符
        * @return 总计替换个数
        * @return <0 出错
        */
        int Replace(const T tch,const T sch)														///<替换字符
        {
            if(!Unlink())
                return(-1);

            return replace(data->c_str(),tch,sch);
        }

    public:	//操作符重载

        operator const InstClass &()
        {
            return data;
        }

        const T &operator [](int index)
        {
            if(data.valid())
                if(index>=0&&index<data->GetLength())
                    return *(data->c_str()+index);

            const static T zero_char=0;

            return zero_char;	//this is error
        }

        operator 		T *()		{return c_str();}
        operator const	T *()const	{return c_str();}

        SelfClass &operator =	(const T *str		 ){Set(str);return(*this);}
        SelfClass &operator	=	(const SelfClass &str){Set(str);return(*this);}
        SelfClass &operator	+=	(const SelfClass &str){Strcat(str);return(*this);}
        SelfClass &operator	<<	(const SelfClass &str){return(operator+=(str));}

        static SelfClass ComboString(const T *str1,int len1,const T *str2,int len2)
        {
            if(!str1||len1<=0)
            {
                if(!str2||len2<=0)
                    return(SelfClass());

                return SelfClass(str2,len2);
            }
            else
            {
                if(!str2||len2<=0)
                    return(SelfClass(str1,len1));
            }

            const int new_len=len1+len2;

            T *ms=new T[new_len+1];

            memcpy(ms,		str1,len1*sizeof(T));
            memcpy(ms+len1,	str2,len2*sizeof(T));

            ms[new_len]=0;

            return(SelfClass(ms,new_len,true));
        }

        SelfClass  operator +	(const SelfClass &str) const
        {
            if(str.Length()<=0)		//如果对方为空
                return(*this);

            if(!data.valid())		//如果我方为空
                return(str);

            return ComboString(data->c_str(),data->GetLength(),str.c_str(),str.Length());
        }

        SelfClass	operator +	(const T ch) const
        {
            if(!data.valid())
                return(SelfClass(ch));

            return ComboString(data->c_str(),data->GetLength(),&ch,1);
        }

        SelfClass	operator +	(const T *str) const
        {
            if(!data.valid())
                return(SelfClass(str));

            return ComboString(data->c_str(),data->GetLength(),str,strlen(str));
        }

#define BASE_STRING_NUMBER_OPERATOR_ADD(type,func)	SelfClass	operator +	(const type &num) const	\
        {	\
            SharedPtr<T> vstr=func(new T[8*sizeof(type)],8*sizeof(type),num);	\
            \
            return operator+(vstr->data);	\
        }

        BASE_STRING_NUMBER_OPERATOR_ADD(int,	itos);
        BASE_STRING_NUMBER_OPERATOR_ADD(uint,	utos);
        BASE_STRING_NUMBER_OPERATOR_ADD(int64,	itos);
        BASE_STRING_NUMBER_OPERATOR_ADD(uint64,	utos);

        BASE_STRING_NUMBER_OPERATOR_ADD(float,	ftos);
        BASE_STRING_NUMBER_OPERATOR_ADD(double,	dtos);

#undef BASE_STRING_NUMBER_OPERATOR_ADD

        CompOperator(const T *,Comp);
        CompOperator(const SelfClass &,Comp);
    };//template<typename T> class BaseString

    //这种重载用于value+str的情况
    //而类中的的重载用于str+value的情况

    template<typename V,typename T> BaseString<T> operator + (const V &value,const BaseString<T> &str)
    {
        if(str.Length()<=0)
            return BaseString<T>(value);

        return BaseString<T>(value)+str;
    }

    typedef BaseString<char>		UTF8String;
    typedef BaseString<char>		AnsiString;
    typedef BaseString<u16char>		UTF16String;
    typedef BaseString<os_char>		OSString;
    typedef BaseString<char32_t>	UTF32String;
    typedef BaseString<wchar_t>		WideString;

    /**
     * 以累加的方式为一个字符串计算出一个hash码
     */
    template<typename T,int HASH_MAX> uint StringFastHash(const BaseString<T> &str)
    {
        const T *p=str.c_str();
        int c=str.Length();

        uint result=0;

        while(c-->0)
            result+=*p++;

        return(result%HASH_MAX);
    }

    /**
     * 将一串原始数据，转换成一个16进制数值组成的字符串
     * @param value 要转换的原始数据
     * @return 转换好的字符串
     * @see HexToString
     */
    template<typename T,typename I> BaseString<T> ToHexString(const I &value)
    {
        T str[(sizeof(I)<<1)+1];

        ToUpperHexStr(str,value);

        return BaseString<T>(str);
    }

    /**
     * 将一个数值转换为一个用16进制表示的字符串<br>
     * 同ToHexString区别在于，HexToString按数值大小处理
     * @param value 要转换的数值
     * @see ToHexString
     */
    template<typename T,typename I> BaseString<T> HexToString(const I &value)
    {
        T str[(sizeof(I)<<1)+1];

        htos(str,sizeof(I)<<1,value);

        return BaseString<T>(str);
    }

    /**
     * 将一个指针转换成一个16进制字符串
     */
    template<typename T> BaseString<T> PointerToHexString(const void *ptr)
    {
        return HexToString<T,HGL_POINTER_UINT>(reinterpret_cast<const HGL_POINTER_UINT>(ptr));
    }

    inline BaseString<os_char> PointerToHexOSString(const void *value)
    {
        return PointerToHexString<os_char>(value);
    }

    inline BaseString<char> PointerToHexUTF8String(const void *value)
    {
        return PointerToHexString<char>(value);
    }

    inline BaseString<u16char> PointerToHexUTF16String(const void *value)
    {
        return PointerToHexString<u16char>(value);
    }

    template<typename T>
    inline void strcpy(T *dst,int max_count,const BaseString<T> &src)
    {
        hgl::strcpy(dst,max_count,src.c_str(),src.Length());
    }

    template<typename T>
    inline void strcat(T *dst,int max_count,const BaseString<T> &src)
    {
        hgl::strcat(dst,max_count,src.c_str(),src.Length());
    }
}//namespace hgl
#endif//HGL_TYPE_BASE_STRING_INCLUDE
