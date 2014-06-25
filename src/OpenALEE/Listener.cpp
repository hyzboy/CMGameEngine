#include<hgl/audio/Listener.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/LogInfo.h>

using namespace openal;
namespace hgl
{
    AudioListener::AudioListener()
    {
		if(!alGetListenerf)
		{
			LOG_ERROR(u"OpenAL/EE 还未初始化!");
			return;
		}

        orientation=new Vector3f[2];

        alGetListenerf(AL_GAIN,&gain);
        alGetListenerfv(AL_POSITION,(ALfloat *)&position);
        alGetListenerfv(AL_VELOCITY,(ALfloat *)&velocity);
        alGetListenerfv(AL_ORIENTATION,(ALfloat *)&orientation);
    }

    AudioListener::~AudioListener()
    {
        delete[] orientation;
    }

    void AudioListener::SetGain(float _gain)
    {
        gain=_gain;
        alListenerf(AL_GAIN,gain);
    }

	void AudioListener::SetPosition(const Vector3f &pos)
    {
        position=pos;
        alListenerfv(AL_POSITION,position.data());
    }

    void AudioListener::SetVelocity(const Vector3f &vel)
    {
        velocity=vel;
        alListenerfv(AL_VELOCITY,velocity.data());
    }

    void AudioListener::SetOrientation(const Vector3f *ori)
    {
        memcpy(orientation,ori,2*sizeof(Vector3f));
        alListenerfv(AL_ORIENTATION,(float *)orientation);
    }
}//namespace hgl
