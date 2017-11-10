#include<hgl/audio/OpenAL.h>
#include<hgl/audio/Listener.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/audio/AudioPlayer.h>

using namespace hgl;
using namespace openal;

HGL_PLUGIN_FUNC AudioListener *CreateListener()
{
    return(new AudioListener());
}

HGL_PLUGIN_FUNC void ClearListener(AudioListener *listener)
{
    if(listener)
        delete listener;
}

HGL_PLUGIN_FUNC const float	ListenerGetGain			(AudioListener *listener)
{
    if (!listener)
        return(0);

    return listener->GetGain();
}

HGL_PLUGIN_FUNC const bool	ListenerGetPosition		(AudioListener *listener, Vector3f &pos)
{
    if (!listener)
        return(false);

    pos = listener->GetPosition();
    return(true);
}

HGL_PLUGIN_FUNC const bool	ListenerGetVelocity		(AudioListener *listener, Vector3f &vel)
{
    if (!listener)
        return(false);

    vel = listener->GetVelocity();
    return(true);
}

HGL_PLUGIN_FUNC const bool	ListenerGetOrientation	(AudioListener *listener, ListenerOrientation &	ori)
{
    if (!listener)return(false);
    memcpy(&ori, &(listener->GetOrientation()), sizeof(ListenerOrientation));
    return(true);
}

HGL_PLUGIN_FUNC void ListenerSetGain		(AudioListener *listener,		float					gain) {if (listener)listener->SetGain(gain); }
HGL_PLUGIN_FUNC void ListenerSetPosition	(AudioListener *listener, const Vector3f &				pos	) {if (listener)listener->SetPosition(pos);}
HGL_PLUGIN_FUNC void ListenerSetVelocity	(AudioListener *listener, const Vector3f &				vel	) {if (listener)listener->SetVelocity(vel);}                                                         ///<设置速率
HGL_PLUGIN_FUNC void ListenerSetOrientation	(AudioListener *listener, const ListenerOrientation &	ori	) {if (listener)listener->SetOrientation(ori);}

HGL_PLUGIN_FUNC AudioBuffer *CreateAudioBufferFromData(void *buf, int size, AudioFileType aft){return(new AudioBuffer(buf, size, aft));}
HGL_PLUGIN_FUNC AudioBuffer *CreateAudioBufferFromFile(const os_char *filename, AudioFileType aft){return(new AudioBuffer(filename, aft));}

HGL_PLUGIN_FUNC void ClearAudioBuffer(AudioBuffer *buf){if(buf)delete buf;}

HGL_PLUGIN_FUNC bool SetAudioBufferData(AudioBuffer *audio_buffer, uint format, const void *data, uint size, uint freq)
{
    if (!audio_buffer)return(false);

    return audio_buffer->SetData(format, data, size, freq);
}

HGL_PLUGIN_FUNC bool LoadDataToAudioBuffer(AudioBuffer *audio_buffer, void *data, int size, AudioFileType aft)
{
    if (!audio_buffer)return(false);

    return audio_buffer->Load(data, size, aft);
}

HGL_PLUGIN_FUNC bool LoadFileToAudioBuffer(AudioBuffer *audio_buffer, const os_char *filename, AudioFileType aft)
{
    if (!audio_buffer)return(false);

    return audio_buffer->Load(filename, aft);
}

HGL_PLUGIN_FUNC void ClearAudioBufferData(AudioBuffer *audio_buffer)
{
    if (!audio_buffer)return;

    audio_buffer->Clear();
}

HGL_PLUGIN_FUNC bool SourceGetPosition	(AudioSource *source,Vector3f &pos){if(!source)return(false);pos=source->GetPosition();return(true);}
HGL_PLUGIN_FUNC bool SourceGetVelocity	(AudioSource *source,Vector3f &vel){if(!source)return(false);vel=source->GetVelocity();return(true);}
HGL_PLUGIN_FUNC bool SourceGetDirection	(AudioSource *source,Vector3f &dir){if(!source)return(false);dir=source->GetDirection();return(true);}
HGL_PLUGIN_FUNC bool SourceGetAngle		(AudioSource *source,ConeAngle &ca){if(!source)return(false);ca =source->GetAngle();return(true);}

