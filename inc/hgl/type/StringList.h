#ifndef HGL_STRINGLIST_INCLUDE
#define HGL_STRINGLIST_INCLUDE

#include<hgl/type/List.h>
#include<hgl/File.h>
#include<hgl/type/BaseString.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
namespace hgl
{
	class HAC;

	/**
	* 字符串列表处理类，用于保存字符串列表。
	*/
	template<typename T> class StringList															///字符串列表处理类
	{
        static T NullString;

	protected:

		ObjectList<T> Items;

	public:	//属性

		T **GetDataList()const{return Items.GetData();}												///<取得字符串列表指针数据
		const int GetCount()const{return Items.GetCount();}											///<字符串列表行数虚拟变量

	public:	//操作符重载

		T &operator[](int n)const
		{
            if(n<0||n>=Items.GetCount())
                return NullString;

            T *result=Items[n];

            if(result)
                return(*result);
            else
                return NullString;
        }

		StringList<T> &operator = (const StringList<T> &sl)
		{
			Clear();

			const int n=sl.GetCount();

			for(int i=0;i<n;i++)
				Add(new T(sl[i]));

			return(*this);
		}

	public: //方法

		StringList()=default;															///<本类构造函数
		StringList(const T &list){operator=(&list);}												///<本类构造函数
		StringList(const T *list){operator=(list);}													///<本类构造函数
		virtual ~StringList(){Clear();}																///<本类析构函数

		/**
		* 增加一行字符串到当前列表中
		* @param str 要增加的字符串
		* @return 增加字符串成功后的索引
		*/
		int Add(const T &str){return Items.Add(new T(str));}										///<添加字符串

		/**
		* 增加一行字符串到当前列表中
		* @param str 要增加的字符串
		* @param len 要增加的字符串的长度
		* @return 增加字符串成功后的索引
		*/
		int Add(const T &str,int len){return Items.Add(new T(str,len));}							///<添加字符串

		/**
		* 增加一个字符串列表到当前字符串列表中
		* @param sl 要增加的字符串列表
		*/
		int Add(const StringList<T> &sl)															///<添加字符串
		{
			const int count=sl.GetCount();

			for(int i=0;i<count;i++)
				Add(sl.Items[i]);

			return(count);
		}

		/**
		* 清除所有字符串
		*/
		void Clear(){Items.Clear();}																///<删除列表中的所有字符串

		/**
		* 删除指定的字符串
		* @param index 要删除的字符串索引
		*/
		void Delete(int index){Items.Delete(index);}												///<删除列表中指定的字符串

		/**
		 * 删除指定字符串
		 * @param str 要删除的字符串
		 * @return 成功删除的行数
		 */
		int Delete(const T &str)
		{
			int count=Items.GetCount();
			int result=0;

			while(count--)
			{
				if(Items[count]->Comp(str)==0)
				{
					Items.Delete(count);
					result++;
				}
			}

			return(result);
		}

		/**
		* 查找字符串
		* @param str 要查找的字符串
		* @return 查找到的字符串的索引,未找到返回-1
		*/
		int  Find(const T &str) const																///<查找字符串,未找到返回-1
		{
			const int count=Items.GetCount();

			for(int i=0;i<count;i++)
				if(Items[i]->Comp(str)==0)
					return(i);

			return(-1);
		}

		/**
		* 查找字符串,英文无视大小写
		* @param str 要指找的字符串
		* @return 查找到的字符串的索引,未找到返回-1
		*/
		int  CaseFind(const T &str) const															///<查找字符串,英文无视大小写,未找到返回-1
		{
			const int count=Items.GetCount();

			for(int i=0;i<count;i++)
				if(Items[i]->CaseComp(str)==0)
					return(i);

			return(-1);
		}

		/**
        * 查找字符串，并指定最大比较长度
        * @param str 要查找的字符串
        * @param cn 限定的要查找字符串的最大长度
        * @return 查找到的字符串的索引,未找到返回-1
        */
        int  Find(const T &str,const int cn) const                                                               ///<查找字符串,未找到返回-1
        {
            const int count=Items.GetCount();

            for(int i=0;i<count;i++)
                if(Items[i]->Comp(str,cn)==0)
                    return(i);

            return(-1);
        }

        /**
        * 查找字符串,英文无视大小写，并指定最大比较长度
        * @param str 要指找的字符串
        * @param cn 限定的要查找字符串的最大长度
        * @return 查找到的字符串的索引,未找到返回-1
        */
        int  CaseFind(const T &str,const int cn) const                                                           ///<查找字符串,英文无视大小写,未找到返回-1
        {
            const int count=Items.GetCount();

            for(int i=0;i<count;i++)
                if(Items[i]->CaseComp(str,cn)==0)
                    return(i);

            return(-1);
        }

		/**
		* 在指定位置插入一个字符串
		* @param index 要插入字符串的位置
		* @param str 要插入的字符串
		*/
		void Insert(int index,const T &str)															///<在指定位置插入一个字符串
		{
			if(index<Items.GetCount())
				Items.List<T *>::Insert(index,new T(str));
		}

		/**
		* 交换两个字符串的位置
		* @param index1 第一个字符串的位置
		* @param index2 第二个字符串的位置
		*/
		void Exchange(int index1,int index2)														///<交换两行字符串
		{
			Items.Exchange(index1,index2);
		}

		T &GetString(int n)const{return *(Items[n]);}												///<取得指定行字符串
	};//template<typename T> class StringList

