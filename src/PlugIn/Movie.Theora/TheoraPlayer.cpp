#include<hgl/type/DataType.h>
#include"TheoraPlayer.h"
#include<string.h>

using namespace hgl;
//--------------------------------------------------------------------------------------------------
static int sAdjCrr[256];
static int sAdjCrg[256];
static int sAdjCbg[256];
static int sAdjCbb[256];
static int sAdjY[256];
static unsigned char  sClampBuff[1024];
static unsigned char *sClamp = sClampBuff + 384;

static unsigned char YV_R[256][256];
static unsigned char YU_B[256][256];

inline int getMin(int a, int b)
{
   return a>b ? b : a;
}

inline int getMax(int a, int b)
{
   return a>b ? a : b;
}

inline int mClamp(int val, int low, int high)
{
   return getMax(getMin(val, high), low);
}

void InitTheoraDecode()
{
    int i;

    for(i = 0; i < 256; i++)
    {
        sAdjCrr[i] = (409 * (i - 128) + 128) >> 8;
        sAdjCrg[i] = (208 * (i - 128) + 128) >> 8;
        sAdjCbg[i] = (100 * (i - 128) + 128) >> 8;
        sAdjCbb[i] = (516 * (i - 128) + 128) >> 8;
        sAdjY[i]   = (298 * (i -  16)      ) >> 8;
    }

    for(i = -384; i < 640; i++)
        sClamp[i] = mClamp(i, 0, 0xFF);

     for(int v=0;v<256;v++)
        for(int y=0;y<256;y++)
        {
            YV_R[v][y]=sClamp[sAdjY[y]+sAdjCrr[v]];
            YU_B[v][y]=sClamp[sAdjY[y]+sAdjCbb[v]];
        }
}
//--------------------------------------------------------------------------------------------------
TheoraPlayer::TheoraPlayer()
{
    in=0;
    isPlaying=false;
    isLoop=false;
    theoraLoaded=false;
    startTime=0;
    decodedFrames=0;
    theora_p = 0;
    stateflag = 0;
    videobuf_granulepos = -1;
    videobuf_time = 0;
    BufferSize=64*1024;
}

TheoraPlayer::~TheoraPlayer()
{
    Close();
}

int TheoraPlayer::buffer_data(ogg_sync_state *oy)
{
    if(!in)return 0;

    char *buffer=ogg_sync_buffer(oy,BufferSize);
    int bytes=Read(in,buffer,BufferSize);
    ogg_sync_wrote(oy,bytes);
    return(bytes);
}

int TheoraPlayer::queue_page(ogg_page * /*page*/)
{
    if (theora_p)ogg_stream_pagein(&to,&og);

    return 0;
}

