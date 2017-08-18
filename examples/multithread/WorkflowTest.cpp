#include<hgl/Console.h>
#include<hgl/Workflow.h>
#include<hgl/graph/Bitmap.h>

using namespace hgl;
using namespace hgl::workflow;

/**
 * 工作内容
 */
struct MyWork
{
	uint8 strong;		//强度

	uint8 *start;		//起始地址
	uint width,height;	//大小
	uint line_gap;		//行间隔
};

using MyWorkPost=WorkPost<MyWork>;				//重定义工作投递器

/**
 * 工作线程
 */
class MyWorkThread:public WorkThread<MyWork>	//实现工作线程类
{
	uint8 base_color[3];						//基础色

public:

	using WorkThread<MyWork>::WorkThread;

	void SetBaseColor(uint8 r,uint8 g,uint8 b)
	{
		base_color[0]=r;
		base_color[1]=g;
		base_color[2]=b;
	}

	void ProcWork(MyWork *obj) override			//实现具体工具处理函数
	{
		uint8 write_color[3]={0,0,0};		   //得出要写入的颜色

		if(base_color[0])write_color[0]=obj->strong;
        if(base_color[1])write_color[1]=obj->strong;
        if(base_color[2])write_color[2]=obj->strong;

		uint8 *line_start=obj->start;
		uint8 *p;

		for(uint row=0;row<obj->height;row++)
		{
			p=line_start;
			for(uint col=0;col<obj->width;col++)	//写入颜色
			{
				*p++=write_color[0];
				*p++=write_color[1];
				*p++=write_color[2];
			}
			line_start+=obj->line_gap;			//下一行
		}

		delete obj;								//删除工作对象
	}
};//class MyWorkThread

using MyWorkPtr=MyWork *;
using MyWorkPostPtr=MyWorkPost *;
using MyWorkThreadPtr=MyWorkThread *;

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectCode=OS_TEXT("WorkflowTest");

    app.Init(&sii);

	WorkGroup<MyWorkPost,MyWorkThread> group;	//工作组

	MyWorkPostPtr wp[4]={   new MyWorkPost,
                            new MyWorkPost,
                            new MyWorkPost,
                            new MyWorkPost};				//创建工作投递器

	group.Add(wp,4);							//添加到工作组

	MyWorkThreadPtr wt[4];						//创建四个工作线程

	wt[0]=new MyWorkThread(wp[0]);wt[0]->SetBaseColor(1,0,0);group.Add(wt[0]);
	wt[1]=new MyWorkThread(wp[1]);wt[1]->SetBaseColor(0,1,0);group.Add(wt[1]);
	wt[2]=new MyWorkThread(wp[2]);wt[2]->SetBaseColor(0,0,1);group.Add(wt[2]);
	wt[3]=new MyWorkThread(wp[3]);wt[3]->SetBaseColor(1,1,1);group.Add(wt[3]);

	group.Start();								//启动工作线程

	//让四个工作线程，各画256个16x16大小的块。

	constexpr uint TILE_WIDTH	=16;
	constexpr uint TILE_HEIGHT	=16;
	constexpr uint PIXEL_BYTE	=3;
	constexpr uint COLOR_NUM	=256;
	constexpr uint LINE_BYTES	=512*PIXEL_BYTE;		//图片每行字节数
	constexpr uint TILE_ROWS	=512/TILE_HEIGHT;		//横向块数量
	constexpr uint TILE_COLS	=512/TILE_WIDTH;		//纵向块数量

	constexpr uint bitmap_bytes=TILE_WIDTH*TILE_HEIGHT*COLOR_NUM*4*PIXEL_BYTE;
	uint8 *bitmap=new uint8[bitmap_bytes];			//创建位图,16x16x4x256=512x512，所以下面视图片以512x512
	uint8 strong=255;
	uint row=0,col=0;

    MyWorkPtr mwp[4][16*16];

	for(uint i=0;i<16*16;i++)
	{
		for(int j=0;j<4;j++)
		{
			MyWork *w=new MyWork;

			w->line_gap	=LINE_BYTES;
			w->width	=TILE_WIDTH;
			w->height	=TILE_HEIGHT;
			w->strong	=strong;

			w->start	=bitmap+(row*TILE_COLS*TILE_HEIGHT+col)*TILE_WIDTH*PIXEL_BYTE;

            mwp[j][i]=w;

			++col;
			if(col==TILE_COLS)
			{
				col=0;
				++row;
			}
		}

		--strong;
	}

	for(int i=0;i<4;i++)
    {
        wp[i]->Post(mwp[i],256);				//提交工作到工作投递器
		wp[i]->ToWork();						//发送信号给工作线程
    }

    LOG_INFO("group.Close() begin");

	group.Close();								//关闭并等待工作线程结束

    LOG_INFO("group.Close() end,write bitmap to .TGA");

	graph::SaveToTGA(OS_TEXT("Workflow.TGA"),bitmap,512,512,24);

    delete[] bitmap;

	return(0);
}
