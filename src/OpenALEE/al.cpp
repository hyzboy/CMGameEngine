#include<hgl/al/al.h>
#include<hgl/al/alc.h>
#include<hgl/ExternalModule.h>
#include<hgl/LogInfo.h>

using namespace hgl;

namespace openal
{
	alEnablePROC alEnable=nullptr;
	alDisablePROC alDisable=nullptr;
	alIsEnabledPROC alIsEnabled=nullptr;
	alGetStringPROC alGetString=nullptr;
	alGetBooleanvPROC alGetBooleanv=nullptr;
	alGetIntegervPROC alGetIntegerv=nullptr;
	alGetFloatvPROC alGetFloatv=nullptr;
	alGetDoublevPROC alGetDoublev=nullptr;
	alGetBooleanPROC alGetBoolean=nullptr;
	alGetIntegerPROC alGetInteger=nullptr;
	alGetFloatPROC alGetFloat=nullptr;
	alGetDoublePROC alGetDouble=nullptr;
	alGetErrorPROC alGetError=nullptr;
	alIsExtensionPresentPROC alIsExtensionPresent=nullptr;
	alGetProcAddressPROC alGetProcAddress=nullptr;
	alGetEnumValuePROC alGetEnumValue=nullptr;
	alListenerfPROC alListenerf=nullptr;
	alListener3fPROC alListener3f=nullptr;
	alListenerfvPROC alListenerfv=nullptr;
	alListeneriPROC alListeneri=nullptr;
	alListener3iPROC alListener3i=nullptr;
	alListenerivPROC alListeneriv=nullptr;
	alGetListenerfPROC alGetListenerf=nullptr;
	alGetListener3fPROC alGetListener3f=nullptr;
	alGetListenerfvPROC alGetListenerfv=nullptr;
	alGetListeneriPROC alGetListeneri=nullptr;
	alGetListener3iPROC alGetListener3i=nullptr;
	alGetListenerivPROC alGetListeneriv=nullptr;
	alGenSourcesPROC alGenSources=nullptr;
	alDeleteSourcesPROC alDeleteSources=nullptr;
	alIsSourcePROC alIsSource=nullptr;
	alSourcefPROC alSourcef=nullptr;
	alSource3fPROC alSource3f=nullptr;
	alSourcefvPROC alSourcefv=nullptr;
	alSourceiPROC alSourcei=nullptr;
	alSource3iPROC alSource3i=nullptr;
	alSourceivPROC alSourceiv=nullptr;
	alGetSourcefPROC alGetSourcef=nullptr;
	alGetSource3fPROC alGetSource3f=nullptr;
	alGetSourcefvPROC alGetSourcefv=nullptr;
	alGetSourceiPROC alGetSourcei=nullptr;
	alGetSource3iPROC alGetSource3i=nullptr;
	alGetSourceivPROC alGetSourceiv=nullptr;
	alSourcePlayvPROC alSourcePlayv=nullptr;
	alSourceStopvPROC alSourceStopv=nullptr;
	alSourceRewindvPROC alSourceRewindv=nullptr;
	alSourcePausevPROC alSourcePausev=nullptr;
	alSourcePlayPROC alSourcePlay=nullptr;
	alSourceStopPROC alSourceStop=nullptr;
	alSourceRewindPROC alSourceRewind=nullptr;
	alSourcePausePROC alSourcePause=nullptr;
	alSourceQueueBuffersPROC alSourceQueueBuffers=nullptr;
	alSourceUnqueueBuffersPROC alSourceUnqueueBuffers=nullptr;
	alGenBuffersPROC alGenBuffers=nullptr;
	alDeleteBuffersPROC alDeleteBuffers=nullptr;
	alIsBufferPROC alIsBuffer=nullptr;
	alBufferDataPROC alBufferData=nullptr;
	alBufferfPROC alBufferf=nullptr;
	alBuffer3fPROC alBuffer3f=nullptr;
	alBufferfvPROC alBufferfv=nullptr;
	alBufferiPROC alBufferi=nullptr;
	alBuffer3iPROC alBuffer3i=nullptr;
	alBufferivPROC alBufferiv=nullptr;
	alGetBufferfPROC alGetBufferf=nullptr;
	alGetBuffer3fPROC alGetBuffer3f=nullptr;
	alGetBufferfvPROC alGetBufferfv=nullptr;
	alGetBufferiPROC alGetBufferi=nullptr;
	alGetBuffer3iPROC alGetBuffer3i=nullptr;
	alGetBufferivPROC alGetBufferiv=nullptr;
	alDopplerFactorPROC alDopplerFactor=nullptr;
	alDopplerVelocityPROC alDopplerVelocity=nullptr;
	alSpeedOfSoundPROC alSpeedOfSound=nullptr;
	alDistanceModelPROC alDistanceModel=nullptr;
}

