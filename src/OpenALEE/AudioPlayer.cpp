#include<hgl/audio/AudioPlayer.h>
#include<hgl/LogInfo.h>
#include<hgl/PlugIn.h>
#include<hgl/Time.h>
#include<hgl/io/MemoryInputStream.h>
#include"AudioDecode.h"

namespace hgl
{
    const os_char *GetAudioDecodeName(const AudioFileType aft);

    void AudioPlayer::InitPrivate()
    {
        if(!alGenSources)
        {
            LOG_ERROR(OS_TEXT("OpenAL/EE 还未初始化!"));
            return;
        }

        auto_gain.open=false;

        audio_ptr=nullptr;

        audio_data=nullptr;
        audio_data_size=0;

        audio_buffer=nullptr;
        audio_buffer_size=0;

        decode=nullptr;

        ps=psNone;

        {
            hglSetPropertyRead(	Index,          this,AudioPlayer::GetIndex		);
            hglSetPropertyRead( Time,           this,AudioPlayer::GetTime		);

            hglSetPropertyRead(	State,          this,AudioPlayer::GetPlayState	);
            hglSetPropertyRead( SourceState,    this,AudioPlayer::GetSourceState);
            hglSetPropertyRead( MinGain,        this,AudioPlayer::GetMinGain	);
            hglSetPropertyRead( MaxGain,        this,AudioPlayer::GetMaxGain	);

            hglSetProperty(     Loop,           this,AudioPlayer::GetLoop,          AudioPlayer::SetLoop			);

            hglSetProperty(     Pitch,          this,AudioPlayer::GetPitch,         AudioPlayer::SetPitch			);
            hglSetProperty(     Gain,           this,AudioPlayer::GetGain,          AudioPlayer::SetGain			);
            hglSetProperty(     ConeGain,       this,AudioPlayer::GetConeGain,      AudioPlayer::SetConeGain		);

            hglSetProperty(     RolloffFactor,	this,AudioPlayer::GetRolloffFactor,	AudioPlayer::SetRolloffFactor   );
        }

        if(!audiosource.Create())return;

        audiosource.Loop=false;

        source=audiosource.index;

        alGenBuffers(3,buffer);
    }

    AudioPlayer::AudioPlayer()
    {
        InitPrivate();
    }

    AudioPlayer::AudioPlayer(const os_char *filename,AudioFileType aft)
    {
        InitPrivate();

        if(filename)
            Load(filename,aft);
    }

    AudioPlayer::AudioPlayer(InputStream *stream,int size,AudioFileType aft)
    {
        InitPrivate();

        Load(stream,size,aft);
    }

//     AudioPlayer::AudioPlayer(HAC *hac,const os_char *filename,AudioFileType aft)
//     {
//         InitPrivate();
//
//         Load(hac,filename,aft);
//     }

    AudioPlayer::~AudioPlayer()
    {
        if(!audio_data)return;

        Clear();

        alDeleteBuffers(3,buffer);

        SAFE_CLEAR_ARRAY(audio_buffer);
    }

    bool AudioPlayer::Load(AudioFileType aft)
    {
        const os_char *plugin_name=GetAudioDecodeName(aft);

        if(!plugin_name)return(false);

        decode=AudioInterfaceCheck(plugin_name);

        if(decode)
        {
            audio_ptr=decode->Open(audio_data,audio_data_size,&format,&rate,&time);

            audio_buffer_size=(AudioTime(format,rate)+9)/10;        // 1/10 秒

            if(audio_buffer)
                delete[] audio_buffer;

            audio_buffer=new char[audio_buffer_size];

            wait_time=0.1;

            if(wait_time>time/3.0f)
                wait_time=time/10.0f;

            return(true);
        }
        else return(false);
    }

    /**
    * 从流中加载音频数据,仅支持OGG
    * @param stream 要加载数据的流
    * @param size 音频数据长度
    * @param aft 音频文件类型
    * @return 是否加载成功
    */
    bool AudioPlayer::Load(InputStream *stream,int size,AudioFileType aft)
    {
        if(!alGenBuffers)return(false);
        if(!stream)return(false);
        if(size<=0)return(false);

        Clear();
        
        if(aft<=aftNone||aft>=aftEnd)
        {
            LOG_ERROR(OS_TEXT("未支持的音频文件类型！AudioFileType: ")+OSString(aft));

            return(false);
        }
        else
        {
            audio_data=new ALbyte[size];

            stream->Read(audio_data,size);

            audio_data_size=size;
            return Load(aft);
        }
    }