HGL_PLUGIN_FUNC void SourceSetPosition			(AudioSource *source,const Vector3f &pos	){if(source)source->SetPosition(pos);}
HGL_PLUGIN_FUNC void SourceSetVelocity			(AudioSource *source,const Vector3f &vel	){if(source)source->SetVelocity(vel);}
HGL_PLUGIN_FUNC void SourceSetDirection			(AudioSource *source,const Vector3f &dir	){if(source)source->SetDirection(dir);}
HGL_PLUGIN_FUNC void SourceSetDistance			(AudioSource *source,const float &ref_distance, const float &max_distance){if(source)source->SetDistance(ref_distance,max_distance);}
HGL_PLUGIN_FUNC void SourceSetConeAngle			(AudioSource *source,const ConeAngle &ca	){if(source)source->SetConeAngle(ca);}
HGL_PLUGIN_FUNC void SourceSetDopplerFactor		(AudioSource *source,const float &df		){if(source)source->SetDopplerFactor(df);}
HGL_PLUGIN_FUNC void SourceSetDopplerVelocity	(AudioSource *source,const float &dv		){if(source)source->SetDopplerVelocity(dv);}

HGL_PLUGIN_FUNC AudioSource *CreateAudioSource(bool create){return(new AudioSource(create));}
HGL_PLUGIN_FUNC AudioSource *CreateAudioSourceFromBuffer(AudioBuffer *buf){return(new AudioSource(buf));}
HGL_PLUGIN_FUNC void ClearAudioSource(AudioSource *source){if(source)delete source;}

HGL_PLUGIN_FUNC bool SourcePlay		(AudioSource *source,bool loop){return source?source->Play(loop):false;}
HGL_PLUGIN_FUNC void SourcePause	(AudioSource *source){if(source)source->Pause();}
HGL_PLUGIN_FUNC void SourceResume	(AudioSource *source){if(source)source->Resume();}
HGL_PLUGIN_FUNC void SourceStop		(AudioSource *source){if(source)source->Stop();}
HGL_PLUGIN_FUNC void SourceRewind	(AudioSource *source){if(source)source->Rewind();}

HGL_PLUGIN_FUNC bool SourceCreate	(AudioSource *source){return source?source->Create():false;}
HGL_PLUGIN_FUNC void SourceClose	(AudioSource *source){if(source)source->Close();}

HGL_PLUGIN_FUNC bool SourceLink		(AudioSource *source,AudioBuffer *buf){return source?source->Link(buf):false;}
HGL_PLUGIN_FUNC void SourceUnlink	(AudioSource *source){if(source)source->Unlink();}


HGL_PLUGIN_FUNC uint    				PlayerGetSource		(AudioPlayer *ap){return ap?ap->GetIndex():0;}
HGL_PLUGIN_FUNC double					PlayerGetTime		(AudioPlayer *ap){return ap?ap->GetTime():0;}
HGL_PLUGIN_FUNC AudioPlayer::PlayState	PlayerGetPlayState	(AudioPlayer *ap){return ap?ap->GetPlayState():(AudioPlayer::psNone);}
HGL_PLUGIN_FUNC int     				PlayerGetSourceState(AudioPlayer *ap){return ap?ap->GetSourceState():0;}
HGL_PLUGIN_FUNC float   				PlayerGetMinGain	(AudioPlayer *ap){return ap?ap->GetMinGain():0;}
HGL_PLUGIN_FUNC float   				PlayerGetMaxGain	(AudioPlayer *ap){return ap?ap->GetMaxGain():0;}

HGL_PLUGIN_FUNC float   PlayerGetPitch(AudioPlayer *ap){return ap?ap->GetPitch():0;}
HGL_PLUGIN_FUNC void    PlayerSetPitch(AudioPlayer *ap,float val){if(ap)ap->SetPitch(val);}

HGL_PLUGIN_FUNC float   PlayerGetGain(AudioPlayer *ap){return ap?ap->GetGain():0;}
HGL_PLUGIN_FUNC void    PlayerSetGain(AudioPlayer *ap,float val){if(ap)ap->SetGain(val);}