namespace openal
{
	bool LoadALFunc(hgl::ExternalModule *module)
	{
		if(!module)RETURN_FALSE;

		alGetProcAddress=(alGetProcAddressPROC)(module->GetFunc("alGetProcAddress"));

		if(!alGetProcAddress)RETURN_FALSE;

		HGL_FUNC_LOAD_LIST_BEGIN(al_func_load_list)
			HGL_FUNC_LOAD(alGetProcAddress)

			HGL_FUNC_LOAD(alEnable)
			HGL_FUNC_LOAD(alDisable)
			HGL_FUNC_LOAD(alIsEnabled)
			HGL_FUNC_LOAD(alGetString)
			HGL_FUNC_LOAD(alGetBooleanv)
			HGL_FUNC_LOAD(alGetIntegerv)
			HGL_FUNC_LOAD(alGetFloatv)
			HGL_FUNC_LOAD(alGetDoublev)
			HGL_FUNC_LOAD(alGetBoolean)
			HGL_FUNC_LOAD(alGetInteger)
			HGL_FUNC_LOAD(alGetFloat)
			HGL_FUNC_LOAD(alGetDouble)
			HGL_FUNC_LOAD(alGetError)
			HGL_FUNC_LOAD(alIsExtensionPresent)
			HGL_FUNC_LOAD(alGetEnumValue)
			HGL_FUNC_LOAD(alListenerf)
			HGL_FUNC_LOAD(alListener3f)
			HGL_FUNC_LOAD(alListenerfv)
			HGL_FUNC_LOAD(alListeneri)
			HGL_FUNC_LOAD(alListener3i)
			HGL_FUNC_LOAD(alListeneriv)
			HGL_FUNC_LOAD(alGetListenerf)
			HGL_FUNC_LOAD(alGetListener3f)
			HGL_FUNC_LOAD(alGetListenerfv)
			HGL_FUNC_LOAD(alGetListeneri)
			HGL_FUNC_LOAD(alGetListener3i)
			HGL_FUNC_LOAD(alGetListeneriv)
			HGL_FUNC_LOAD(alGenSources)
			HGL_FUNC_LOAD(alDeleteSources)
			HGL_FUNC_LOAD(alIsSource)
			HGL_FUNC_LOAD(alSourcef)
			HGL_FUNC_LOAD(alSource3f)
			HGL_FUNC_LOAD(alSourcefv)
			HGL_FUNC_LOAD(alSourcei)
			HGL_FUNC_LOAD(alSource3i)
			HGL_FUNC_LOAD(alSourceiv)
			HGL_FUNC_LOAD(alGetSourcef)
			HGL_FUNC_LOAD(alGetSource3f)
			HGL_FUNC_LOAD(alGetSourcefv)
			HGL_FUNC_LOAD(alGetSourcei)
			HGL_FUNC_LOAD(alGetSource3i)
			HGL_FUNC_LOAD(alGetSourceiv)
			HGL_FUNC_LOAD(alSourcePlayv)
			HGL_FUNC_LOAD(alSourceStopv)
			HGL_FUNC_LOAD(alSourceRewindv)
			HGL_FUNC_LOAD(alSourcePausev)
			HGL_FUNC_LOAD(alSourcePlay)
			HGL_FUNC_LOAD(alSourceStop)
			HGL_FUNC_LOAD(alSourceRewind)
			HGL_FUNC_LOAD(alSourcePause)
			HGL_FUNC_LOAD(alSourceQueueBuffers)
			HGL_FUNC_LOAD(alSourceUnqueueBuffers)
			HGL_FUNC_LOAD(alGenBuffers)
			HGL_FUNC_LOAD(alDeleteBuffers)
			HGL_FUNC_LOAD(alIsBuffer)
			HGL_FUNC_LOAD(alBufferData)
			HGL_FUNC_LOAD(alBufferf)
			HGL_FUNC_LOAD(alBuffer3f)
			HGL_FUNC_LOAD(alBufferfv)
			HGL_FUNC_LOAD(alBufferi)
			HGL_FUNC_LOAD(alBuffer3i)
			HGL_FUNC_LOAD(alBufferiv)
			HGL_FUNC_LOAD(alGetBufferf)
			HGL_FUNC_LOAD(alGetBuffer3f)
			HGL_FUNC_LOAD(alGetBufferfv)
			HGL_FUNC_LOAD(alGetBufferi)
			HGL_FUNC_LOAD(alGetBuffer3i)
			HGL_FUNC_LOAD(alGetBufferiv)
			HGL_FUNC_LOAD(alDopplerFactor)
			HGL_FUNC_LOAD(alDopplerVelocity)
			HGL_FUNC_LOAD(alSpeedOfSound)
			HGL_FUNC_LOAD(alDistanceModel)
		HGL_FUNC_LOAD_LIST_END

		FuncLoad *flp=al_func_load_list;

		while(flp->func_pointer)
		{
			(*(flp->func_pointer))=alGetProcAddress(flp->func_name);

			if(!(*(flp->func_pointer)))
				(*(flp->func_pointer))=module->GetFunc(flp->func_name);

			++flp;
		}

		return(true);
	}

	void ClearAL()
	{
// 		ClearFuncLoadPointer(al_func_load_list);
	}
}//namespace openal