    template<typename T> T StringList<T>::NullString;                                               ///<空字符串实例

	/**
	 * 以不可打印字符为分隔拆解一个字符串到一个字符串列表
	 * @param sl 字符串列表处理类
	 * @param str 字符串
	 * @param size 字符串长度
	 * @return 字符串行数
	 */
	template<typename T> int SplitToStringListBySpace(StringList<BaseString<T> > &sl,const T *str,int size)
	{
        if(!str||size<=0)return(-1);

		int count=0;
		const T *p,*sp;

		sp=p=str;

		while(size>0)
		{
			if(!(*p))
			{
				if(p>sp)
				{
					sl.Add(sp,p-sp);
					++count;
				}

				--size;
				return count;
			}

			if(isspace(*p))
			{
                if(p>sp)
                {
                    sl.Add(sp,p-sp);
                    ++count;
                }

				sp=p+1; ///<跳过分割符
			}

			++p;
			--size;
		}

		if(p>sp)
		{
			sl.Add(sp,p-sp);
			++count;
		}

		return count;
	}//int SplitToStringList

	/**
	 * 以指定字符为分隔拆解一个字符串到一个字符串列表
	 * @param sl 字符串列表处理类
	 * @param str 字符串
	 * @param size 字符串长度
	 * @param split_char 分隔字符
	 * @return 字符串行数
	 */
	template<typename T> int SplitToStringList(StringList<BaseString<T> > &sl,const T *str,int size,const T &split_char)
	{
        if(!str||size<=0)return(-1);

		int count=0;
		const T *p,*sp;

		sp=p=str;

		while(size>0)
		{
			if(!(*p))
			{
				if(p>sp)
				{
					sl.Add(sp,p-sp);
					++count;
				}

				--size;
				return count;
			}

			if(*p==split_char)
			{
                if(p>sp)
                {
                    sl.Add(sp,p-sp);
                    ++count;
                }

				sp=p+1; ///<跳过分割符
			}

			++p;
			--size;
		}

		if(p>sp)
		{
			sl.Add(sp,p-sp);
			++count;
		}

		return count;
	}//int SplitToStringList

	template<typename T> int SplitToStringList(StringList<BaseString<T> > &sl,const BaseString<T> &str,const T &split_char)
	{
		return SplitToStringList<T>(sl,str.c_str(),str.Length(),split_char);
	}

	/**
	 * 以指定字符为分隔拆解一个字符串到一个字符串列表
	 * @param sl 字符串列表处理类
	 * @param str 字符串
	 * @param size 字符串长度
	 * @param split_char 分隔字符
	 * @param maxSize 最多执行次数
	 * @return 字符串行数
	 */
	template<typename T> int SplitToStringList(StringList<BaseString<T> > &sl,const T *str,int size,const T &split_char,int maxSize)
	{
        if(!str||size<=0)return(-1);

		int count=0;
		const T *p,*sp;

		sp=p=str;

		while(size>0)
		{
			if(!(*p))
			{
				if(p>sp)
				{
					sl.Add(sp,p-sp);
					++count;
				}

				--size;
				return count;
			}

			if(*p==split_char)
			{
                if(p>sp)
                {
                    sl.Add(sp,p-sp);
                    ++count;
                }

				sp=p+1; ///<跳过分割符
				if(maxSize >0 && count >=maxSize-1)
				{
					++p;
					--size;
					if(size > 0)
					{
						sl.Add(sp,size);
						++count;
					}

					return count;
				}
			}

			++p;
			--size;
		}

		if(p>sp)
		{
			sl.Add(sp,p-sp);
			++count;
		}

		return count;
	}//int SplitToStringList

