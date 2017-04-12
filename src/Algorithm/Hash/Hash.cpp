#include<hgl/algorithm/Hash.h>
#include<hgl/io/FileInputStream.h>

namespace hgl
{
    namespace algorithm
    {
        bool GetFileHash(const OSString &filename,HASH_ALGORITHML ha,void *hash_code)
        {
            Hash *hash=CreateHash(ha);

            if(!hash)return(false);

            io::FileInputStream fp;

            if(fp.Open(filename)==false)
                return(false);

            int filesize=fp.GetSize();
            int pos=0,size;
            uint8 data[HGL_SIZE_1MB];

            hash->Init();

            while(pos<filesize)
            {
                size=HGL_SIZE_1MB;
                if(pos+size>filesize)size=filesize-pos;

                fp.Read(data,size);

                hash->Update(data,size);
                pos+=size;
            }

            hash->Final(hash_code);
            fp.Close();
            delete hash;

            return(true);
        }
    }//namespace algorithm
}//namespace hgl
