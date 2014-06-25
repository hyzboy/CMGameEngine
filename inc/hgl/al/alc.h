#ifndef HGL_ALC_INCLUDE
#define HGL_ALC_INCLUDE

#define ALCAPI ALC_API
#define ALCAPIENTRY ALC_APIENTRY
#define ALC_INVALID 0
#define ALC_VERSION_0_1         1
#define ALC_FALSE                                0
#define ALC_TRUE                                 1
#define ALC_FREQUENCY                            0x1007
#define ALC_REFRESH                              0x1008
#define ALC_SYNC                                 0x1009
#define ALC_MONO_SOURCES                         0x1010
#define ALC_STEREO_SOURCES                       0x1011
#define ALC_NO_ERROR                             ALC_FALSE
#define ALC_INVALID_DEVICE                       0xA001
#define ALC_INVALID_CONTEXT                      0xA002
#define ALC_INVALID_ENUM                         0xA003
#define ALC_INVALID_VALUE                        0xA004
#define ALC_OUT_OF_MEMORY                        0xA005
#define ALC_DEFAULT_DEVICE_SPECIFIER             0x1004
#define ALC_DEVICE_SPECIFIER                     0x1005
#define ALC_EXTENSIONS                           0x1006
#define ALC_MAJOR_VERSION                        0x1000
#define ALC_MINOR_VERSION                        0x1001
#define ALC_ATTRIBUTES_SIZE                      0x1002
#define ALC_ALL_ATTRIBUTES                       0x1003
#define ALC_DEFAULT_ALL_DEVICES_SPECIFIER        0x1012
#define ALC_ALL_DEVICES_SPECIFIER                0x1013
#define ALC_CAPTURE_DEVICE_SPECIFIER             0x310
#define ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER     0x311
#define ALC_CAPTURE_SAMPLES                      0x312

namespace openal
{
	typedef struct ALCdevice_struct ALCdevice;
	typedef struct ALCcontext_struct ALCcontext;
	typedef char ALCboolean;
	typedef char ALCchar;
	typedef char ALCbyte;
	typedef unsigned char ALCubyte;
	typedef short ALCshort;
	typedef unsigned short ALCushort;
	typedef int ALCint;
	typedef unsigned int ALCuint;
	typedef int ALCsizei;
	typedef int ALCenum;
	typedef float ALCfloat;
	typedef double ALCdouble;
	typedef void ALCvoid;
}

namespace openal
{
	typedef ALCcontext *     (*alcCreateContextPROC)( ALCdevice *device, const ALCint* attrlist );
	typedef ALCboolean       (*alcMakeContextCurrentPROC)( ALCcontext *context );
	typedef void             (*alcProcessContextPROC)( ALCcontext *context );
	typedef void             (*alcSuspendContextPROC)( ALCcontext *context );
	typedef void             (*alcDestroyContextPROC)( ALCcontext *context );
	typedef ALCcontext *     (*alcGetCurrentContextPROC)( void );
	typedef ALCdevice*       (*alcGetContextsDevicePROC)( ALCcontext *context );
	typedef ALCdevice *      (*alcOpenDevicePROC)( const ALCchar *devicename );
	typedef ALCboolean       (*alcCloseDevicePROC)( ALCdevice *device );
	typedef ALCenum          (*alcGetErrorPROC)( ALCdevice *device );
	typedef ALCboolean       (*alcIsExtensionPresentPROC)( ALCdevice *device, const ALCchar *extname );
	typedef void  *          (*alcGetProcAddressPROC)( ALCdevice *device, const ALCchar *funcname );
	typedef ALCenum          (*alcGetEnumValuePROC)( ALCdevice *device, const ALCchar *enumname );
	typedef const ALCchar *  (*alcGetStringPROC)( ALCdevice *device, ALCenum param );
	typedef void             (*alcGetIntegervPROC)( ALCdevice *device, ALCenum param, ALCsizei size, ALCint *data );
	typedef ALCdevice*       (*alcCaptureOpenDevicePROC)( const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize );
	typedef ALCboolean       (*alcCaptureCloseDevicePROC)( ALCdevice *device );
	typedef void             (*alcCaptureStartPROC)( ALCdevice *device );
	typedef void             (*alcCaptureStopPROC)( ALCdevice *device );
	typedef void             (*alcCaptureSamplesPROC)( ALCdevice *device, ALCvoid *buffer, ALCsizei samples );
}

namespace openal
{
	extern alcCreateContextPROC alcCreateContext;
	extern alcMakeContextCurrentPROC alcMakeContextCurrent;
	extern alcProcessContextPROC alcProcessContext;
	extern alcSuspendContextPROC alcSuspendContext;
	extern alcDestroyContextPROC alcDestroyContext;
	extern alcGetCurrentContextPROC alcGetCurrentContext;
	extern alcGetContextsDevicePROC alcGetContextsDevice;
	extern alcOpenDevicePROC alcOpenDevice;
	extern alcCloseDevicePROC alcCloseDevice;
	extern alcGetErrorPROC alcGetError;
	extern alcIsExtensionPresentPROC alcIsExtensionPresent;
	extern alcGetProcAddressPROC alcGetProcAddress;
	extern alcGetEnumValuePROC alcGetEnumValue;
	extern alcGetStringPROC alcGetString;
	extern alcGetIntegervPROC alcGetIntegerv;
	extern alcCaptureOpenDevicePROC alcCaptureOpenDevice;
	extern alcCaptureCloseDevicePROC alcCaptureCloseDevice;
	extern alcCaptureStartPROC alcCaptureStart;
	extern alcCaptureStopPROC alcCaptureStop;
	extern alcCaptureSamplesPROC alcCaptureSamples;
}
#endif//HGL_ALC_INCLUDE