	template<typename T> int SplitToStringList(StringList<BaseString<T> > &sl,const BaseString<T> &str,const T &split_char,int maxSize)
	{
		return SplitToStringList<T>(sl,str.c_str(),str.Length(),split_char,maxSize);
	}

	/**
	 * 拆解一个多行字符串到一个字符串列表
	 * @param sl 字符串列表处理类
	 * @param str 字符串
	 * @param size 字符串长度
	 * @return 字符串行数
	 */
	template<typename T> int SplitToStringListByEnter(StringList<BaseString<T> > &sl,const T *str,int size)
	{
        if(!str||size<=0)return(-1);

		int count=0;
		const T *p,*sp;

		sp=p=str;

		while(size>0)
		{
			if(!(*p))
			{
				if(p>sp)
				{
					sl.Add(sp,p-sp);
					++count;
				}

				--size;
				return count;
			}

			if(*p==0x0D)			// \r
			{
				sl.Add(sp,p-sp);
				++count;

				++p;
				--size;

				if(*p==0x0A)		// \r\n,Windows下的断行是(\r\n)0x0D+0x0A，而其它系统是仅有一个
				{
					++p;
					--size;
				}

				sp=p;
			}
			else
			if(*p==0x0A)			// \n
			{
				sl.Add(sp,p-sp);
				++count;

				++p;
				--size;

				sp=p;
			}
			else
			{
				++p;
				--size;
			}
		}

		if(p>sp)
		{
			sl.Add(sp,p-sp);
			++count;
		}

		return count;
	}//int SplitToStringList

	template<typename T> int SplitToStringList(StringList<BaseString<T> > &sl,const BaseString<T> &str)
	{
		return SplitToStringList<T>(sl,str.c_str(),str.Length());
	}

	/**
	 * 拆解一个多行字符串到多个字符串列表
	 * @param sl 字符串列表处理类
	 * @param slc 字符串列表处理类个数
	 * @param str 字符串
	 * @param size 字符串长度
	 * @return 字符串行数
	 */
	template<typename T> int SplitToMultiStringList(StringList<BaseString<T> > **sl,int slc,const T *str,int size)
	{
        if(!str||size<=0)return(-1);
		if(slc<=0)return(-1);

		int index=0;
		int count=0;
		const T *p,*sp;

		sp=p=str;

		while(size>0)
		{
			if(!(*p))
			{
				if(p>sp)
				{
					sl[index]->Add(sp,p-sp);
					if(++index==slc)index=0;
					++count;
				}

				--size;
				return count;
			}

			if(*p==0x0D)			// \r
			{
				sl[index]->Add(sp,p-sp);
				if(++index==slc)index=0;

				++count;

				++p;
				--size;

				if(*p==0x0A)		// \r\n,Windows下的断行是(\r\n)0x0D+0x0A，而其它系统是仅有一个
				{
					++p;
					--size;
				}

				sp=p;
			}
			else
			if(*p==0x0A)			// \n
			{
				sl[index]->Add(sp,p-sp);
				if(++index==slc)index=0;

				++count;

				++p;
				--size;

				sp=p;
			}
			else
			{
				++p;
				--size;
			}
		}

		if(p>sp)
		{
			sl[index]->Add(sp,p-sp);
			++count;
		}

		return count;
	}//int SplitToStringList

	template<typename T> int SplitToMultiStringList(StringList<BaseString<T> > **sl,int slc,const BaseString<T> &str)
	{
		if(!sl||slc<=0)return(false);

		if(slc==1)
			return SplitToStringList<T>(**sl,str.c_str(),str.Length());
		else
			return SplitToMultiStringList<T>(sl,slc,str.c_str(),str.Length());
	}

	typedef StringList<UTF8String	> UTF8StringList;
	typedef StringList<UTF16String	> UTF16StringList;
//	typedef StringList<UTF32String	> UTF32StringList;
	typedef StringList<AnsiString	> AnsiStringList;
	typedef StringList<WideString	> WideStringList;
	typedef StringList<OSString		> OSStringList;

    template<typename T,ByteOrderMask bom> struct ReadStringFromDIS
    {
        bool ReadString(io::DataInputStream *dis,T &str);
    };

    template<typename T> struct ReadStringFromDIS<T,bomUTF8>
    {
        bool ReadString(io::DataInputStream *dis,T &str)
        {
            return dis->ReadUTF8String(str);
        }
    };

