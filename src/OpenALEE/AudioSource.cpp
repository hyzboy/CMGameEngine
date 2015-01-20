#include<hgl/audio/AudioSource.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/LogInfo.h>

using namespace openal;
namespace hgl
{
	inline void alSourcefv(openal::ALuint sid, openal::ALenum param, const Vector3f &v3f)
	{
		openal::alSourcefv(sid, param, (const openal::ALfloat *)&v3f);
	}

	inline void alGetSourcefv(openal::ALuint sid, openal::ALenum param, Vector3f &v3f)
	{
		openal::alGetSourcefv(sid, param, (openal::ALfloat *)&v3f);
	}

	const unsigned int InvalidIndex=0xFFFFFFFF;

	void AudioSource::InitPrivate()
	{
		index=InvalidIndex;
		Buffer=nullptr;

		hglSetPropertyRead(	Index,			this,AudioSource::GetIndex);

		hglSetPropertyRead(	CurTime,		this,AudioSource::GetCurTime);

		hglSetPropertyRead(	State,			this,AudioSource::GetState);
		hglSetPropertyRead(	MinGain,		this,AudioSource::GetMinGain);
		hglSetPropertyRead(	MaxGain,		this,AudioSource::GetMaxGain);

		hglSetProperty(		Loop,			this,AudioSource::GetLoop,			AudioSource::SetLoop);

		hglSetProperty(		Pitch,			this,AudioSource::GetPitch,			AudioSource::SetPitch);
		hglSetProperty(		Gain,			this,AudioSource::GetGain,			AudioSource::SetGain);
		hglSetProperty(		ConeGain,		this,AudioSource::GetConeGain,		AudioSource::SetConeGain);

		hglSetProperty(		RolloffFactor,	this,AudioSource::GetRolloffFactor,	AudioSource::SetRolloffFactor	);
	}

	/**
	* 音频发声源基类
	* @param create 是否创建发声源
	*/
	AudioSource::AudioSource(bool create)
	{
		InitPrivate();

		if(create)Create();
	}

	/**
	* 音频发声源基类
	* @param ab 音频缓冲区
	*/
	AudioSource::AudioSource(AudioBuffer *ab)
	{
		InitPrivate();

		Create();

		Link(ab);
	}

	AudioSource::~AudioSource()
	{
		Close();
	}

	double AudioSource::GetCurTime()
	{
		if(!alGetSourcei)return(0);
		if(index==InvalidIndex)return(0);

		int offset;

		alGetSourcei(index,AL_BYTE_OFFSET,&offset);

		return(double(offset)/double(Buffer->Size)*double(Buffer->Time));
	}

	int AudioSource::GetState()
	{
		if(!alGetSourcei)return(0);
		if(index==InvalidIndex)return(AL_NONE);

		int state;

		alGetSourcei(index,AL_SOURCE_STATE,&state);

		return(state);
	}

	float AudioSource::GetMinGain()
	{
		if(!alGetSourcef)return(0);
		if(index==InvalidIndex)return(0);

		float min;

		alGetSourcef(index,AL_MIN_GAIN,&min);

		return(min);
	}

	float AudioSource::GetMaxGain()
	{
		if(!alGetSourcef)return(0);
		if(index==InvalidIndex)return(0);

		float max;

		alGetSourcef(index,AL_MIN_GAIN,&max);

		return(max);
	}

	void AudioSource::SetLoop(bool _loop)
	{
		if(!alSourcei)return;
		if(index==InvalidIndex)return;

		loop=_loop;
		alSourcei(index,AL_LOOPING,loop);
	}

