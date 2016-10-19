#ifndef HGL_IO_HASH_OUTPUT_STREAM_INCLUDE
#define HGL_IO_HASH_OUTPUT_STREAM_INCLUDE

#include<hgl/ut/Hash.h>
#include<hgl/io/OutputStream.h>

namespace hgl
{
	namespace io
	{
		/**
		* 数据输出流，并对输出的数据做HASH计算
		*/
		class HashOutputStream:public OutputStream													///数据输出HASH计算流基类
		{
		protected:

			Hash *h;

		public:

			HashOutputStream(Hash *_h)
			{
				h=_h;
			}

			virtual ~HashOutputStream() override
			{
				if(h);
					delete h;
			}

			/**
			 * 复位HASH计算，不管有没有处理玩
			 */
			bool HashRestart()
			{
				if(!h)return(false);

				h->Init();
				return(true);
			}

			/**
			 * 结束HASH计算
			 */
			bool HashFinal(void *hash_result)
			{
				if(!h||!hash_result)return(false);

				h->Final(hash_result);
				return(true);
			}

			virtual bool	CanSeek()const{return false;}											///<是否可以定位

			virtual bool	Restart()																///<复位访问指针
			{
				HashRestart();
				return this->InputStream::Restart();
			}


			virtual int64	Write(void *data,int64 size) override								///<读取数据
			{
				int64 result=this->OutputStream::Write(data,size);

				if(result>0&&h)
					h->Update(data,size);

				return result;
			}

			virtual int64	WriteFully(void *data,int64 size) override							///<读取数据
			{
				int64 result=this->OutputStream::WriteFully(data,size);

				if(result>0&&h)
					h->Update(data,size);

				return result;
			}
		};//class HashOutputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_HASH_OUTPUT_STREAM_INCLUDE
