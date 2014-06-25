#include<iostream>
#include<unistd.h>

using namespace std;
int main(int argc,char **argv)
{
	char cur_path[1024];

	getcwd(cur_path,1024);

	cout<<"cwd:"<<cur_path<<endl;
	cout<<"argc:"<<argc<<endl;

	for(int i=0;i<argc;i++)
		cout<<"argv["<<i<<"]:"<<argv[i]<<endl;

	return(0);
}