    /**
    * 从文件中加载一段音频数据
    * @param filename 音频文件名称
    * @param aft 音频文件类型
    * @return 是否加载成功
    */
    bool AudioPlayer::Load(const os_char *filename,AudioFileType aft)
    {
        if(!alGenBuffers)return(false);
        if(!filename||!(*filename))return(false);

        if(aft<=aftNone||aft>=aftEnd)
            aft=CheckAudioFileType(filename);

        if(aft<=aftNone||aft>=aftEnd)
        {
            LOG_ERROR(OS_TEXT("未知的音频文件类型！AudioFile: ")+OSString(filename));
            return(false);
        }

        OpenFileInputStream fis(filename);

        return(Load(fis,fis->Available(),aft));
    }

//     /**
//     * 从文件中加载一段音频数据
//     * @param hac HAC包指针
//     * @param filename 音频文件名称
//     * @param aft 音频文件类型
//     * @return 是否加载成功
//     */
//     bool AudioPlayer::Load(HAC *hac,const os_char *filename,AudioFileType aft)
//     {
//         if(!alGenBuffers)return(false);
//
//         os_char *ext;
//         Stream *stream;
//         bool result;
//
//         ext=strrchr(filename,u'.');
//
//         LowerString(ext);
//
//         if(aft<=aftNone||aft>=aftEnd)
//         {
//             if(strcmp(ext,u".ogg")==0)aft=aftOGG;else
//             {
//                 PutError(u"未支持的音频文件类型！AudioFileType:%d",aft);
//                 return(false);
//             }
//         }
//
//         stream=hac->LoadFile(filename);
//
//         if(stream)
//         {
//             result=Load(stream,aft);
//
//             delete stream;
//
//             return(result);
//         }
//
//         return(false);
//     }

    void AudioPlayer::Clear()
    {
        Stop();

        if(decode&&audio_ptr)
            decode->Close(audio_ptr);

        SAFE_CLEAR_ARRAY(audio_data);
        SAFE_CLEAR_ARRAY(audio_buffer);

        audio_ptr=nullptr;

        time=0;
    }

    bool AudioPlayer::GetLoop()
    {
        lock.Lock();
        bool rv=loop;
        lock.Unlock();

        return(rv);
    }

    void AudioPlayer::SetLoop(bool val)
    {
        lock.Lock();
        loop=val;
        lock.Unlock();
    }

    bool AudioPlayer::ReadData(ALuint n)
    {
        uint size;

        size=decode->Read(audio_ptr,audio_buffer,audio_buffer_size);

        if(size)
        {
            alBufferData(n,format,audio_buffer,size,rate);

            if(alLastError())return(false);

            return(true);
        }

        return(false);
    }

    bool AudioPlayer::Playback()
    {
        if(!audio_data)return(false);

        alSourceStop(source);
        ClearBuffer();
        decode->Restart(audio_ptr);

        int count=0;

        audio_buffer_count=0;

        if(ReadData(buffer[0]))
        {
            count++;

            if(ReadData(buffer[1]))                //以免有些音效太短，在这里直接失败
                count++;

            if(ReadData(buffer[2]))                //以免有些音效太短，在这里直接失败
                count++;

            alSourceQueueBuffers(source,count,buffer);
            alSourcePlay(source);
            start_time=GetDoubleTime();

            ps=psPlay;
            return(true);
        }
        else
        {
            ps=psExit;

            return(false);
        }
    }

    /**
    * 开始播放
    * @param _loop 是否循环播放
    */
    void AudioPlayer::Play(bool _loop)
    {
        if(!audio_data)return;

        lock.Lock();

        loop=_loop;

        if(ps==psNone||ps==psPause)      //未启动线程
            Start();

        Playback();            //Execute执行有检测Lock，所以不必担心该操作会引起线程冲突

        lock.Unlock();
    }

    /**
    * 停止播放
    */
    void AudioPlayer::Stop()
    {
        if(!audio_data)return;

        bool thread_is_live=true;

        lock.Lock();

        if(Thread::IsLive())
            ps=psExit;
        else
            thread_is_live=false;

        lock.Unlock();

        if(thread_is_live)
            Thread::Wait();

        ps=psNone;
    }

