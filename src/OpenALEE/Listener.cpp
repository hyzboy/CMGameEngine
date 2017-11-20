#include<hgl/audio/Listener.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/LogInfo.h>

using namespace openal;
namespace hgl
{
    inline void alGetListenerfv(ALenum param, Vector3f &v3f)
    {
        openal::alGetListenerfv(param, (ALfloat *)&v3f);
    }

    inline void alGetListenerfv(ALenum param, ListenerOrientation &v3f)
    {
        openal::alGetListenerfv(param, (ALfloat *)&v3f);
    }

    inline void alListenerfv(ALenum param, const Vector3f &v3f)
    {
        openal::alListenerfv(param, (const ALfloat *)&v3f);
    }

    inline void alListenerfv(ALenum param, const ListenerOrientation &v3f)
    {
        openal::alListenerfv(param, (const ALfloat *)&v3f);
    }

    AudioListener::AudioListener()
    {
        if(!alGetListenerf)
        {
            LOG_ERROR(OS_TEXT("OpenAL/EE 还未初始化!"));
            return;
        }

        alGetListenerf(AL_GAIN,&gain);
        alGetListenerfv(AL_POSITION,position);
        alGetListenerfv(AL_VELOCITY,velocity);
        alGetListenerfv(AL_ORIENTATION,orientation);
    }

    AudioListener::~AudioListener()
    {
    }

    void AudioListener::SetGain(float _gain)
    {
        gain=_gain;
        alListenerf(AL_GAIN,gain);
    }

    void AudioListener::SetPosition(const Vector3f &pos)
    {
        position=pos;
        alListenerfv(AL_POSITION,position);
    }

    void AudioListener::SetVelocity(const Vector3f &vel)
    {
        velocity=vel;
        alListenerfv(AL_VELOCITY,velocity);
    }

    void AudioListener::SetOrientation(const ListenerOrientation &ori)
    {
        memcpy(&orientation,&ori,sizeof(ListenerOrientation));
        alListenerfv(AL_ORIENTATION,orientation);
    }
}//namespace hgl
