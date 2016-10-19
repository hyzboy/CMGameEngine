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

	public:

		BitArray bit;

	public:

		Switch(uint s,uint e):bit(e-s+1)
		{
			start=s;
			end=e;
		}

		Switch(const Switch *sw):Switch(sw->start,sw->end)
		{
			bit		=sw->bit;
		}

		Switch(const Switch &sw):Switch(&sw)
		{
		}

		virtual ~Switch()=default;

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
			bit.WritePos(el-start,true);

			return *this;
		}

		Switch& operator >>(const E el)
		{
			bit.WritePos(el-start,false);

			return *this;
		}

		void Clear(bool all_select=false)
		{
        	bit.Fill(all_select);
		}

		bool Check(const E el)const
		{
			return bit.ReadPos(el-start);
		}

		bool operator == (const Switch &rhs) const
		{
            return bit==rhs.bit;
		}

    	bool operator !=(const Switch& rhs) const {return !operator==(rhs);}
	};//class Switch
}//namespace hgl
#endif//HGL_TYPE_SWITCH_INCLUDE
