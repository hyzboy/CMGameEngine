#ifndef HGL_TYPE_SWITCH_INCLUDE
#define HGL_TYPE_SWITCH_INCLUDE

#include<hgl/type/BitArray.h>
namespace hgl
{
	/**
	* 选择设置用模板
	*/
	template<typename E> class Switch
	{
	protected:

		uint start,end;
		BitArray bit;

	public:

		Switch(uint s,uint e):bit(e-s+1)
		{
			start=s;
			end=e;
		}

		Switch(Switch *sw):bit(sw->end-sw->start+1)
		{
			start	=sw->start;
			end		=sw->end;

			bit		=sw->bit;
		}

		virtual ~Switch()
		{
		}

		/**
		* 取得数据访问指针
		* @param buf 数据指针
		* @return 字节长度
		*/
		int GetData(void **buf)
		{
			int byte_size=(bit.Count+7>>3);

			*buf=bit.Data;

			return byte_size;
		}

		Switch &operator=(const Switch &rhs)
		{
			if(this!=&rhs)
				bit=rhs.bit;

			return *this;
		}

		Switch &operator+=(const Switch &rhs)
		{
			if(this!=&rhs)
				bit+=rhs.bit;

			return *this;
		}

		Switch &operator-=(const Switch &rhs)
		{
			if(this!=&rhs)
				bit-=rhs.bit;

			return *this;
		}

		Switch &operator*=(const Switch &rhs)
		{
			if(this!=&rhs)
				bit*=rhs.bit;

			return *this;
		}

		Switch operator+(const Switch &rhs)const
		{
			Switch<E> s(this);

			s+=rhs;

			return s;
		}

		Switch operator-(const Switch &rhs)const
		{
			Switch<E> s(this);

			s-=rhs;

			return s;
		}

		Switch operator*(const Switch &rhs)const
		{
			Switch<E> s(this);

			s+=rhs;

			return s;
		}

		Switch& operator <<(const E el)
		{
			bit.WritePos(el-Start,true);

			return *this;
		}

		Switch& operator >>(const E el)
		{
			bit.WritePos(el-Start,false);

			return *this;
		}

		void Clear(bool all_select=false)
		{
        	bit.Fill(all_select);
		}

		bool Check(const E el)const
		{
			return bit.ReadPos(el-Start);
		}

		bool operator == (const Switch &rhs) const
		{
            return bit==rhs.bit;
		}

    	bool operator !=(const Switch& rhs) const {return !operator==(rhs);}

		bool SaveToStream	(io::DataOutputStream *str){return bit.SaveToStream(str);}				///<保存当前位阵列数据到流
		bool LoadFromStream	(io::DataInputStream *str){return bit.LoadFromStream(str);}				///<从流中读取位阵列数据
	};//class Switch
}//namespace hgl
#endif//HGL_TYPE_SWITCH_INCLUDE
