#ifndef HGL_AL_INCLUDE
#define HGL_AL_INCLUDE

#if defined(_WIN32)
	#define AL_APIENTRY __cdecl
#else
	#define AL_APIENTRY
#endif

#define OPENAL
#define ALAPI AL_API
#define ALAPIENTRY AL_APIENTRY
#define AL_VERSION_1_0
#define AL_VERSION_1_1
#define AL_INVALID                                -1
#define AL_NONE                                   0
#define AL_FALSE                                  0
#define AL_TRUE                                   1
#define AL_SOURCE_RELATIVE                        0x202
#define AL_CONE_INNER_ANGLE                       0x1001
#define AL_CONE_OUTER_ANGLE                       0x1002
#define AL_PITCH                                  0x1003
#define AL_POSITION                               0x1004
#define AL_DIRECTION                              0x1005
#define AL_VELOCITY                               0x1006
#define AL_LOOPING                                0x1007
#define AL_BUFFER                                 0x1009
#define AL_GAIN                                   0x100A
#define AL_MIN_GAIN                               0x100D
#define AL_MAX_GAIN                               0x100E
#define AL_ORIENTATION                            0x100F
#define AL_CHANNEL_MASK                           0x3000
#define AL_SOURCE_STATE                           0x1010
#define AL_INITIAL                                0x1011
#define AL_PLAYING                                0x1012
#define AL_PAUSED                                 0x1013
#define AL_STOPPED                                0x1014
#define AL_BUFFERS_QUEUED                         0x1015
#define AL_BUFFERS_PROCESSED                      0x1016
#define AL_SEC_OFFSET                             0x1024
#define AL_SAMPLE_OFFSET                          0x1025
#define AL_BYTE_OFFSET                            0x1026
#define AL_SOURCE_TYPE                            0x1027
#define AL_STATIC                                 0x1028
#define AL_STREAMING                              0x1029
#define AL_UNDETERMINED                           0x1030
#define AL_FORMAT_MONO8                           0x1100
#define AL_FORMAT_MONO16                          0x1101
#define AL_FORMAT_STEREO8                         0x1102
#define AL_FORMAT_STEREO16                        0x1103
#define AL_REFERENCE_DISTANCE                     0x1020
#define AL_ROLLOFF_FACTOR                         0x1021
#define AL_CONE_OUTER_GAIN                        0x1022
#define AL_MAX_DISTANCE                           0x1023
#define AL_FREQUENCY                              0x2001
#define AL_BITS                                   0x2002
#define AL_CHANNELS                               0x2003
#define AL_SIZE                                   0x2004
#define AL_UNUSED                                 0x2010
#define AL_PENDING                                0x2011
#define AL_PROCESSED                              0x2012
#define AL_NO_ERROR                               AL_FALSE
#define AL_INVALID_NAME                           0xA001
#define AL_ILLEGAL_ENUM                           0xA002
#define AL_INVALID_ENUM                           0xA002
#define AL_INVALID_VALUE                          0xA003
#define AL_ILLEGAL_COMMAND                        0xA004
#define AL_INVALID_OPERATION                      0xA004
#define AL_OUT_OF_MEMORY                          0xA005
#define AL_VENDOR                                 0xB001
#define AL_VERSION                                0xB002
#define AL_RENDERER                               0xB003
#define AL_EXTENSIONS                             0xB004
#define AL_DOPPLER_FACTOR                         0xC000
#define AL_DOPPLER_VELOCITY                       0xC001
#define AL_SPEED_OF_SOUND                         0xC003
#define AL_DISTANCE_MODEL                         0xD000
#define AL_INVERSE_DISTANCE                       0xD001
#define AL_INVERSE_DISTANCE_CLAMPED               0xD002
#define AL_LINEAR_DISTANCE                        0xD003
#define AL_LINEAR_DISTANCE_CLAMPED                0xD004
#define AL_EXPONENT_DISTANCE                      0xD005
#define AL_EXPONENT_DISTANCE_CLAMPED              0xD006

namespace openal
{
	typedef char ALboolean;
	typedef char ALchar;
	typedef char ALbyte;
	typedef unsigned char ALubyte;
	typedef short ALshort;
	typedef unsigned short ALushort;
	typedef int ALint;
	typedef unsigned int ALuint;
	typedef int ALsizei;
	typedef int ALenum;
	typedef float ALfloat;
	typedef double ALdouble;
	typedef void ALvoid;
}

