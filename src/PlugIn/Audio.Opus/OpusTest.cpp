#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>

#include<opus/opusfile.h>

#define OPUS_SAMPLEWIDTH 2

void opus_dec(const unsigned char *data,int size)
{
    OggOpusFile *of;

    int op_error;

    of=op_open_memory(data,size,&op_error);

    if(!of)
    {
        printf("op_open_memory return error: %d\n",op_error);
        return;
    }

    printf("op_open_memory ok\n");

    long long raw_total=op_raw_total(of,-1);

    printf("raw total = %lld\n",raw_total);

    long pcm_total=op_pcm_total(of,-1);

    printf("pcm total = %li\n",pcm_total);

    ogg_int64_t pcm_offset;

    pcm_offset=op_pcm_tell(of);

    printf("PCM offset %ld\n",pcm_offset);

    int prev_li=-1;
    int li=op_current_link(of);

    printf("current link %d\n",li);

    const OpusHead *head=op_head(of,li);

    printf("channel count %d\n",head->channel_count);
    printf("input sample rate %u\n",head->input_sample_rate);
    printf("output gain %d\n",head->output_gain);
    printf("mapping family %d\n",head->mapping_family);
    printf("stream count %d\n",head->stream_count);

    printf("OPUS_SAMPLEWIDTH %d\n",OPUS_SAMPLEWIDTH);

    double total_time=double(pcm_total)/double(head->input_sample_rate);

    printf("time %f\n",total_time);

    pcm_total*=head->channel_count;

    opus_int16 *buf=new opus_int16[pcm_total];
    int total_out_size=0;
    int out_size;

    while(-1)
    {
        out_size=op_read(of,buf+total_out_size,(pcm_total)-total_out_size,NULL);

        if(out_size<=0)
            break;

        total_out_size+=out_size*head->channel_count;

//        printf("output size %d total output size %d\n",out_size,total_out_size);
    }

    printf("total output size %d\n",total_out_size);
}

int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("OpusTest music.opus\n");
        return 0;
    }

    printf("filename: %s\n",argv[1]);

    int file=open(argv[1],O_RDONLY);

    struct stat64 fs;

    fstat64(file,&fs);

    printf("filesize: %ld\n",fs.st_size);

    unsigned char *data=new unsigned char[fs.st_size];

    read(file,data,fs.st_size);

    close(file);

    opus_dec(data,fs.st_size);
}
