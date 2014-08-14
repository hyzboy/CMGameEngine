#include<hgl/ut/FindPath.h>

namespace hgl
{
	/**
	* 本类构造函数
	* @param w 查找范围宽度
	* @param h 查找范围高度
	*/
	FindPath::FindPath(int w,int h)
	{
		MaxFind=128;

		Left=0;
		Top=0;
		Width=w;
		Height=h;

		CheckPoint=nullptr;

		Steps.Malloc((w+h)*2);
	}

	void FindPath::SetStart(int x,int y)
	{
		StartX=x-Left;
		StartY=y-Top;
	}

	void FindPath::SetEnd(int x,int y)
	{
		EndX=x-Left;
		EndY=y-Top;
	}

	bool FindPath::Find()
	{
		Steps.ClearData();

		if(CheckPoint==nullptr)
		{
			LOG_ERROR(OS_TEXT("FindPath无CheckPoint函数!"));
			return(false);
		}

		if(StartX==EndX&&StartY==EndY)return(false);

		if(StartX<0||StartX>=Width
		 ||StartY<0||StartY>=Height)return(false);

		if(EndX<0||EndX>=Width
		 ||EndY<0||EndY>=Height)return(false);

        return find_path();
	}

	/**
	* 开始查找路线<br>
	* (sx,sy)、(ex,ey)会被减去(l,t)<br>
	* 主要用于超大地图，但只在一部分范围内寻路的处理
	* @param l 左边界
	* @param t 上边界
	* @param sx 起始点X坐标
	* @param sy 起始点Y坐标
	* @param ex 目标点X坐标
	* @param ey 目标点Y坐标
	*/
	bool FindPath::Find(int l,int t,int sx,int sy,int ex,int ey)
	{
		Left=l;
		Top=t;

		StartX=sx-l;
		StartY=sy-t;

		EndX=ex-l;
		EndY=ey-t;

		return Find();
	}

	/**
	* 开始查找路线
	* @param sx 起始点X坐标
	* @param sy 起始点Y坐标
	* @param ex 目标点X坐标
	* @param ey 目标点Y坐标
	*/
	bool FindPath::Find(int sx,int sy,int ex,int ey)
	{
    	return Find(0,0,sx,sy,ex,ey);
	}
}//namespace hgl

namespace hgl
{
	namespace findpath
	{
    	FindPath *CreateAStar4(int w,int h);
		FindPath *CreateAStar8(int w,int h);
		FindPath *CreateLine(int w,int h);

		FindPath *(*CreateFindPath[fpaEnd-1])(int,int)=
		{
			CreateAStar4,
			CreateAStar8,
			CreateLine
		};
	}//namespace findpath

	/**
	* 创建一个寻路模块
	* @param fpa 寻路算法
	* @param width 查找范围宽度
	* @param height 查找范围高度
	* @return 寻路模块
	*/
	FindPath *CreateFindPath(FPA fpa,int width,int height)
	{
		if(fpa<=fpaNone||fpa>=fpaEnd)
		{
			LOG_ERROR(OS_TEXT("未知的寻路器"));
			return(nullptr);
		}

		if(width<=1||height<=1)
		{
			LOG_ERROR(OS_TEXT("寻路范围太小"));
			return(nullptr);
		}

		return findpath::CreateFindPath[fpa-1](width,height);
	}
}//namespace hgl