namespace openal
{
	typedef void (*alEnablePROC)( ALenum capability );
	typedef void (*alDisablePROC)( ALenum capability );
	typedef ALboolean (*alIsEnabledPROC)( ALenum capability );
	typedef const ALchar* (*alGetStringPROC)( ALenum param );
	typedef void (*alGetBooleanvPROC)( ALenum param, ALboolean* data );
	typedef void (*alGetIntegervPROC)( ALenum param, ALint* data );
	typedef void (*alGetFloatvPROC)( ALenum param, ALfloat* data );
	typedef void (*alGetDoublevPROC)( ALenum param, ALdouble* data );
	typedef ALboolean (*alGetBooleanPROC)( ALenum param );
	typedef ALint (*alGetIntegerPROC)( ALenum param );
	typedef ALfloat (*alGetFloatPROC)( ALenum param );
	typedef ALdouble (*alGetDoublePROC)( ALenum param );
	typedef ALenum (*alGetErrorPROC)( void );
	typedef ALboolean (*alIsExtensionPresentPROC)( const ALchar* extname );
	typedef void* (*alGetProcAddressPROC)( const ALchar* fname );
	typedef ALenum (*alGetEnumValuePROC)( const ALchar* ename );
	typedef void (*alListenerfPROC)( ALenum param, ALfloat value );
	typedef void (*alListener3fPROC)( ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 );
	typedef void (*alListenerfvPROC)( ALenum param, const ALfloat* values );
	typedef void (*alListeneriPROC)( ALenum param, ALint value );
	typedef void (*alListener3iPROC)( ALenum param, ALint value1, ALint value2, ALint value3 );
	typedef void (*alListenerivPROC)( ALenum param, const ALint* values );
	typedef void (*alGetListenerfPROC)( ALenum param, ALfloat* value );
	typedef void (*alGetListener3fPROC)( ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3 );
	typedef void (*alGetListenerfvPROC)( ALenum param, ALfloat* values );
	typedef void (*alGetListeneriPROC)( ALenum param, ALint* value );
	typedef void (*alGetListener3iPROC)( ALenum param, ALint *value1, ALint *value2, ALint *value3 );
	typedef void (*alGetListenerivPROC)( ALenum param, ALint* values );
	typedef void (*alGenSourcesPROC)( ALsizei n, ALuint* sources );
	typedef void (*alDeleteSourcesPROC)( ALsizei n, const ALuint* sources );
	typedef ALboolean (*alIsSourcePROC)( ALuint sid );
	typedef void (*alSourcefPROC)( ALuint sid, ALenum param, ALfloat value );
	typedef void (*alSource3fPROC)( ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 );
	typedef void (*alSourcefvPROC)( ALuint sid, ALenum param, const ALfloat* values );
	typedef void (*alSourceiPROC)( ALuint sid, ALenum param, ALint value );
	typedef void (*alSource3iPROC)( ALuint sid, ALenum param, ALint value1, ALint value2, ALint value3 );
	typedef void (*alSourceivPROC)( ALuint sid, ALenum param, const ALint* values );
	typedef void (*alGetSourcefPROC)( ALuint sid, ALenum param, ALfloat* value );
	typedef void (*alGetSource3fPROC)( ALuint sid, ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3);
	typedef void (*alGetSourcefvPROC)( ALuint sid, ALenum param, ALfloat* values );
	typedef void (*alGetSourceiPROC)( ALuint sid,  ALenum param, ALint* value );
	typedef void (*alGetSource3iPROC)( ALuint sid, ALenum param, ALint* value1, ALint* value2, ALint* value3);
	typedef void (*alGetSourceivPROC)( ALuint sid,  ALenum param, ALint* values );
	typedef void (*alSourcePlayvPROC)( ALsizei ns, const ALuint *sids );
	typedef void (*alSourceStopvPROC)( ALsizei ns, const ALuint *sids );
	typedef void (*alSourceRewindvPROC)( ALsizei ns, const ALuint *sids );
	typedef void (*alSourcePausevPROC)( ALsizei ns, const ALuint *sids );
	typedef void (*alSourcePlayPROC)( ALuint sid );
	typedef void (*alSourceStopPROC)( ALuint sid );
	typedef void (*alSourceRewindPROC)( ALuint sid );
	typedef void (*alSourcePausePROC)( ALuint sid );
	typedef void (*alSourceQueueBuffersPROC)( ALuint sid, ALsizei numEntries, const ALuint *bids );
	typedef void (*alSourceUnqueueBuffersPROC)( ALuint sid, ALsizei numEntries, ALuint *bids );
	typedef void (*alGenBuffersPROC)( ALsizei n, ALuint* buffers );
	typedef void (*alDeleteBuffersPROC)( ALsizei n, const ALuint* buffers );
	typedef ALboolean (*alIsBufferPROC)( ALuint bid );
	typedef void (*alBufferDataPROC)( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq );
	typedef void (*alBufferfPROC)( ALuint bid, ALenum param, ALfloat value );
	typedef void (*alBuffer3fPROC)( ALuint bid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 );
	typedef void (*alBufferfvPROC)( ALuint bid, ALenum param, const ALfloat* values );
	typedef void (*alBufferiPROC)( ALuint bid, ALenum param, ALint value );
	typedef void (*alBuffer3iPROC)( ALuint bid, ALenum param, ALint value1, ALint value2, ALint value3 );
	typedef void (*alBufferivPROC)( ALuint bid, ALenum param, const ALint* values );
	typedef void (*alGetBufferfPROC)( ALuint bid, ALenum param, ALfloat* value );
	typedef void (*alGetBuffer3fPROC)( ALuint bid, ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3);
	typedef void (*alGetBufferfvPROC)( ALuint bid, ALenum param, ALfloat* values );
	typedef void (*alGetBufferiPROC)( ALuint bid, ALenum param, ALint* value );
	typedef void (*alGetBuffer3iPROC)( ALuint bid, ALenum param, ALint* value1, ALint* value2, ALint* value3);
	typedef void (*alGetBufferivPROC)( ALuint bid, ALenum param, ALint* values );
	typedef void (*alDopplerFactorPROC)( ALfloat value );
	typedef void (*alDopplerVelocityPROC)( ALfloat value );
	typedef void (*alSpeedOfSoundPROC)( ALfloat value );
	typedef void (*alDistanceModelPROC)( ALenum distanceModel );
}