void TheoraPlayer::Rewind()
{
    if (this->theoraLoaded)
        StopTheora();

    Seek(in,0);

    startTime = 0;
    decodedFrames = 0;

    theora_p = 0;
    stateflag = 0;

    videobuf_granulepos = -1;
    videobuf_time = 0;

    ogg_sync_init(&oy);

    th_comment_init(&tc);
    th_info_init(&ti);
    tdc=NULL;
    tsi=NULL;

    while (!stateflag)
    {
        int ret=buffer_data(&oy);

        if (ret==0)break;

        while (ogg_sync_pageout(&oy,&og) > 0)
        {
            ogg_stream_state test;

            if (!ogg_page_bos(&og))
            {
                queue_page(&og);
                stateflag=1;
                break;
            }

            ogg_stream_init(&test,ogg_page_serialno(&og));
            ogg_stream_pagein(&test,&og);
            ogg_stream_packetout(&test,&op);

            if(!theora_p && th_decode_headerin(&ti,&tc,&tsi,&op)>=0)            //theora 视频流

            {
                memcpy(&to,&test,sizeof(test));
                theora_p=1;
            }
            else
                ogg_stream_clear(&test);
        }
    }

    while (theora_p < 3)
    {
        while (theora_p && theora_p < 3 && ogg_stream_packetout(&to,&op))
        {
            th_decode_headerin(&ti,&tc,&tsi,&op);

            theora_p++;

            if (theora_p==3)break;
        }

        if (ogg_sync_pageout(&oy,&og)>0)
            queue_page(&og);
        else
            buffer_data(&oy);
    }

    if (theora_p)
    {
        tdc=th_decode_alloc(&ti,tsi);

        videoWidth  = ti.frame_width;
        videoHeight = ti.frame_height;
        videoFpS    = ((float)ti.fps_numerator/ti.fps_denominator);

        one_frame_time=1.0f/videoFpS;
        half_frame_time=one_frame_time/2.0f;

        th_decode_ctl(tdc,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max,sizeof(pp_level_max));
        pp_level=0;//pp_level_max;      //可以设为0，能稍降CPU占用
        th_decode_ctl(tdc,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
    }
    else
    {
        th_info_clear(&ti);
        th_comment_clear(&tc);
    }

    if(tsi)
        th_setup_free(tsi);

    stateflag=0;

    while (ogg_sync_pageout(&oy,&og) > 0)
        queue_page(&og);

    this->theoraLoaded = true;
    this->isPlaying = true;
}

void TheoraPlayer::StopTheora()
{
    if (theora_p)
    {
        ogg_stream_clear(&to);

        if(tdc)th_decode_free(tdc);

        th_info_clear(&ti);
        th_comment_clear(&tc);
    }

    ogg_sync_clear(&oy);
    this->isPlaying = false;
}

bool TheoraPlayer::Open(Stream *file,SeekFunc sf,EofFunc ef,ReadFunc rf)
{
    in=file;

    Seek=sf;
    Eof=ef;
    Read=rf;

    Rewind();

    isOpen=true;

    return true;
}

void TheoraPlayer::DecodeToRGB(th_ycbcr_buffer& yuv,unsigned char* buf)
{
    uint8 *dst0 = buf;
    uint8 *dst1 = dst0+ti.frame_width*3;

    const int pictOffset  = yuv[0].stride*ti.pic_y+ti.pic_x;

    const uint8 *pY0, *pY1, *pU, *pV;

    //yuv.y_height,yuv.y_width是以16点对齐的，所以可能会超过frame_width,frame_height

    for(int y = 0; y < ti.frame_height; y += 2)
    {
        pY0 = yuv[0].data +   pictOffset +  y       * (yuv[0].stride);
        pY1 = yuv[0].data +   pictOffset + (y | 1)  * (yuv[0].stride);
        pU  = yuv[1].data + ((pictOffset +  y       * (yuv[1].stride)) >> 1);
        pV  = yuv[2].data + ((pictOffset +  y       * (yuv[2].stride)) >> 1);

        for(int x = 0; x < yuv[0].width; x += 2)
        {
            const int G = sAdjCrg[*pV] + sAdjCbg[*pU];

            const uint8 *r =YV_R[*pV++];
            const uint8 *b =YU_B[*pU++];

            // pixel 0x0
            *dst0++ = r[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = b[*pY0++];

            // pixel 1x0
            *dst0++ = r[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = b[*pY0++];

            // pixel 0x1
            *dst1++ = r[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = b[*pY1++];

            // pixel 1x1
            *dst1++ = r[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = b[*pY1++];
        }

        dst0 = dst1;
        dst1 += ti.frame_width*3;
    }
}

void TheoraPlayer::DecodeToRGBA(th_ycbcr_buffer& yuv,unsigned char* buf)
{
    uint8 *dst0 = buf;
    uint8 *dst1 = dst0+(ti.frame_width<<2);

    const int pictOffset  = yuv[0].stride*ti.pic_y+ti.pic_x;

    const uint8 *pY0, *pY1, *pU, *pV;

    //yuv.y_height,yuv.y_width是以16点对齐的，所以可能会超过frame_width,frame_height

    for(int y = 0; y < ti.frame_height; y += 2)
    {
        pY0 = yuv[0].data +   pictOffset +  y       * (yuv[0].stride);
        pY1 = yuv[0].data +   pictOffset + (y | 1)  * (yuv[0].stride);
        pU  = yuv[1].data + ((pictOffset +  y       * (yuv[1].stride)) >> 1);
        pV  = yuv[2].data + ((pictOffset +  y       * (yuv[2].stride)) >> 1);

        for(int x = 0; x < yuv[0].width; x += 2)
        {
            const int G = sAdjCrg[*pV] + sAdjCbg[*pU];

            const uint8 *r =YV_R[*pV++];
            const uint8 *b =YU_B[*pU++];

            // pixel 0x0
            *dst0++ = r[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = b[*pY0++];
            dst0++;

            // pixel 1x0
            *dst0++ = r[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = b[*pY0++];
            dst0++;

            // pixel 0x1
            *dst1++ = r[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = b[*pY1++];
            dst1++;

            // pixel 1x1
            *dst1++ = r[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = b[*pY1++];
            dst1++;
        }

        dst0 = dst1;
        dst1 += (ti.frame_width<<2);
    }
}

void TheoraPlayer::DecodeToBGR(th_ycbcr_buffer& yuv,unsigned char* buf)
{
    uint8 *dst0 = buf;
    uint8 *dst1 = dst0+ti.frame_width*3;

    const int pictOffset  = yuv[0].stride*ti.pic_y+ti.pic_x;

    const uint8 *pY0, *pY1, *pU, *pV;

    //yuv.y_height,yuv.y_width是以16点对齐的，所以可能会超过frame_width,frame_height

    for(int y = 0; y < ti.frame_height; y += 2)
    {
        pY0 = yuv[0].data +   pictOffset +  y       * (yuv[0].stride);
        pY1 = yuv[0].data +   pictOffset + (y | 1)  * (yuv[0].stride);
        pU  = yuv[1].data + ((pictOffset +  y       * (yuv[1].stride)) >> 1);
        pV  = yuv[2].data + ((pictOffset +  y       * (yuv[2].stride)) >> 1);

        for(int x = 0; x < yuv[0].width; x += 2)
        {
            const int G = sAdjCrg[*pV] + sAdjCbg[*pU];

            const uint8 *r =YV_R[*pV++];
            const uint8 *b =YU_B[*pU++];

            // pixel 0x0
            *dst0++ = b[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = r[*pY0++];

            // pixel 1x0
            *dst0++ = b[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = r[*pY0++];

            // pixel 0x1
            *dst1++ = b[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = r[*pY1++];

            // pixel 1x1
            *dst1++ = b[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = r[*pY1++];
        }

        dst0 = dst1;
        dst1 += ti.frame_width*3;
    }
}

void TheoraPlayer::DecodeToBGRA(th_ycbcr_buffer& yuv,unsigned char* buf)
{
    uint8 *dst0 = buf;
    uint8 *dst1 = dst0+(ti.frame_width<<2);

    const int pictOffset  = yuv[0].stride*ti.pic_y+ti.pic_x;

    const uint8 *pY0, *pY1, *pU, *pV;

    //yuv.y_height,yuv.y_width是以16点对齐的，所以可能会超过frame_width,frame_height

    for(int y = 0; y < ti.frame_height; y += 2)
    {
        pY0 = yuv[0].data +   pictOffset +  y       * (yuv[0].stride);
        pY1 = yuv[0].data +   pictOffset + (y | 1)  * (yuv[0].stride);
        pU  = yuv[1].data + ((pictOffset +  y       * (yuv[1].stride)) >> 1);
        pV  = yuv[2].data + ((pictOffset +  y       * (yuv[2].stride)) >> 1);

        for(int x = 0; x < yuv[0].width; x += 2)
        {
            const int G = sAdjCrg[*pV] + sAdjCbg[*pU];

            const uint8 *r =YV_R[*pV++];
            const uint8 *b =YU_B[*pU++];

            // pixel 0x0
            *dst0++ = b[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = r[*pY0++];
            dst0++;

            // pixel 1x0
            *dst0++ = b[*pY0];
            *dst0++ = sClamp[sAdjY[*pY0] - G];
            *dst0++ = r[*pY0++];
            dst0++;

            // pixel 0x1
            *dst1++ = b[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = r[*pY1++];
            dst1++;

            // pixel 1x1
            *dst1++ = b[*pY1];
            *dst1++ = sClamp[sAdjY[*pY1] - G];
            *dst1++ = r[*pY1++];
            dst1++;
        }

        dst0 = dst1;
        dst1 += ti.frame_width<<2;
    }
}

double TheoraPlayer::CheckUpdate()
{
    if(!theora_p)
        return(-1);

    currentTime=GetDoubleTime();

    while(true)
    {
        while(ogg_stream_packetout(&to,&op) > 0)
        {
            if(op.granulepos>=0)
            {
                if (op.granulepos == 0)
                {
                    startTime = 0;      //第一帧
                    GetDoubleTime();
                }

                videobuf_time=th_granule_time(tdc,op.granulepos);

                if(videobuf_time<currentTime)       //这一帧的时间已经过了，此包丢弃。
                    continue;                       //注意此举将使画面损坏，但为保证画面流畅所以做如此处理。

                th_decode_ctl(tdc,TH_DECCTL_SET_GRANPOS,&op.granulepos,sizeof(op.granulepos));
            }

            if(th_decode_packetin(tdc,&op,&videobuf_granulepos)==0)
            {
                videobuf_time=th_granule_time(tdc,videobuf_granulepos);

                if(videobuf_time>=currentTime)
                    return videobuf_time-currentTime;
            }
        }

        if (Eof(in))
        {
            if (this->isLoop)
            {
                Rewind();
            }
            else
            {
                this->isPlaying = false;
                return(-1);
            }
        }

        buffer_data(&oy);

        while (ogg_sync_pageout(&oy,&og) > 0)
            queue_page(&og);
    }
}

void TheoraPlayer::DecodeToRGB(unsigned char *buf)
{
    th_decode_ycbcr_out(tdc,yuv);

    DecodeToRGB(yuv,buf);

/*  Fw8u *src[3]={yuv[0].data,yuv[1].data,yuv[2].data};
    int step[3]={yuv[0].stride,yuv[1].stride,yuv[2].stride};
    FwiSize roi={videoWidth,videoHeight};

    fwiYCbCr420ToRGB_8u_P3C3R(src,step,buf,videoWidth*3,roi);*/
}

void TheoraPlayer::DecodeToBGR(unsigned char *buf)
{
    th_decode_ycbcr_out(tdc,yuv);

    DecodeToBGR(yuv,buf);

/*  Fw8u *src[3]={yuv[0].data,yuv[1].data,yuv[2].data};
    int step[3]={yuv[0].stride,yuv[1].stride,yuv[2].stride};
    FwiSize roi={videoWidth,videoHeight};

    fwiYCbCr420ToBGR_8u_P3C3R(src,step,buf,videoWidth*3,roi);*/
}

void TheoraPlayer::DecodeToRGBA(unsigned char *buf)
{
    th_decode_ycbcr_out(tdc,yuv);

    DecodeToRGBA(yuv,buf);
}

void TheoraPlayer::DecodeToBGRA(unsigned char *buf)
{
    th_decode_ycbcr_out(tdc,yuv);

    DecodeToBGRA(yuv,buf);
}

th_img_plane *TheoraPlayer::DecodeToYUV(int &ox,int &oy)
{
    th_decode_ycbcr_out(tdc,yuv);

    ox=ti.pic_x;
    oy=ti.pic_y;

    return(yuv);
}

void TheoraPlayer::Close()
{
    if (this->theoraLoaded)
        this->StopTheora();

    this->isOpen = false;
};
//--------------------------------------------------------------------------------------------------
