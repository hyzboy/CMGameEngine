#ifndef HGL_AUDIO_BUFFER_INCLUDE
#define HGL_AUDIO_BUFFER_INCLUDE

#include<hgl/io/InputStream.h>
// #include<hgl/HAC.H>
#include<hgl/type/ObjectBuffer.h>
#include<hgl/audio/AudioFileType.h>

namespace hgl
{
	using namespace io;

	/**
	* AudioBuffer是一个简单的音频数据管理类
	*/
	class AudioBuffer                                                                               ///音频数据缓冲区类
	{
		bool ok;

		void InitPrivate();

	public:

		uint      Index;
		double    Time;                                                                          	///<缓冲区中音频数据可以播放的时间(秒)
		uint      Size;																				///<缓冲区中音频数据的总字节数
		uint      Freq;                                                                             ///<音频数量采样率

	public:

		AudioBuffer(void *,int,AudioFileType);														///<本类构造函数
		AudioBuffer(InputStream *,int,AudioFileType);												///<本类构造函数
		AudioBuffer(const os_char *filename=0,AudioFileType=aftNone);								///<本类构造函数
// 		AudioBuffer(HAC *,const os_char *,AudioFileType=aftNone);	   								///<本类构造函数
		virtual ~AudioBuffer();                                                                     ///<本类析构函数

		bool SetData(uint format,const void *data,uint size,uint freq);

		bool Load(void *,int,AudioFileType);														///<从内存中加载音频数据
		bool Load(InputStream *,int,AudioFileType);													///<从流中加载音频数据
		bool Load(const os_char *,AudioFileType=aftNone);											///<从文件中加载音频数据
// 		bool Load(HAC *,const os_char *,AudioFileType=aftNone);										///<从HAC包的文件中加载音频数据

		void Clear();                                                                               ///<清除数据
	};//class AudioBuffer

//	typedef ObjectBuffer<AudioBuffer> 			AudioBufferBuffer;									///<AudioBuffer缓冲管理器
// 	typedef ObjectBufferFromHAC<AudioBuffer> 	AudioBufferHAC;										///<AudioBuffer缓冲管理器(从HAC名中取得文件)
}//namespace hgl
#endif//AUDIO_BUFFER_INCLUDE
