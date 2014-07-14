#ifndef HGL_TYPE_DATA_TEAM_INCLUDE
#define HGL_TYPE_DATA_TEAM_INCLUDE

namespace hgl
{
	/**
	 * 数据分组处理模板
	 */
	template<typename T,typename F> class DataTeam													///数据分组处理模板
	{
	protected:

		T *data_list;

	protected:

		int FlagToTeam(const F &);

	public:

		DataTeam()
		{
			data_list=nullptr;
		}

		virtual ~DataTeam()
		{
			Clear();
		}

		/**
		 * 数据初始化
		 * @param team_count 分组数量
		 */
		void Init(int team_count)
		{
			data_list=new T[team_count];
		}

		void Clear()																				///<数据清空
		{
			delete[] data_list;
		}

		T &operator [](const F &f)
		{
			return data_list[FlagToTeam(f)];
		}
	};//template<typename T> class DataTeam

/*	//测试用例
	class TestData		//数据类
	{
		int get();
	};

	//特例化HASH函数，使用传入的值算出属于那一组
	namespace hgl	//注：在某些编译器如GCC 4.7.2中，出现名字空间无法正确识别的情况下，请将此特例化函数定义在hgl名字空间内
	{
		template<> int DataTeam<TestData,int>::FlagToTeam(const int id)
		{
			return id&0xFF;
		}
	}//namespace hgl

	DataTeam<TestData,int> test_data_list;

	void Init(){test_data_list.Init(0x100);}	//初始化数据分组为0x100组
	void Clear(){test_data_list.Clear();}

	int GetData(int index)
	{
		return test_data_list[index].get();	//根据索引调用对应的类
	}
	*/
}//namespace hgl
#endif//HGL_TYPE_DATA_TEAM_INCLUDE
