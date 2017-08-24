#include<hgl/Console.h>
#include<hgl/thread/Workflow.h>
#include<hgl/Time.h>
#include<hgl/graph/Bitmap.h>

using namespace hgl;
using namespace hgl::workflow;

#pragma pack(push,1)
using COLOR3B=uint8[3];
#pragma pack(pop)

const uint WORK_TEAM_COUNT=4;				//工作组数量
const COLOR3B WorkColor[WORK_TEAM_COUNT]=
{
	{1,0,0},{0,1,0},{0,0,1},{1,1,1},
	//{1,1,0},{1,0,1},{0,1,1}
};

constexpr uint TILE_WIDTH	=16;
constexpr uint TILE_HEIGHT	=16;
constexpr uint PIXEL_BYTE	=3;
constexpr uint COLOR_NUM	=256;

constexpr uint TILE_ROWS	=COLOR_NUM/TILE_HEIGHT;						//按块算图片高
constexpr uint TILE_COLS	=(COLOR_NUM/TILE_WIDTH)*WORK_TEAM_COUNT;	//按块算图片宽

constexpr uint TILE_COUNT	=TILE_ROWS*TILE_COLS;						//块数量

constexpr uint IMAGE_WIDTH	=TILE_WIDTH*TILE_COLS;
constexpr uint IMAGE_HEIGHT	=TILE_HEIGHT*TILE_ROWS;

constexpr uint LINE_BYTES	=TILE_WIDTH*PIXEL_BYTE*TILE_COLS;			//图片每行字节数

constexpr uint IMAGE_BYTES	=IMAGE_WIDTH*IMAGE_HEIGHT*PIXEL_BYTE;		//图片总字节数

/**
 * 工作内容
 */
struct MyWork
{
	uint8 *start;		//起始地址
	uint width,height;	//大小
	uint line_gap;		//行间隔

	uint8 strong;		//颜色强度
};

/**
 * 工作处理
 */
class MyWorkProc:public MultiWorkProc<MyWork>		//实现工作处理类
{
public:

	using MultiWorkProc<MyWork>::MultiWorkProc;		//使用基类构造函数

	uint8 ComputeColor(uint8 origin_color)
    {
        int off=(rand()%16)-8;          //在原始颜色上随机-8 to +8

        int result=origin_color;

        result+=off;

        if(result+off<0)
            return 0;
        else
        if(result+off>255)
            return 255;
        else
            return result;
    }

	void OnWork(const uint wt_index,MyWork *obj) override			//实现具体工具处理函数
	{
		uint8 *line_start=obj->start;
		uint8 *p;

		for(uint row=0;row<obj->height;row++)
		{
			p=line_start;
			for(uint col=0;col<obj->width;col++)			//写入颜色
			{
				*p++=ComputeColor(WorkColor[wt_index][0]*obj->strong);
				*p++=ComputeColor(WorkColor[wt_index][1]*obj->strong);
				*p++=ComputeColor(WorkColor[wt_index][2]*obj->strong);
			}
			line_start+=obj->line_gap;			//下一行
		}

		delete obj;								//删除工作对象
	}
};//class MyWorkThread

using MyWorkPtr			=MyWork *;
using MyWorkProcPtr		=MyWorkProc *;
using MyWorkThread		=WorkThread<MyWork>;
using MyWorkThreadPtr	=MyWorkThread *;
using MyWorkGroup		=WorkGroup<MyWorkProc,MyWorkThread>;

void FillGrey(uint8 *bitmap,uint size)
{
    uint8 *p=bitmap;

    for(uint i=0;i<size/2;i++)
    {
        *p=255;++p;
        *p=255;++p;
        *p=255;++p;
        *p=0;++p;
        *p=0;++p;
        *p=0;++p;
    }
}

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectCode=OS_TEXT("WorkflowUnsortTest");

    app.Init(&sii);

	MyWorkGroup group;	//工作组

	MyWorkProcPtr	wp=new MyWorkProc();
	MyWorkThreadPtr wt[WORK_TEAM_COUNT];

	for(int i=0;i<WORK_TEAM_COUNT;i++)
		wt[i]=new MyWorkThread(wp);				//创建工作线程并配对

	group.Add(wp);								//添加到工作组
	group.Add(wt,WORK_TEAM_COUNT);				//添加到工作组

	group.Start();								//启动工作组

	uint8 *bitmap=new uint8[IMAGE_BYTES];
	uint row=0,col=0;

    FillGrey(bitmap,IMAGE_BYTES/3);             //填充网格，好根据图像调试是否有格子没有被填充

	for(uint i=0;i<COLOR_NUM;i++)
	{
		for(int j=0;j<WORK_TEAM_COUNT;j++)
		{
			MyWork *w=new MyWork;

			w->line_gap	=LINE_BYTES;
			w->width	=TILE_WIDTH;
			w->height	=TILE_HEIGHT;

			w->strong	=i;

			w->start	=bitmap+(row*TILE_COLS*TILE_HEIGHT+col)*TILE_WIDTH*PIXEL_BYTE;

			wp->Post(w);						//提交工作

			++col;
			if(col==TILE_COLS)
			{
				col=0;
				++row;
			}
		}
	}

    LOG_INFO("group.Close() begin");

	group.Close();								//关闭工作线程结束

    LOG_INFO("group.Close() end,write bitmap to .TGA");

	graph::SaveToTGA(OS_TEXT("WorkflowUnsort.TGA"),bitmap,IMAGE_WIDTH,IMAGE_HEIGHT,24);

    delete[] bitmap;

	return(0);
}
