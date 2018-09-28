#include<hgl/type/MemBlock.h>
#include<stdio.h>

using namespace hgl;

int main()
{
	SharedPtr<MemBlock<char> > mb=new MemBlock<char>(1024);

	int length=mb->length();

	printf("mem block length=%d\n",length);

	SaveMemBlockToFile(OS_TEXT("test.mb"),*mb);
	return 0;
}
