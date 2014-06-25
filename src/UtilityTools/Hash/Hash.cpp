#include<hgl/ut/Hash.h>
#include<apr_md5.h>
#include<apr_md4.h>
#include<apr_sha1.h>
#include<hgl/io/FileInputStream.h>

namespace hgl
{
	namespace util
	{
		class MD5:public Hash
		{
			apr_md5_ctx_t ctx;

		public:

			const int GetHashBytes()const HGL_OVERRIDE{return APR_MD5_DIGESTSIZE;}

			void Init()HGL_OVERRIDE{apr_md5_init(&ctx);}
			void Update(const void *data,int size)HGL_OVERRIDE{apr_md5_update(&ctx,data,size);}
			void GetResult(void *result)HGL_OVERRIDE{apr_md5_final((uchar *)result,&ctx);}
		};//class MD5

		class MD4:public Hash
		{
			apr_md4_ctx_t ctx;

		public:

			const int GetHashBytes()const HGL_OVERRIDE{return APR_MD4_DIGESTSIZE;}

			void Init()HGL_OVERRIDE{apr_md4_init(&ctx);}
			void Update(const void *data,int size)HGL_OVERRIDE{apr_md4_update(&ctx,(const uchar *)data,size);}
			void GetResult(void *result)HGL_OVERRIDE{apr_md4_final((uchar *)result,&ctx);}
		};//class MD4

		class SHA1:public Hash
		{
			apr_sha1_ctx_t ctx;

		public:

			const int GetHashBytes()const HGL_OVERRIDE{return APR_SHA1_DIGESTSIZE;}

			void Init()HGL_OVERRIDE{apr_sha1_init(&ctx);}
			void Update(const void *data,int size)HGL_OVERRIDE{apr_sha1_update_binary(&ctx,(const uchar *)data,size);}
			void GetResult(void *result)HGL_OVERRIDE{apr_sha1_final((uchar *)result,&ctx);}
		};//class MD5
	}//namespace util

	namespace util
	{
		Hash *CreateHash(HASH_ALGORITHML ha)
		{
			if(ha<=hashNone||ha>=hashEnd)return(nullptr);

			if(ha==hashMD5)return(new MD5);else
			if(ha==hashMD4)return(new MD4);else
			if(ha==hashSHA1)return(new SHA1);else
				return(nullptr);
		}

		bool CountHash(const void *data,int size,void *hash_code,HASH_ALGORITHML ha)
		{
			if(!data||size<=0||!hash_code||ha<=hashNone||ha>=hashEnd)return(false);

			if(ha==hashMD5)
			{
				apr_md5_ctx_t ctx;

				apr_md5_init(&ctx);
				apr_md5_update(&ctx,(const uchar *)data,size);
				apr_md5_final((uchar *)hash_code,&ctx);
			}
			else
			if(ha==hashMD4)
			{
				apr_md4_ctx_t ctx;

				apr_md4_init(&ctx);
				apr_md4_update(&ctx,(const uchar *)data,size);
				apr_md4_final((uchar *)hash_code,&ctx);
			}
			else
			if(ha==hashSHA1)
			{
				apr_sha1_ctx_t ctx;

				apr_sha1_init(&ctx);
				apr_sha1_update_binary(&ctx,(const uchar *)data,size);
				apr_sha1_final((uchar *)hash_code,&ctx);
			}
			else return(false);

			return(true);
		}

		bool GetFileHash(const OSString &filename,void *hash_code,HASH_ALGORITHML ha)
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

			hash->GetResult(hash_code);
			fp.Close();
			delete hash;

			return(true);
		}
	}//namespace util
}//namespace hgl