	void AudioSource::SetPitch(float _pitch)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		pitch=_pitch;
		alSourcef(index,AL_PITCH,pitch);
	}

	void AudioSource::SetGain(float _gain)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		gain=_gain;
		alSourcef(index,AL_GAIN,gain);
	}

	void AudioSource::SetConeGain(float _gain)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		cone_gain=_gain;
		alSourcef(index,AL_CONE_OUTER_GAIN,cone_gain);
	}

	void AudioSource::SetPosition(const Vector3f &pos)
	{
		if(!openal::alSourcefv)return;
		if(index==InvalidIndex)return;

		position=pos;
		alSourcefv(index,AL_POSITION,position);
	}

	void AudioSource::SetVelocity(const Vector3f &vel)
	{
		if(!openal::alSourcefv)return;
		if(index==InvalidIndex)return;

		velocity=vel;
		alSourcefv(index,AL_VELOCITY,velocity);
	}

	void AudioSource::SetDirection(const Vector3f &dir)
	{
		if(!openal::alSourcefv)return;
		if(index==InvalidIndex)return;

		direction=dir;
		alSourcefv(index,AL_DIRECTION,direction);
	}

	void AudioSource::SetDistance(const ReferenceValue &dist)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		distance=dist;
		alSourcef(index,AL_REFERENCE_DISTANCE,dist.half);
		alSourcef(index,AL_MAX_DISTANCE,dist.max);
	}

	void AudioSource::SetRolloffFactor(float rf)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		rolloff_factor=rf;
		alSourcef(index,AL_ROLLOFF_FACTOR,rolloff_factor);
	}

	void AudioSource::SetAngle(const ConeAngle &ca)
	{
		if(!alSourcef)return;
		if(index==InvalidIndex)return;

		angle=ca;
		alSourcef(index,AL_CONE_INNER_ANGLE,ca.inner);
		alSourcef(index,AL_CONE_OUTER_ANGLE,ca.outer);
	}

	bool AudioSource::Play(bool _loop)
	{
		if(!alSourcePlay)return(false);
		if(index==InvalidIndex)return(false);
		if(!Buffer
		  ||Buffer->Time<=0)return(false);

		if(State==AL_PLAYING)
			alSourceStop(index);

		alSourcePlay(index);
		alSourcei(index,AL_LOOPING,loop=_loop);

		pause=false;

		return(!alLastError());
	}

	void AudioSource::Pause()
	{
		if(!alSourcePlay)return;
		if(!alSourcePause)return;

		if(index==InvalidIndex)return;

		if(!pause)
		{
			alSourcePause(index);
			pause=true;
		}
	}

	void AudioSource::Resume()
	{
		if(!alSourcePlay)return;
		if(!alSourcePause)return;

		if(index==InvalidIndex)return;

		if(pause)
		{
			alSourcePlay(index);
			pause=false;
		}
	}

	void AudioSource::Stop()
	{
		if(!alSourceStop)return;
		if(index==InvalidIndex)return;

		alSourceStop(index);
	}

	void AudioSource::Rewind()
	{
		if(!alSourceRewind)return;
		if(index==InvalidIndex)return;

		alSourceRewind(index);
	}

	/**
	* 创建一个发声源
	* @return 发声源是否创建成功
	*/
	bool AudioSource::Create()
	{
		if(!alGenSources)
		{
			LOG_INFO(OS_TEXT("OpenAL/EE 还未初始化!"));
			return(false);
		}

		if(index!=InvalidIndex)Close();

    	alGetError();		//清空错误

		alGenSources(1,&index);

		if(alLastError())
		{
			index=InvalidIndex;

			LOG_INFO(OS_TEXT("无法再创建音频播放源了！"));
			return(false);
		}

		loop=false;

		alGetSourcef	(index,AL_PITCH,				&pitch);
		alGetSourcef	(index,AL_GAIN,					&gain);
		alGetSourcef	(index,AL_CONE_OUTER_GAIN,		&cone_gain);
		alGetSourcefv	(index,AL_POSITION,				position);
		alGetSourcefv	(index,AL_VELOCITY,				velocity);
		alGetSourcefv	(index,AL_DIRECTION,			direction);
		alGetSourcef	(index,AL_MAX_DISTANCE,			&distance.max);
		alGetSourcef	(index,AL_REFERENCE_DISTANCE,	&distance.half);
		alGetSourcef	(index,AL_ROLLOFF_FACTOR,		&rolloff_factor);
		alGetSourcef	(index,AL_CONE_INNER_ANGLE,		&angle.inner);
		alGetSourcef	(index,AL_CONE_OUTER_ANGLE,		&angle.outer);

		return(true);
	}

	/**
	* 关闭发声源
	*/
	void AudioSource::Close()
	{
		if(!alDeleteSources)return;
		if(index==InvalidIndex)return;

		alSourceStop(index);
		alSourcei(index,AL_BUFFER,0);
		alDeleteSources(1,&index);
	}

	/**
	* 绑定一个音频数据缓冲区到当前发声源上
	* @param buf 要绑定的音频数据缓冲区
	* @return 是否绑定成功
	*/
	bool AudioSource::Link(AudioBuffer *buf)
	{
    	if(!buf)return(false);
		if(!buf->Time)return(false);
		if(!alSourcei)return(false);
		if(index==InvalidIndex)
		{
			if(!Create())return(false);
		}
		else
			Stop();


		alSourcei(index,AL_BUFFER,buf->Index);

		Buffer=buf;

		return(!alLastError());
	}

	/**
	* 解决绑定在当前音频源上的数据缓冲区
	*/
	void AudioSource::Unlink()
	{
		if(!alSourcei)return;
		if(index==InvalidIndex)return;

		Buffer=nullptr;

		alSourcei(index,AL_BUFFER,0);
	}
}//namespace hgl
