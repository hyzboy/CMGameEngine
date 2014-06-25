#include<hgl/al/alc.h>
#include<hgl/ExternalModule.h>
#include<hgl/LogInfo.h>

using namespace hgl;

namespace openal
{
	alcCreateContextPROC alcCreateContext=nullptr;
	alcMakeContextCurrentPROC alcMakeContextCurrent=nullptr;
	alcProcessContextPROC alcProcessContext=nullptr;
	alcSuspendContextPROC alcSuspendContext=nullptr;
	alcDestroyContextPROC alcDestroyContext=nullptr;
	alcGetCurrentContextPROC alcGetCurrentContext=nullptr;
	alcGetContextsDevicePROC alcGetContextsDevice=nullptr;
	alcOpenDevicePROC alcOpenDevice=nullptr;
	alcCloseDevicePROC alcCloseDevice=nullptr;
	alcGetErrorPROC alcGetError=nullptr;
	alcIsExtensionPresentPROC alcIsExtensionPresent=nullptr;
	alcGetProcAddressPROC alcGetProcAddress=nullptr;
	alcGetEnumValuePROC alcGetEnumValue=nullptr;
	alcGetStringPROC alcGetString=nullptr;
	alcGetIntegervPROC alcGetIntegerv=nullptr;
	alcCaptureOpenDevicePROC alcCaptureOpenDevice=nullptr;
	alcCaptureCloseDevicePROC alcCaptureCloseDevice=nullptr;
	alcCaptureStartPROC alcCaptureStart=nullptr;
	alcCaptureStopPROC alcCaptureStop=nullptr;
	alcCaptureSamplesPROC alcCaptureSamples=nullptr;
}

namespace openal
{
	bool LoadALCFunc(hgl::ExternalModule *module)
	{
		if(!module)RETURN_FALSE;

		HGL_FUNC_LOAD_LIST_BEGIN(alc_func_load_list)
			HGL_FUNC_LOAD(alcGetProcAddress)

			HGL_FUNC_LOAD(alcCreateContext)
			HGL_FUNC_LOAD(alcMakeContextCurrent)
			HGL_FUNC_LOAD(alcProcessContext)
			HGL_FUNC_LOAD(alcSuspendContext)
			HGL_FUNC_LOAD(alcDestroyContext)
			HGL_FUNC_LOAD(alcGetCurrentContext)
			HGL_FUNC_LOAD(alcGetContextsDevice)
			HGL_FUNC_LOAD(alcOpenDevice)
			HGL_FUNC_LOAD(alcCloseDevice)
			HGL_FUNC_LOAD(alcGetError)
			HGL_FUNC_LOAD(alcIsExtensionPresent)
			HGL_FUNC_LOAD(alcGetEnumValue)
			HGL_FUNC_LOAD(alcGetString)
			HGL_FUNC_LOAD(alcGetIntegerv)
			HGL_FUNC_LOAD(alcCaptureOpenDevice)
			HGL_FUNC_LOAD(alcCaptureCloseDevice)
			HGL_FUNC_LOAD(alcCaptureStart)
			HGL_FUNC_LOAD(alcCaptureStop)
			HGL_FUNC_LOAD(alcCaptureSamples)
		HGL_FUNC_LOAD_LIST_END

		return module->Get(alc_func_load_list);
	}

	void ClearALC()
	{
// 		ClearFuncLoadPointer(alc_func_load_list);
	}
}//namespace openal
