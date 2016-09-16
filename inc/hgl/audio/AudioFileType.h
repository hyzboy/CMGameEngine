#ifndef HGL_AUDIO_FILE_TYPE_INCLUDE
#define HGL_AUDIO_FILE_TYPE_INCLUDE

#include<hgl/platform/Platform.h>
namespace hgl
{
	/**
	* 音频文件格式
	*/
	enum AudioFileType
	{
		aftNone=0,			///<起始定义，如使用表示自动根据扩展名识别

		aftWAV,             ///<Wav音波文件
		aftVorbis,          ///<Vorbis OGG文件
        aftOpus,            ///<Opus OGG文件

		aftEnd
	};//enum AudioFileType

	AudioFileType CheckAudioExtName(const os_char *ext_name);
	AudioFileType CheckAudioFileType(const os_char *filename);
}//namespace hgl
#endif//HGL_AUDIO_FILE_TYPE_INCLUDE