    /**
    * 暂停播放
    */
    void AudioPlayer::Pause()
    {
        if(!audio_data)return;

        lock.Lock();

        if(ps==psPlay)
            ps=psPause;

        lock.Unlock();
    }

    /**
    * 继续播放
    */
    void AudioPlayer::Resume()
    {
        if(!audio_data)return;

        lock.Lock();

        if(ps==psPause)
        {
            ps=psPlay;

            Thread::Start();
        }

        lock.Unlock();
    }

    bool AudioPlayer::UpdateBuffer()
    {
        int processed=0;
        bool active=true;

        alGetSourcei(source,AL_BUFFERS_PROCESSED,&processed);        //取得处理结束的缓冲区数量

        if(processed<=0)return(true);

        const double cur_time=GetDoubleTime();

        if(cur_time-start_time<fade_in_time)        //淡入时间
        {
            audiosource.SetGain(((cur_time-start_time)/fade_in_time)*gain);
        }
        else
        if(cur_time-start_time>time-fade_out_time)    //淡出时间
        {
            audiosource.SetGain(((time-(cur_time-start_time))/fade_out_time)*gain);
        }

        if(auto_gain.open)
        {
            if(cur_time>=auto_gain.end.time)
            {
                auto_gain.open=false;

                Gain=auto_gain.end.gain;

                if(auto_gain.end.gain<=0)
                    ps=psExit;
            }
            else
            {
                Gain=auto_gain.start.gain+auto_gain.gap*((cur_time-auto_gain.start.time)/auto_gain.time);
            }
        }

        while(processed--)
        {
            ALuint buffer;

            audio_buffer_count+=audio_buffer_size;

            alSourceUnqueueBuffers(source,1,&buffer);       //解除一个已处理完成的缓冲区
            alLastError();

            active=ReadData(buffer);                        //解码数据到这个缓冲区

            if(active)
            {
                alSourceQueueBuffers(source,1,&buffer);     //重新将这个缓冲区加入队列
                alLastError();
            }
            else
                return(false);
        }

        return(true);
    }

    void AudioPlayer::ClearBuffer()
    {
        int queued;
        ALuint buffer;

        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

        while(queued--)
            alSourceUnqueueBuffers(source, 1, &buffer);
    }

    bool AudioPlayer::Execute()
    {
        if(!audio_data)return(false);

        while(true)
        {
            lock.Lock();

            if(ps==psPlay)    //被要求播放
            {
                if(!UpdateBuffer())
                {
                    if(loop)        //被要求循环播放
                    {
                        if(SourceState!=AL_STOPPED)               //等它放完
                            Playback();
                    }
                    else
                    {
                        //退出
                        lock.Unlock();

                        ps=psNone;
                        return(false);
                    }
                }
                else
                {
                    if(SourceState!=AL_PLAYING)
                        alSourcePlay(source);
                }
            }
            else
            if(ps==psPause)        //被要求暂停
            {
                alSourcePause(source);

                lock.Unlock();
                return(false);
            }
            else
            if(ps==psExit)      //被要求暂停或退出
            {
                alSourceStop(source);
                alSourcei(source,AL_BUFFER,0);
                ClearBuffer();

                lock.Unlock();
                return(false);
            }

            lock.Unlock();

            WaitTime(wait_time);      //以让线程空出CPU时间片
        }
    }

    double AudioPlayer::GetPlayTime()
    {
        if(!audio_data)return(0);

        uint base=audio_buffer_count;
        int off;

        lock.Lock();

        alGetSourcei(source,AL_BYTE_OFFSET,&off);

        lock.Unlock();

        return AudioDataTime(base+off,format,rate);
    }

    /**
    * 自动调整增益
    */
    void AudioPlayer::AutoGain(float target_gain,double adjust_time)
    {
        if(!audio_data)return;

        lock.Lock();
            auto_gain.start.gain=Gain;
            auto_gain.start.time=GetDoubleTime();
            auto_gain.end.gain=target_gain;
            auto_gain.end.time=auto_gain.start.time+adjust_time;

            auto_gain.time=adjust_time;
            auto_gain.gap=target_gain-auto_gain.start.gain;

            auto_gain.open=true;
        lock.Unlock();
    }

    void AudioPlayer::SetFadeTime(double in,double out)
    {
        fade_in_time=in;
        fade_out_time=out;
    }
}//namespace hgl
