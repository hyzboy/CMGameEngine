#define __MAKE_PLUGIN__

#include<hgl/PlugIn.h>
#include<malloc.h>
#include<string.h>
#include<hgl/al/al.h>

#include<opusfile.h>

using namespace hgl;
using namespace openal;

ALvoid LoadOpus(ALbyte *memory, ALsizei memory_size,ALenum *format, ALvoid **data, ALsizei *size, ALsizei *freq, ALboolean *loop)
{
    OggOpusFile *of;

    int op_error;

    of=op_open_memory((const unsigned char *)memory,memory_size,&op_error);

    if(!of)
        return;

    const OpusHead *head=op_head(of,0);

    if(head->channel_count==1)*format=AL_FORMAT_MONO16;else
    if(head->channel_count==2)*format=AL_FORMAT_STEREO16;else
        return;

    long pcm_total=op_pcm_total(of,-1)*head->channel_count;

    opus_int16 *out_buf=new opus_int16[pcm_total];
    int out_size;
    int out_total_size=0;

    while(-1)
    {
        out_size=op_read(of,out_buf+out_total_size,pcm_total-out_total_size,nullptr);

        if(out_size<=0)
            break;

        out_total_size+=out_size*head->channel_count;
    }

    *freq=head->input_sample_rate;
    *size=out_total_size*2;
    *data=out_buf;
}

void ClearOpus(ALenum,ALvoid *data,ALsizei,ALsizei)
{
    opus_int16 *pcm=(opus_int16 *)data;

    delete[] pcm;
}

void *OpenOpus(ALbyte *memory,ALsizei memory_size,ALenum *format,ALsizei *freq,double *total_time)
{
    OggOpusFile *of;

    int op_error;

    of=op_open_memory((const unsigned char *)memory,memory_size,&op_error);

    if(!of)
        return(nullptr);

    const OpusHead *head=op_head(of,0);

    if(head->channel_count==1)*format=AL_FORMAT_MONO16;else
    if(head->channel_count==2)*format=AL_FORMAT_STEREO16;else
        return(nullptr);

    *freq=head->input_sample_rate;

    *total_time=double(op_pcm_total(of,-1))/double(head->input_sample_rate);

	return(of);
}

void CloseOpus(void *ptr)
{
    OggOpusFile *of=(OggOpusFile *)ptr;

    op_free(of);
}

uint ReadOpus(void *ptr,char *data,uint buf_max)
{
	OggOpusFile *of=(OggOpusFile *)ptr;
	int result;
	uint size=0;
    uint buf_left=buf_max/2;        //16位,所以要除2

	while(size<buf_max)
	{
		result=op_read(of,((opus_int16 *)data)+size,buf_left-size,nullptr);

		if(result>0)size+=result;else
		if(result<=0)break;
	}

	return(size*2);
}

void RestartOpus(void *ptr)
{
	OggOpusFile *of=(OggOpusFile *)ptr;

    op_pcm_seek(of,0);
}
//--------------------------------------------------------------------------------------------------
struct OutInterface
{
	void (*Load)(ALbyte *,ALsizei,ALenum *,ALvoid **,ALsizei *,ALsizei *,ALboolean *);
	void (*Clear)(ALenum,ALvoid *,ALsizei,ALsizei);

	void *(*Open)(ALbyte *,ALsizei,ALenum *,ALsizei *,double *);
	void  (*Close)(void *);
	uint  (*Read)(void *,char *,uint);
	void  (*Restart)(void *);
};

static OutInterface out_interface=
{
	LoadOpus,
	ClearOpus,

	OpenOpus,
	CloseOpus,
	ReadOpus,
	RestartOpus
};
//--------------------------------------------------------------------------------------------------
#if HGL_OS != HGL_OS_Windows
const u16char plugin_intro[]=U16_TEXT("Opus 音频文件解码(使用操作系统内置解码器,2016-09-16)");
#else
const u16char plugin_intro[]=U16_TEXT("Opus 音频文件解码(LibOpus 1.1.3,2016-09-16)");
#endif//

HGL_PLUGIN_FUNC uint32 GetPlugInVersion()
{
	return(2);		//版本号
					//根据版本号取得不同的API
}

HGL_PLUGIN_FUNC u16char * GetPlugInIntro()
{
	return((u16char *)plugin_intro);
}

HGL_PLUGIN_FUNC bool GetPlugInInterface(uint32 ver,void *data)
{
	if(ver==2)
	{
		memcpy(data,&out_interface,sizeof(OutInterface));
	}
	else
		return(false);

	return(true);
}

HGL_PLUGIN_FUNC void SetPlugInInterface(uint32,void *)
{
}
//--------------------------------------------------------------------------------------------------
