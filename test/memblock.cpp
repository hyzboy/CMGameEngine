#include<hgl/MemBlock.h>
#include<stdio.h>

using namespace hgl;

int main()
{
	SharedPtr<MemBlock<char> > mb=new MemBlock<char>(1024);

	int length=mb->Length;

	printf("mem block length=%d\n",length);

	SaveMemBlockToFile("test.mb",mb)
	return 0;
}
