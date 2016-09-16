#ifndef HGL_AUDIO_DECODE_INCLUDE
#define HGL_AUDIO_DECODE_INCLUDE

#include<hgl/audio/OpenAL.h>
#include<hgl/type/BaseString.h>

using namespace openal;
namespace hgl
{
	struct AudioPlugInInterface
	{
		void	(AL_APIENTRY *Load		)(ALbyte *,ALsizei,ALenum *,ALvoid **,ALsizei *,ALsizei *,ALboolean *);
		void	(AL_APIENTRY *Clear		)(ALenum,ALvoid *,ALsizei,ALsizei);

		void *	(AL_APIENTRY *Open		)(ALbyte *,ALsizei,ALenum *,ALsizei *,double *);
		void	(AL_APIENTRY *Close		)(void *);
		uint	(AL_APIENTRY *Read		)(void *,char *,uint);
		void	(AL_APIENTRY *Restart	)(void *);
	};//struct AudioPlugInInterface

	AudioPlugInInterface *AudioInterfaceCheck(const OSString &);

    struct AudioFloatPlugInInterface
	{
		void	(AL_APIENTRY *Load		)(ALbyte *,ALsizei,ALenum *,float **,ALsizei *,ALsizei *,ALboolean *);
		void	(AL_APIENTRY *Clear		)(ALenum,ALvoid *,ALsizei,ALsizei);

		uint	(AL_APIENTRY *Read		)(void *,float *,uint);
	};//struct AudioPlugInInterface

	AudioFloatPlugInInterface *AudioFloatInterfaceCheck(const OSString &);
}//namespace hgl
#endif//HGL_AUDIO_DECODE_INCLUDE