namespace openal
{
	extern alEnablePROC alEnable;
	extern alDisablePROC alDisable;
	extern alIsEnabledPROC alIsEnabled;
	extern alGetStringPROC alGetString;
	extern alGetBooleanvPROC alGetBooleanv;
	extern alGetIntegervPROC alGetIntegerv;
	extern alGetFloatvPROC alGetFloatv;
	extern alGetDoublevPROC alGetDoublev;
	extern alGetBooleanPROC alGetBoolean;
	extern alGetIntegerPROC alGetInteger;
	extern alGetFloatPROC alGetFloat;
	extern alGetDoublePROC alGetDouble;
	extern alGetErrorPROC alGetError;
	extern alIsExtensionPresentPROC alIsExtensionPresent;
	extern alGetProcAddressPROC alGetProcAddress;
	extern alGetEnumValuePROC alGetEnumValue;
	extern alListenerfPROC alListenerf;
	extern alListener3fPROC alListener3f;
	extern alListenerfvPROC alListenerfv;
	extern alListeneriPROC alListeneri;
	extern alListener3iPROC alListener3i;
	extern alListenerivPROC alListeneriv;
	extern alGetListenerfPROC alGetListenerf;
	extern alGetListener3fPROC alGetListener3f;
	extern alGetListenerfvPROC alGetListenerfv;
	extern alGetListeneriPROC alGetListeneri;
	extern alGetListener3iPROC alGetListener3i;
	extern alGetListenerivPROC alGetListeneriv;
	extern alGenSourcesPROC alGenSources;
	extern alDeleteSourcesPROC alDeleteSources;
	extern alIsSourcePROC alIsSource;
	extern alSourcefPROC alSourcef;
	extern alSource3fPROC alSource3f;
	extern alSourcefvPROC alSourcefv;
	extern alSourceiPROC alSourcei;
	extern alSource3iPROC alSource3i;
	extern alSourceivPROC alSourceiv;
	extern alGetSourcefPROC alGetSourcef;
	extern alGetSource3fPROC alGetSource3f;
	extern alGetSourcefvPROC alGetSourcefv;
	extern alGetSourceiPROC alGetSourcei;
	extern alGetSource3iPROC alGetSource3i;
	extern alGetSourceivPROC alGetSourceiv;
	extern alSourcePlayvPROC alSourcePlayv;
	extern alSourceStopvPROC alSourceStopv;
	extern alSourceRewindvPROC alSourceRewindv;
	extern alSourcePausevPROC alSourcePausev;
	extern alSourcePlayPROC alSourcePlay;
	extern alSourceStopPROC alSourceStop;
	extern alSourceRewindPROC alSourceRewind;
	extern alSourcePausePROC alSourcePause;
	extern alSourceQueueBuffersPROC alSourceQueueBuffers;
	extern alSourceUnqueueBuffersPROC alSourceUnqueueBuffers;
	extern alGenBuffersPROC alGenBuffers;
	extern alDeleteBuffersPROC alDeleteBuffers;
	extern alIsBufferPROC alIsBuffer;
	extern alBufferDataPROC alBufferData;
	extern alBufferfPROC alBufferf;
	extern alBuffer3fPROC alBuffer3f;
	extern alBufferfvPROC alBufferfv;
	extern alBufferiPROC alBufferi;
	extern alBuffer3iPROC alBuffer3i;
	extern alBufferivPROC alBufferiv;
	extern alGetBufferfPROC alGetBufferf;
	extern alGetBuffer3fPROC alGetBuffer3f;
	extern alGetBufferfvPROC alGetBufferfv;
	extern alGetBufferiPROC alGetBufferi;
	extern alGetBuffer3iPROC alGetBuffer3i;
	extern alGetBufferivPROC alGetBufferiv;
	extern alDopplerFactorPROC alDopplerFactor;
	extern alDopplerVelocityPROC alDopplerVelocity;
	extern alSpeedOfSoundPROC alSpeedOfSound;
	extern alDistanceModelPROC alDistanceModel;
}
#endif//HGL_AL_INCLUDE
