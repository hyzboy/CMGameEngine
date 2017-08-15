#include<hgl/audio/OpenAL.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/audio/AudioPlayer.h>

using namespace hgl;
using namespace openal;

extern "C"
{
	HGL_PLUGIN_FUNC bool InitOpenALEE();
}//extern "C"
