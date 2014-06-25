#include<hgl/dfs/dfsClientFile.h>
#include<hgl/thread/Thread.h>
#include<hgl/Str.h>
#include<iostream>

using namespace hgl;
using namespace hgl::dfs;

int TEST_THREAD_MAX=32;
int TEST_FILE_MAX=16;
int TEST_FILE_SIZE=256;

dfsClientFile *dfs_file;

class TestThread:public Thread
{
	char *buf;

public:

	TestThread()
	{
		buf=new char[TEST_FILE_SIZE+4];
	}

	~TestThread()
	{
		delete[] buf;
	}

	bool Execute()
	{
		if(rand()%99)
		{
			InputStream *is=dfs_file->LoadFile(rand()%TEST_FILE_MAX);

			delete is;
		}
		else
		{
			OutputStream *os=dfs_file->SaveFile(rand()%TEST_FILE_MAX);

			int length=4+(rand()%TEST_FILE_SIZE);

			for(int i=0;i<length;i++)
				buf[i]=rand()%256;

			os->Write(buf,length);
			os->Close();
			delete os;
		}

		return(true);
	}
};//class TestThread

int main(int args,char **argv)
{
	if(args<4)
	{
		std::cout<<"dfsClientTest [dfsFileServer IP][Thread count][file count]"<<std::endl;
		return(0);
	}

	stoi(argv[2],TEST_THREAD_MAX);
	stoi(argv[3],TEST_FILE_MAX);
	stoi(argv[4],TEST_FILE_SIZE);

	std::cout<<"dfsClientTest Thread:"<<TEST_THREAD_MAX<<",File:"<<TEST_FILE_MAX<<",Size:"<<TEST_FILE_SIZE<<std::endl;

	dfs_file=new dfsClientFile("Test",TEST_THREAD_MAX);

	dfs_file->Init(argv[1],4000,"Test");

	srandom(time(nullptr));

	TestThread **tt;

	tt=new TestThread *[TEST_THREAD_MAX];

	for(int i=0;i<TEST_THREAD_MAX;i++)
	{
		tt[i]=new TestThread;
		tt[i]->Start();
	}

	for(int i=0;i<TEST_THREAD_MAX;i++)
	{
		tt[i]->Wait();
		delete tt[i];
	}

	delete tt;
	return(0);
}
