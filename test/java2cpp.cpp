#include<java/io/File.h>
#include<iostream>

using namespace std;

int main(int,char **)
{
	java::io::File *f=nullptr;

	java::lang::String *filename=new java::lang::String(JvNewStringLatin1("./java2cpp.cpp"));

	f=new java::io::File(filename);

	if(f->exists())
		cout<<"java2cpp.cpp exists"<<endl;

	if(f->canRead())
		cout<<"can read"<<endl;

	if(f->canWrite())
		cout<<"can write"<<endl;

	return 0;
}