#ifndef HGL_TYPE_BATCH_SWITCH_INCLUDE
#define HGL_TYPE_BATCH_SWITCH_INCLUDE

#include<hgl/type/Map.h>
namespace hgl
{
	/**
	* 批量分类处理模板
	*/
	template<typename W,typename LT,typename T> class BatchSwitchProc
	{
		Map<W *,LT *> work_set;

	protected:

		virtual void Proc(W *w,LT *sup)=0;									///<处理数据虚拟函数函数

	public:

		void Add(W *wt,const T &usp)										///<增加一个要分类的数据
		{
			LT *sus;

			if(!work_set.Get(wt,sus))
			{
				sus=new LT;
				work_set.Add(wt,sus);
			}

			sus->Add(usp);
		}

		void Clear()														///<清空数据
		{
			work_set.ClearData();
		}

		void Finish()														///<结束添加并处理
		{
			const int count=work_set.GetCount();
			Pair<W *,LT *> **pws=work_set.GetDataList();

			for(int i=0;i<count;i++)
			{
				if((*pws)->second->GetCount()>0)
				{
					Proc((*pws)->first,(*pws)->second);

					(*pws)->second->ClearData();
				}

				++pws;
			}
		}
	};//class BatchSwitchProc
}//namespace hgl
#endif//HGL_TYPE_BATCH_SWITCH_INCLUDE