    template<typename T> struct ReadStringFromDIS<T,bomUTF16LE>
    {
        bool ReadString(io::DataInputStream *dis,T &str)
        {
            return dis->ReadUTF16LEString(str);
        }
    };

    template<typename T> struct ReadStringFromDIS<T,bomUTF16BE>
    {
        bool ReadString(io::DataInputStream *dis,T &str)
        {
            return dis->ReadUTF16BEString(str);
        }
    };

	/**
	 * 从DataInputStream流中读取一个字符串列表
	 * @param sl 字符串列表处理类
	 * @param dis 数据输入流
	 * @return 字符串行数
	 */
	template<typename T,ByteOrderMask bom> int LoadStringList(StringList<BaseString<T> > &sl,io::DataInputStream *dis)
	{
		if(!dis)return(-1);

		int count;
		int result=0;

		if(!dis->ReadInt32(count))
			return(-2);

        ReadStringFromDIS<BaseString<T>,bom> rsfd;

		BaseString<T> str;

		for(int i=0;i<count;i++)
		{
			if(!rsfd.ReadString(dis,str))
				break;

			sl.Add(str);
			result++;
		}

		return(result);
	}//int LoadStringList

	inline int LoadUTF8StringList      (UTF8StringList &   sl,io::DataInputStream *dis){return LoadStringList<char,    bomUTF8     >(sl,dis);}
	inline int LoadUTF16LEStringList   (UTF16StringList &  sl,io::DataInputStream *dis){return LoadStringList<u16char, bomUTF16LE  >(sl,dis);}
	inline int LoadUTF16BEStringList   (UTF16StringList &  sl,io::DataInputStream *dis){return LoadStringList<u16char, bomUTF16BE  >(sl,dis);}

    int LoadStringListFromText(UTF8StringList   &sl,void *data,const int size,const CharSet &default_charset=UTF8CharSet);              ///<从文件加载一个文本块到UTF8StringList
    int LoadStringListFromText(UTF16StringList  &sl,void *data,const int size,const CharSet &default_charset=UTF16LECharSet);           ///<从文件加载一个文本块到UTF16StringList

    int LoadStringListFromTextFile(UTF8StringList   &sl,const OSString &filename,const CharSet &default_charset=UTF8CharSet);              ///<从文件加载一个文本文件到UTF8StringList
    int LoadStringListFromTextFile(UTF16StringList  &sl,const OSString &filename,const CharSet &default_charset=UTF16LECharSet);           ///<从文件加载一个文本文件到UTF16StringList

	template<typename T,ByteOrderMask bom> struct WriteStringToDOS
	{
		bool WriteString(io::DataOutputStream *dos,const T &str);
	};

	template<typename T> struct WriteStringToDOS<T,bomUTF8>
	{
		bool WriteString(io::DataOutputStream *dos,const T &str)
		{
			return dos->WriteUTF8String(str);
		}
	};

	template<typename T> struct WriteStringToDOS<T,bomUTF16LE>
	{
		bool WriteString(io::DataOutputStream *dos,const T &str)
		{
			return dos->WriteUTF16LEString(str);
		}
	};

	template<typename T> struct WriteStringToDOS<T,bomUTF16BE>
	{
		bool WriteString(io::DataOutputStream *dos,const T &str)
		{
			return dos->WriteUTF16BEString(str);
		}
	};

	template<typename T,ByteOrderMask bom> int WriteStringList(io::DataOutputStream *dos,const StringList<T> &sl)
	{
		WriteStringToDOS<T,bom> wtd;

		const int32 count=sl.Count;
		int result=0;

		if(!dos->WriteInt32(count))
			return(-2);

		for(int32 i=0;i<count;i++)
		{
			if(!wtd.WriteString(dos,sl[i]))
				return(-3);

			result++;
		}

		return(result);
	};

 	template<typename T> int SaveUTF8StringList(io::DataOutputStream *dos,const StringList<T> &sl)
 	{
 		return WriteStringList<T,bomUTF8>(dos,sl);
 	}

 	template<typename T> int SaveUTF16LEStringList(io::DataOutputStream *dos,const StringList<T> &sl)
 	{
		return WriteStringList<T,bomUTF16LE>(dos,sl);
 	}

 	template<typename T> int SaveUTF16BEStringList(io::DataOutputStream *dos,const StringList<T> &sl)
 	{
 		return WriteStringList<T,bomUTF16BE>(dos,sl);
 	}
}//namespace hgl
#endif//HGL_STRINGLIST_INCLUDE
