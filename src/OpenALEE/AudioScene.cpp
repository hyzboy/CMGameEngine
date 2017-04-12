#include<hgl/audio/AudioScene.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/audio/Listener.h>
#include<hgl/al/al.h>
#include<hgl/Time.h>
namespace hgl
{
    /**
     * 計算指定音源相對收聽者的音量
     */
    const double GetGain(AudioListener *l,AudioSourceItem *s)
    {
        if(!l||!s)return(0);

        if(s->gain<=0)return(0);        //本身音量就是0

        float distance;

        const Vector3f &        lpos=l->GetPosition();

        distance=length(lpos,s->cur_pos);

        if(s->distance_model==AL_INVERSE_DISTANCE_CLAMPED||s->distance_model==AL_INVERSE_DISTANCE)
        {
            if(s->distance_model==AL_INVERSE_DISTANCE_CLAMPED)
                if(distance<=s->ref_distance)
                    return 1;

            distance = hgl_max(distance,s->ref_distance);
            distance = hgl_min(distance,s->max_distance);

            return s->ref_distance/(s->ref_distance+s->rolloff_factor*(distance-s->ref_distance));
        }
        else
        if(s->distance_model==AL_LINEAR_DISTANCE_CLAMPED)
        {
            distance = hgl_max(distance,s->ref_distance);
            distance = hgl_min(distance,s->max_distance);

            return (1-s->rolloff_factor*(distance-s->ref_distance)/(s->max_distance-s->ref_distance));
        }
        else
        if(s->distance_model==AL_LINEAR_DISTANCE)
        {
            distance = hgl_min(distance,s->max_distance);
            return (1-s->rolloff_factor*(distance-s->ref_distance)/(s->max_distance-s->ref_distance));
        }
        else
        if(s->distance_model==AL_EXPONENT_DISTANCE)
        {
            return pow(distance/s->ref_distance,-s->rolloff_factor);
        }
        else
        if(s->distance_model==AL_EXPONENT_DISTANCE_CLAMPED)
        {
            distance = hgl_max(distance,s->ref_distance);
            distance = hgl_min(distance,s->max_distance);

            return pow(distance/s->ref_distance,-s->rolloff_factor);
        }
        else
            return 1;
    }

    /**
     * 音频场景管理类构造函数
     * @param max_source 最大音源数量
     * @param al 收的者
     */
    AudioScene::AudioScene(int max_source,AudioListener *al)
    {
        source_pool.PreMalloc(max_source);

        listener=al;

        ref_distance=1000;
        max_distance=100000;
    }

    AudioSourceItem *AudioScene::Create(AudioBuffer *buf,const Vector3f &pos,const float &gain)
    {
        if(!buf)
            return(nullptr);

        AudioSourceItem *asi=new AudioSourceItem;

        asi->buffer=buf;

        asi->loop=false;

        asi->gain=gain;

        asi->distance_model=AL_INVERSE_DISTANCE_CLAMPED;
        asi->rolloff_factor=1;
        asi->ref_distance=ref_distance;
        asi->max_distance=max_distance;

        asi->start_play_time=0;
        asi->is_play=false;

        asi->last_pos=pos;
        asi->cur_pos=pos;

        asi->last_time=asi->cur_time=0;
        asi->move_speed=0;

        asi->last_gain=0;

        asi->source=nullptr;

        return asi;
    }

    void AudioScene::Delete(AudioSourceItem *asi)
    {
        if(!asi)return;

        ToMute(asi);

        source_list.Delete(asi);
    }

    bool AudioScene::ToMute(AudioSourceItem *asi)
    {
        if(!asi)return(false);
        if(!asi->source)return(false);

        OnToMute(asi);

        asi->source->Stop();
        asi->source->Unlink();
        source_pool.Release(asi->source);

        asi->source=nullptr;

        return(true);
    }

