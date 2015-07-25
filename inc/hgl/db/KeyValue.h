#ifndef HGL_KEY_VALUE_INCLUDE
#define HGL_KEY_VALUE_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
namespace hgl
{
	template<typename K,typename V> struct KeyValue
	{
		K key;
		V value;

	public:

		KeyValue(const K &k):key(k){}
		KeyValue(const K &k,const V &v):key(k),value(v){}
		virtual ~KeyValue(){}
	};//class KeyValue

	/**
	 * Key-Value节点
	 */
	template<typename K,typename V,V NULL_VALUE> class KVNode
	{
		K key;
		V value;

	public:

		Map<K,V> value_list;					//数据子节点

		Map<K,KVNode<K,V,NULL_VALUE>> child_list;		//子节点

	public:

		KVNode(const K &k):key(k){}
		KVNode(const K &k,const V &v):key(k),value(v){}
		virtual ~KVNode(){}

		operator KVNode<K,V,NULL_VALUE>()const{return KVNode(key,value);}

		const K &GetKey()const{return key;}
		const V &GetValue()const{return value;}

		void SetKey(const K &k){key=k;}
		void SetValue(const V &v){value=v;}

	public:	//数据成员访问方法

		const V &operator[](const K &k)
		{
			V v;

			if(value_list.Get(k,v))		//直接在数据中查找
				return v;

			//在子节点中查找
			{
				KVNode<K,V,NULL_VALUE> ckv;

				if(child_list.Get(k,ckv))
				{
				}
			}

			return NULL_VALUE;		//返回空值
		}
	};//class KVNode

	template<typename C,C NULL_VALUE>
	using StringKVNode=KVNode<BaseString<C>,BaseString<C>,NULL_VALUE>;

	using UTF8StringKVNode	=StringKVNode<char		,BaseString<char	>(U8_TEXT("")	)>;
	using UTF16StringKVNode	=StringKVNode<u16char	,BaseString<u16char	>(U16_TEXT("")	)>;
	using WideStringKVNode	=StringKVNode<wchar_t	,BaseString<wchar_t	>(L""			)>;

	namespace io
	{
		class InputStream;
	}//namespace io

	bool LoadXML(UTF8StringKVNode *,io::InputStream *);												///<解晰一个XML流到KV节点树上
	bool LoadXML(UTF8StringKVNode *,const OSString &);												///<解晰一个XML文件到KV节点树上
}//namespace hgl
#endif//HGL_KEY_VALUE_INCLUDE
