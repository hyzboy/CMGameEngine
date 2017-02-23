#include<hgl/audio/AudioFileType.h>
#include<hgl/Str.h>

namespace hgl
{
    struct AudioFormatExt
    {
        os_char name[8];
        AudioFileType type;
    };//struct AudioFormatExt

    const AudioFormatExt audio_format_ext_name[]=
    {
        {OS_TEXT("wav"),  aftWAV      },
        {OS_TEXT("ogg"),  aftVorbis   },
        {OS_TEXT("opus"), aftOpus     },
        {OS_TEXT(""), aftNone }
    };

    AudioFileType CheckAudioExtName(const os_char *ext_name)
    {
        auto *afp=audio_format_ext_name;

        while(afp->type)
        {
            if(hgl::strcmp(ext_name,afp->name)==0)
                return(afp->type);

            ++afp;
        }

        return(aftNone);
    }

    AudioFileType CheckAudioFileType(const os_char *filename)
    {
        const os_char *ext;
        os_char extname[16];

        ext=hgl::strrchr(filename,'.');

        if(!ext)
            return(aftNone);

        ++ext;

        hgl::strcpy(extname,16,ext);
        hgl::tolower(extname);

        return CheckAudioExtName(extname);
    }

    const os_char audio_decode_name[aftEnd][32]=
    {
        OS_TEXT(""),
        OS_TEXT("Wav"),
        OS_TEXT("Vorbis"),
        OS_TEXT("Opus")
    };

    const os_char *GetAudioDecodeName(const AudioFileType aft)
    {
        if(aft<=aftNone||aft>=aftEnd)return(nullptr);

        return audio_decode_name[aft];
    }
}//namespace hgl