    bool AudioScene::ToHear(AudioSourceItem *asi)
    {
        if(!asi)return(false);
        if(!asi->buffer)return(false);

        if(asi->start_play_time>cur_time)       //还没到起播时间
            return(false);

        double time_off=0;

        if(asi->start_play_time>0
         &&asi->start_play_time<cur_time)
        {
            time_off=cur_time-asi->start_play_time;

            if(time_off>=asi->buffer->Time)     //大于整个音频的时间
            {
                if(!asi->loop)                  //无需循环
                {
                    asi->is_play=false;         //不用放了
                    return(false);
                }
                else                            //循环播放的
                {
                    const int count=int(time_off/asi->buffer->Time);        //计算超了几次并取整

                    time_off-=asi->buffer->Time*count;                      //计算单次的偏移时间
                }
            }
        }

        if(!asi->source)
        {
            asi->source=source_pool.Acquire();

            if(!asi->source)return(false);
        }

        asi->source->Link(asi->buffer);

        asi->source->Gain=asi->gain;
        asi->source->DistanceModel=asi->distance_model;
        asi->source->RolloffFactor=asi->rolloff_factor;
        asi->source->SetDistance(asi->ref_distance,asi->max_distance);
        asi->source->SetPosition(asi->cur_pos);
        asi->source->SetConeAngle(asi->cone_angle);
        asi->source->SetVelocity(asi->velocity);
        asi->source->SetDirection(asi->direction);
        asi->source->SetDopplerFactor(asi->doppler_factor);
        asi->source->SetDopplerVelocity(0);

        asi->source->CurTime=time_off;
        asi->source->Play(asi->loop);

        OnToHear(asi);

        return(true);
    }

    bool AudioScene::UpdateSource(AudioSourceItem *asi)
    {
        if(!asi)return(false);
        if(!asi->source)return(false);

        if(asi->source->State==AL_STOPPED)    //停播状态
        {
            if(!asi->loop)                  //不是循环播放
            {
                if(OnStopped(asi))
                    ToMute(asi);

                return(true);
            }
            else
            {
                asi->source->Play();        //继续播放
            }
        }

        if(asi->doppler_factor>0)                   //需要多普勒效果
        {
//             double shift=0;

            if(asi->last_pos!=asi->cur_pos)         //坐标不一样了
            {
                asi->move_speed=length(asi->last_pos,asi->cur_pos)/(asi->cur_time-asi->last_time);
                //根据离收听者的距离和速度进行多普勒调整

//                 shift = DOPPLER_FACTOR * freq * (DOPPLER_VELOCITY - l.velocity) / (DOPPLER_VELOCITY + s.velocity)

                asi->source->SetDopplerVelocity(asi->move_speed);       //由于计算未理清，暂用move_speed代替
            }

            if(cur_time>asi->cur_time)          //更新时间和坐标
            {
                asi->last_pos=asi->cur_pos;
                asi->last_time=asi->cur_time;
            }
        }

        return(true);
    }

    /**
     * 刷新處理
     * @param ct 当前时间
     * @return 收聽者仍可聽到的音源數量
     * @return -1 出錯
     */
    int AudioScene::Update(const double &ct)
    {
        if(!listener)return(-1);

        const int count=source_list.GetCount();

        if(count<=0)return 0;

        if(ct!=0)
            cur_time=ct;
        else
            cur_time=GetDoubleTime();

        float new_gain;
        int hear_count=0;

        AudioSourceItem **ptr=source_list.GetData();

        for(int i=0;i<count;i++)
        {
            if(!(*ptr)->is_play)
            {
                if((*ptr)->source)          //还有音源
                    ToMute(*ptr);

                continue;   //不需要放的音源
            }

            new_gain=OnCheckGain(*ptr);

            if(new_gain<=0)                 //听不到声音了
            {
                if((*ptr)->last_gain>0)     //之前可以听到
                    ToMute(*ptr);
                else
                    OnContinuedMute(*ptr);  //之前就听不到
            }
            else
            {
                if((*ptr)->last_gain<=0)
                {
                    if(!ToHear(*ptr))       //之前没声
                        new_gain=0;         //没有足够可用音源、或是已经放完了，所以还是听不到
                }
                else
                {
                    UpdateSource(*ptr);     //刷新音源处理
                    OnContinuedHear(*ptr);  //之前就有声音
                }
            }

            (*ptr)->last_gain=new_gain;

            if(new_gain>0)
                ++hear_count;

            ++ptr;
        }

        return hear_count;
    }
}//namespace hgl