HGL_PLUGIN_FUNC float   PlayerGetConeGain(AudioPlayer *ap){return ap?ap->GetConeGain():0;}
HGL_PLUGIN_FUNC void    PlayerSetConeGain(AudioPlayer *ap,float val){if(ap)ap->SetConeGain(val);}

HGL_PLUGIN_FUNC float   PlayerGetRolloffFactor(AudioPlayer *ap){return ap?ap->GetRolloffFactor():0;}
HGL_PLUGIN_FUNC void    PlayerSetRolloffFactor(AudioPlayer *ap,float rf){if(ap)ap->SetRolloffFactor(rf);}

HGL_PLUGIN_FUNC bool	PlayerGetPosition(AudioPlayer *ap,Vector3f &pos){if(!ap)return(false);pos=ap->GetPosition();return(true);}
HGL_PLUGIN_FUNC void	PlayerSetPosition(AudioPlayer *ap,const Vector3f &pos){if(!ap)return;ap->SetPosition(pos);}

HGL_PLUGIN_FUNC bool	PlayerGetVelocity(AudioPlayer *ap,Vector3f &vel){if(!ap)return(false);vel=ap->GetVelocity();return(true);}
HGL_PLUGIN_FUNC void	PlayerSetVelocity(AudioPlayer *ap,const Vector3f &vel){if(!ap)return;ap->SetVelocity(vel);}

HGL_PLUGIN_FUNC bool	PlayerGetDirection(AudioPlayer *ap,Vector3f &dir){if(!ap)return(false);dir=ap->GetDirection();return(true);}
HGL_PLUGIN_FUNC void	PlayerSetDirection(AudioPlayer *ap,const Vector3f &dir){if(!ap)return;ap->SetDirection(dir);}

HGL_PLUGIN_FUNC void	PlayerGetDistance(AudioPlayer *ap,float &ref_distance, float &max_distance){if(!ap)return;ap->GetDistance(ref_distance,max_distance);}
HGL_PLUGIN_FUNC void	PlayerSetDistance(AudioPlayer *ap,const float &ref_distance,const float &max_distance){if(!ap)return;ap->SetDistance(ref_distance,max_distance);}

HGL_PLUGIN_FUNC bool	PlayerGetConeAngle(AudioPlayer *ap,ConeAngle &ca){if(!ap)return(false);ca=ap->GetConeAngle();return(true);}
HGL_PLUGIN_FUNC void	PlayerSetConeAngle(AudioPlayer *ap,const ConeAngle &ca){if(!ap)return;ap->SetConeAngle(ca);}

HGL_PLUGIN_FUNC AudioPlayer *	CreateAudioPlayer(){return(new AudioPlayer());}
HGL_PLUGIN_FUNC AudioPlayer *	CreateAudioPlayerFromFile(const os_char *filename,AudioFileType aft){return(new AudioPlayer(filename,aft));}
HGL_PLUGIN_FUNC void			ClearAudioPlayer(AudioPlayer *ap){if(ap)delete ap;}

HGL_PLUGIN_FUNC bool	LoadFileToPlayer(AudioPlayer *ap,const os_char *filename,AudioFileType aft){return ap?ap->Load(filename,aft):false;}

HGL_PLUGIN_FUNC void	PlayerPlay	(AudioPlayer *ap,bool loop){if(ap)ap->Play(loop);}
HGL_PLUGIN_FUNC void	PlayerStop	(AudioPlayer *ap){if(ap)ap->Stop();}
HGL_PLUGIN_FUNC void	PlayerPause	(AudioPlayer *ap){if(ap)ap->Pause();}
HGL_PLUGIN_FUNC void	PlayerResume(AudioPlayer *ap){if(ap)ap->Resume();}
HGL_PLUGIN_FUNC void	PlayerClear	(AudioPlayer *ap){if(ap)ap->Clear();}

HGL_PLUGIN_FUNC double	PlayerGetPlayTime(AudioPlayer *ap){return ap?ap->GetPlayTime():0;}
HGL_PLUGIN_FUNC void	PlayerSetFadeTime(AudioPlayer *ap,double in,double out){if(ap)ap->SetFadeTime(in,out);}

HGL_PLUGIN_FUNC void	PlayerSetAutoGain(AudioPlayer *ap,float target_gain,double adjust_time){if(ap)ap->AutoGain(target_gain,adjust_time);}
