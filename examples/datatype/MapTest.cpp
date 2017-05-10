#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<hgl/type/Set.h>
#include<hgl/type/Map.h>

using namespace hgl;
using namespace std;

void out_id(Map<int,int> &ii)
{
    const int count=ii.GetCount();
    Pair<int,int> **p=ii.GetDataList();

    cout<<"count:"<<count<<" ";

    for(int i=0;i<count;i++)
    {
        cout<<"["<</*i<<":"<<*/(*p)->left<<","<<(*p)->right<<"]";
        ++p;
    }

    cout<<endl;
}

int main(int,char **)
{
    int index;
    int value;
    Set<int> si;
    Map<int,int> ii;

    srand(time(nullptr));

    for(int i=0;i<20;i++)
    {
        while(true)
        {
            index=rand()%20;
            if(si.Find(index)==-1)break;
        }

        si.Add(index);

        value=rand()%255;

        cout<<"add index="<<index<<" value="<<value<<endl;
        ii.Add(index,value);

        out_id(ii);
    }

    cout<<endl;

    for(int i=0;i<20;i++)
    {
        value=rand()%255;

        cout<<"update index="<<i<<" value="<<value<<endl;
        ii.Update(i,value);

        out_id(ii);
    }

    return(0);
}
