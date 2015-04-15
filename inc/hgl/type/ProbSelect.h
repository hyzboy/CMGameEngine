#ifndef HGL_PROBSELECT_INCLUDE
#define HGL_PROBSELECT_INCLUDE

#include<hgl/type/List.h>

namespace hgl
{
	/**
	* 机率选择器，用于按一定机率选择结果
	*/
	class ProbSelect                                                                                ///机率选择器
	{
		uint total;

		List<uint> val;

	public:

		uint GetCount(){return val.GetCount();}                                                      ///<数据数量
		uint GetTotal(){return total;}                                                               ///<总量值

	public:

		uint GetValue(uint n)
		{
			uint r;
			return val.Get(n,r)?r:0;
		}

		void SetValue(uint,uint);

		float GetProp(uint n)
		{
			uint r;

			return val.Get(n,r)?float(r)/float(total):0;
		}

	public:

		ProbSelect(const uint *,const uint);
		ProbSelect();
		virtual ~ProbSelect()HGL_DEFAULT_MEMFUNC;

        void Add(const uint);                                                                       ///<添加一个机率数据
		void Add(const uint *,const uint);                                                          ///<添加多个机率数据
        uint AddBatch(const uint,...);                                                              ///<添加多个机率数据，<0为结尾

		uint Select()const;
	};//class ProbSelect
}//namespace hgl
#endif//HGL_PROBSELECT_INCLUDE
