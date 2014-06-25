#ifndef HGL_TYPE_ARRAY_2D_INCLUDE
#define HGL_TYPE_ARRAY_2D_INCLUDE

#include<hgl/type/Set.h>

namespace hgl
{
	/**
	* 2D阵列管理
	*/
	template<typename T> class Array2D																///2D阵列管理模板
	{
	protected:

		int row_count,col_count;																	///<元素块行/列数量
		int cell_count;																				///<元素块总量

		T *items;																					///<元素块

	public:

		const int GetRowCount()const{return row_count;}												///<取得元素块行数
		const int GetColCount()const{return col_count;}												///<取得元素块列数
		const int GetCellCount()const{return cell_count;}											///<取得元素块部量

	public:

		Array2D()
		{
			cell_count=row_count=col_count=0;
			items=nullptr;
		}

		/**
		 * 初始化阵列
		 * @param c 列数
		 * @param r 行数
		 * @return 是否成功
		 */
		bool Init(int c,int r)
		{
			col_count=c;
			row_count=r;

			if(row_count<=0||col_count<=0)
			{
				cell_count=0;
				items=nullptr;
				return(false);
			}

			cell_count=row_count*col_count;

			items=new T[cell_count];
			return(true);
		}

		~Array2D()
		{
			SAFE_CLEAR_OBJECT_ARRAY(items,(row_count*col_count));
		}

		bool Get(T &obj,const int c,const int r) const
		{
			if(c<0)return(false);
			if(c>=col_count)return(false);
			if(r<0)return(false);
			if(r>=row_count)return(false);

			obj=items[c+(r*col_count)];
			return(true);
		}

		/**
		* 取得指定元素块的周边块
		* @param smt_set 取出的块存放用合集
		* @param col 元素块的列索引
		* @param row 元素块的行索引
		* @param edge 边缘厚度
		*/
		void GetEdge(Set<T> &item_set,const int col,const int row,const int edge=1)
		{
			if(!items)return;

			int l=col-edge;
			int r=col+edge;
			int t=row-edge;
			int b=row+edge;

			if(l<0)l=0;
			if(r>=col_count)r=col_count-1;
			if(t<0)t=0;
			if(b>=row_count)b=row_count-1;

			if(l>r)return;
			if(t>b)return;

			for(int h=t;h<=b;h++)
				for(int v=l;v<=r;v++)
					item_set.Add(items[v+h*col_count]);
		}
	};//template<typename T> class MapTileManage
}//namespace hgl
#endif//HGL_TYPE_ARRAY_2D_INCLUDE
