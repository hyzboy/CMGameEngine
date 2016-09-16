#include<hgl/LogInfo.h>
#include<hgl/type/Pair.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/Other.h>
#include<hgl/PlugIn.h>
#include"AudioDecode.h"

using namespace openal;

namespace hgl
{
	using namespace io;

	const os_char *GetAudioDecodeName(const AudioFileType aft);

	double LoadAudioData(int index,AudioFileType aft,void *memory,int memory_size)
	{
		ALenum format;
		ALvoid *data;
		ALsizei size;
		ALsizei freq;
		ALboolean loop;

		const os_char *plugin_name=GetAudioDecodeName(aft);

		if(!plugin_name)RETURN_ERROR(0);

		AudioPlugInInterface *decode=AudioInterfaceCheck(plugin_name);

        if(!decode)RETURN_ERROR(0);

        AudioFloatPlugInInterface *decode_float=nullptr;

        if(IsSupportFloatAudioData())
            decode_float=AudioFloatInterfaceCheck(plugin_name);

        if(decode_float)
            decode_float->Load((ALbyte *)memory, memory_size, &format,(float **)&data, &size, &freq, &loop);
        else
            decode->Load((ALbyte *)memory, memory_size, &format, &data, &size, &freq, &loop);

        alLastError();

		alBufferData(index, format, data, size, freq);

        if(decode_float)
            decode_float->Clear(format, data, size, freq);
        else
            decode->Clear(format, data, size, freq);

		if(alLastError())RETURN_ERROR(0);

    	return AudioDataTime(size,format,freq);
	}

	void AudioBuffer::InitPrivate()
	{
		ok=false;
		Time=0;
		Size=0;
	}

	AudioBuffer::AudioBuffer(void *data,int size,AudioFileType aft)
	{
		InitPrivate();
		Load(data,size,aft);
	}

	AudioBuffer::AudioBuffer(InputStream *str,int size,AudioFileType aft)
	{
		InitPrivate();
		Load(str,size,aft);
	}

	AudioBuffer::AudioBuffer(const os_char *filename,AudioFileType aft)
	{
		InitPrivate();
		if(filename)Load(filename,aft);
	}

// 	AudioBuffer::AudioBuffer(HAC *hac,const os_char *filename,AudioFileType aft)
// 	{
// 		InitPrivate();
// 		Load(hac,filename,aft);
// 	}

	AudioBuffer::~AudioBuffer()
	{
		Clear();
	}

	/**
	* 直接设置音频数据
	* @param format 音频数据格式，可以为“AL_FORMAT_MONO8、AL_FORMAT_MONO16、AL_FORMAT_STEREO16”
	* @param data 数据指针
	* @param size 数据长度
	* @param freq 采样频率
	* @return 音频数据可播放时间
	*/
	bool AudioBuffer::SetData(uint format, const void* data, uint size, uint freq )
	{
		if(!alGenBuffers)RETURN_FALSE;

		Clear();
		alGenBuffers(1,&Index);

		if(alLastError())return(ok=false);

		alBufferData(Index, format, data, size, freq);

		if(alLastError())return(ok=false);

		Time=AudioDataTime(size,format,freq);

		return(true);
	}

	/**
	* 从内存中加载一个音频文件到当前缓冲区,仅支持OGG和WAV。注：由于这个函数会一次性将音频数据载入内存，所以较长的音乐请使用CreateAudioPlayer，以免占用太多的内存。
	* @param memory 要加载数据的内存
	* @param aft 音频文件类型
	* @return 是否加载成功
	*/
	bool AudioBuffer::Load(void *memory,int size,AudioFileType aft)
	{
		if(!alGenBuffers)RETURN_FALSE;

		Clear();
		alGenBuffers(1,&Index);

		if(alLastError())return(ok=false);

		if(aft<=aftNone||aft>=aftEnd)
		{
			LOG_ERROR(OS_TEXT("未知的音频文件类型！AudioFileType:")+OSString(aft));
			alDeleteBuffers(1,&Index);
			RETURN_FALSE;
		}
		else
		{
			Time=LoadAudioData(Index,aft,memory,size);
			Size=size;
		}

		if(Time==0)
		{
			alDeleteBuffers(1,&Index);
			RETURN_FALSE;
		}

		return(ok=true);
	}

	/**
	* 从流中加载一个音频文件到当前缓冲区,仅支持OGG和WAV。注：由于这个函数会一次性将音频数据载入内存，所以较长的音乐请使用CreateAudioPlayer，以免占用太多的内存。
	* @param in 要加载数据的流
	* @param aft 音频文件类型
	* @return 是否加载成功
	*/
	bool AudioBuffer::Load(InputStream *in,int size,AudioFileType aft)
	{
		if(!alGenBuffers)RETURN_FALSE;
		if(!in)RETURN_FALSE;
		if(size<=0)RETURN_FALSE;

		if(aft<=aftNone||aft>=aftEnd)
		{
			LOG_ERROR(OS_TEXT("未知的音频文件类型！AudioFileType:")+OSString(aft));
			ok=false;
		}
		else
		{
			char *memory=new char[size];

			in->Read(memory,size);
			ok=Load(memory,size,aft);

			delete[] memory;
		}

		RETURN_BOOL(ok);
	}

	/**
	* 加载一个音频文件到当前缓冲区，仅支持OGG和WAV。注：由于这个函数会一次性将音频数据载入内存，所以较长的音乐请使用CreateAudioPlayer，以免占用太多的内存。
	* @param filename 音频文件名称
	* @param aft 音频文件类型
	* @return 加载是否成功
	*/
	bool AudioBuffer::Load(const os_char *filename,AudioFileType aft)
	{
		if(!alGenBuffers)RETURN_FALSE;

		aft=CheckAudioFileType(filename);

		if(!aft)
		{
			LOG_ERROR(OS_TEXT("未知的音频文件类型！AudioFile: ")+OSString(filename));
            ok=false;
			RETURN_FALSE;
		}

		OpenFileInputStream fis(filename);

		RETURN_BOOL(Load(fis,fis->Available(),aft));
	}

// 	/**
// 	* 加载一个音频文件到当前缓冲区，仅支持OGG和WAV。注：由于这个函数会一次性将音频数据载入内存，所以较长的音乐请使用CreateAudioPlayer，以免占用太多的内存。
// 	* @param filename 音频文件名称
// 	* @param aft 音频文件类型
// 	* @return 加载是否成功
// 	*/
// 	bool AudioBuffer::Load(HAC *hac,const os_char *filename,AudioFileType aft)
// 	{
// 		if(!alGenBuffers)RETURN_FALSE;
//
// 		os_char *ext;
// 		InputStream *stream;
// 		bool result;
//
// 		ext=strrchr(filename,u'.');
// 		LowerString(ext);
//
// 		if(aft<=aftNone||aft>=aftEnd)
// 		{
// 			if(strcmp(ext,u".ogg")==0)aft=aftOGG;else
// 			if(strcmp(ext,u".wav")==0)aft=aftWAV;else
// 			{
// 				PutError(u"未知的音频文件类型！AudioFileType:%d",aft);
// 				RETURN_FALSE;
// 			}
// 		}
//
// 		stream=hac->LoadFile(filename);
// 		if(stream)
// 		{
// 			result=Load(stream,aft);
// 			delete stream;
//
// 			return(result);
// 		}
//
// 		RETURN_FALSE;
// 	}

	void AudioBuffer::Clear()
	{
		if(!alDeleteBuffers)return;
		if(ok)
		{
			alDeleteBuffers(1,&Index);
			ok=false;
			Time=0;
		}
	}
}//namespace hgl
