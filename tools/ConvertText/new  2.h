#include<fstream>

void main()
{
    unsigned char q=0xA0;
    unsigned char w=0xA0;

    char str[100*2];
    char *p;

    fstream fp;

    fp.create("gbk.txt");

    for(q=0xA0;q<=0xFF;q++)
    {
        p=str;
        for(w=0xA0;w<=0xFF;w++)
        {
            *p++=q;
            *p++=w;
        }

        *p++=0;

        fp<<str<<endl;
    }

    fp.close();
}