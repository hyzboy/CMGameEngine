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


ALvoid LoadOpusFloat32(ALbyte *memory, ALsizei memory_size,ALenum *format, float **data, ALsizei *size, ALsizei *freq, ALboolean *loop)
{
    OggOpusFile *of;

    int op_error;

    of=op_open_memory((const unsigned char *)memory,memory_size,&op_error);

    if(!of)
        return;

    const OpusHead *head=op_head(of,0);

    if(head->channel_count==1)*format=AL_FORMAT_MONO_FLOAT32;else
    if(head->channel_count==2)*format=AL_FORMAT_STEREO_FLOAT32;else
        return;

    long pcm_total=op_pcm_total(of,-1)*head->channel_count;

    float *out_buf=new float[pcm_total];
    int out_size;
    int out_total_size=0;

    while(-1)
    {
        out_size=op_read_float(of,out_buf+out_total_size,pcm_total-out_total_size,nullptr);

        if(out_size<=0)
            break;

        out_total_size+=out_size*head->channel_count;
    }

    *freq=head->input_sample_rate;
    *size=out_total_size*sizeof(float);
    *data=out_buf;
}

void ClearOpusFloat32(ALenum,float *data,ALsizei,ALsizei)
{
    delete[] data;
}

struct OpusStream
{
    OggOpusFile *of;
    const OpusHead *head;
};

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

    OpusStream *os=new OpusStream;

    os->of=of;
    os->head=head;

    return os;
}

void CloseOpus(void *ptr)
{
    OpusStream *os=(OpusStream *)ptr;

    op_free(os->of);

    delete os;
}

uint ReadOpus(void *ptr,char *data,uint buf_max)
{
    OpusStream *os=(OpusStream *)ptr;
    int result;
    uint size=0;
    uint buf_left=buf_max/2;        //16位,所以要除2

    while(size<buf_max)
    {
        result=op_read(os->of,((opus_int16 *)data)+size,buf_left-size,nullptr);

        if(result>0)size+=result*os->head->channel_count;else
        if(result<=0)break;
    }

    return(size*2);
}

uint ReadOpusFloat32(void *ptr,float *data,uint buf_max)
{
    OpusStream *os=(OpusStream *)ptr;
    int result;
    uint size=0;
    uint buf_left=buf_max/sizeof(float);

    while(size<buf_max)
    {
        result=op_read_float(os->of,data+size,buf_left-size,nullptr);

        if(result>0)size+=result*os->head->channel_count;else
        if(result<=0)break;
    }

    return(size*sizeof(float));
}

void RestartOpus(void *ptr)
{
    OpusStream *os=(OpusStream *)ptr;

    op_pcm_seek(os->of,0);
}
//--------------------------------------------------------------------------------------------------
struct OutInterface2
{
    void (*Load)(ALbyte *,ALsizei,ALenum *,ALvoid **,ALsizei *,ALsizei *,ALboolean *);
    void (*Clear)(ALenum,ALvoid *,ALsizei,ALsizei);

    void *(*Open)(ALbyte *,ALsizei,ALenum *,ALsizei *,double *);
    void  (*Close)(void *);
    uint  (*Read)(void *,char *,uint);
    void  (*Restart)(void *);
};

static OutInterface2 out_interface_2=
{
    LoadOpus,
    ClearOpus,

    OpenOpus,
    CloseOpus,
    ReadOpus,
    RestartOpus
};

struct OutInterface3
{
    void (*Load)(ALbyte *,ALsizei,ALenum *,float **,ALsizei *,ALsizei *,ALboolean *);
    void (*Clear)(ALenum,float *,ALsizei,ALsizei);

    uint (*Read)(void *,float *,uint);
};

static OutInterface3 out_interface_3
{
    LoadOpusFloat32,
    ClearOpusFloat32,

    ReadOpusFloat32
};
//--------------------------------------------------------------------------------------------------
#if HGL_OS != HGL_OS_Windows
const u16char plugin_intro[]=U16_TEXT("Opus 音频文件解码(使用操作系统内置解码器,2016-09-16)");
#else
const u16char plugin_intro[]=U16_TEXT("Opus 音频文件解码(LibOpus 1.1.3,LibOpusFile 0.7,LibOGG 1.3.2,2016-09-16)");
#endif//

HGL_PLUGIN_FUNC uint32 GetPlugInVersion()
{
    return(3);        //版本号
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
        memcpy(data,&out_interface_2,sizeof(OutInterface2));
    }
    else
    if(ver==3)
    {
        memcpy(data,&out_interface_3,sizeof(OutInterface3));
    }
    else
        return(false);

    return(true);
}

HGL_PLUGIN_FUNC void SetPlugInInterface(uint32,void *)
{
}
//--------------------------------------------------------------------------------------------------
