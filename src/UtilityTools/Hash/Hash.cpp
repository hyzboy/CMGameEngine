#include<hgl/ut/Hash.h>
#include<hgl/io/FileInputStream.h>

namespace hgl
{
	namespace util
	{
		Hash *CreateAdler32Hash();
		Hash *CreateCRC32Hash();
		Hash *CreateMD4Hash();
		Hash *CreateMD5Hash();
		Hash *CreateSHA1Hash();

		using CreateHashFunc=Hash *(*)();
	}//namespace util

	namespace util
	{
		Hash *CreateHash(HASH_ALGORITHML ha)
		{
			if(ha<=hashNone||ha>=hashEnd)return(nullptr);

			const CreateHashFunc hash_func[hashEnd-1]=
			{
				CreateCRC32Hash,
				CreateMD4Hash,
				CreateMD5Hash,
				CreateSHA1Hash
			};

			return hash_func[ha-1]();
		}

		bool CountHash(const void *data,int size,HASH_ALGORITHML ha,void *hash_code)
		{
			if(!data||size<=0||!hash_code||ha<=hashNone||ha>=hashEnd)return(false);

			Hash *h=CreateHash(ha);

			if(!h)return(false);

			h->Init();
			h->Update(data,size);
			h->Final(hash_code);

			delete h;
			return(true);
		}

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
	}//namespace util
}//namespace hgl
