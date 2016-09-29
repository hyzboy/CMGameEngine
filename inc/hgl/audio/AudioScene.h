#ifndef HGL_AUDIO_SCENE_INCLUDE
#define HGL_AUDIO_SCENE_INCLUDE

#include<hgl/VectorMath.h>
#include<hgl/type/Pool.h>
#include<hgl/type/Map.h>
#include<hgl/audio/ConeAngle.h>
namespace hgl
{
    class AudioBuffer;
    class AudioSource;
    class AudioListener;

    /**
     * 邏輯發聲源
     */
    struct AudioSourceItem
    {
		friend class AudioScene;

	private:

        AudioBuffer *buffer;

	public:

        bool loop;
//         float min_gain,max_gain;
        float gain;

        uint distance_model;
        float rolloff_factor;
//         float doppler_factor;
//         float speed_of_sound;

        float ref_distance;
        float max_distance;
//         ConeAngle cone_angle;
//
//         Vector3f Velocity;
//         Vector3f Direction;

        Vector3f cur_pos;
        double cur_time;

	private:

		Vector3f last_pos;
		double last_time;
		
		double move_speed;

        double last_gain;                                                                           ///<最近一次的音量

        AudioSource *source;
    };//struct AudioSourceItem

    const double GetGain(AudioListener *,AudioSourceItem *);                                        ///<取得相對音量

    /**
     * 音频场景管理
     */
    class AudioScene                                                                                ///<音频场景管理
    {
    protected:

        AudioListener *listener;                                                                    ///<收聽者

        ObjectPool<AudioSource> source_pool;                                                        ///<音源数据池
        Set<AudioSourceItem *> source_list;                                                         ///<音源列表

    protected:

        bool ToMute(AudioSourceItem *);                                                             ///<转静默处理
        bool ToHear(AudioSourceItem *);                                                             ///<转发声处理

        bool UpdateSource(AudioSourceItem *);                                                       ///<刷新音源处理

    public:     //事件

        virtual float   OnCheckGain(AudioSourceItem *asi)                                           ///<檢測音量事件
        {
            return asi?GetGain(listener,asi)*asi->gain:0;
        }

        virtual void    OnToMute(AudioSourceItem *){/*無任何處理，請自行重載處理*/}                 ///<从有声变成聽不到聲音
        virtual void    OnToHear(AudioSourceItem *){/*無任何處理，請自行重載處理*/}                 ///<从听不到声变成能听到声音

        virtual void    OnContinuedMute(AudioSourceItem *){/*無任何處理，請自行重載處理*/}          ///<持续聽不到聲音
        virtual void    OnContinuedHear(AudioSourceItem *){/*無任何處理，請自行重載處理*/}          ///<持续可以聽到聲音

		virtual bool	OnStopped(AudioSourceItem *){return true;}									///<单个音源播放结束事件,返回TRUE表示可以释放这个音源，返回FALSE依然占用这个音源

    public:

        AudioScene(int max_source,AudioListener *al);                                               ///<构造函数(指定最大音源数)
        virtual ~AudioScene();                                                                      ///<析构函数

                void                SetListener(AudioListener *al){listener=al;}                    ///<設置收聽者

        virtual AudioSourceItem *   Create(AudioBuffer *,const Vector3f &pos,const float &gain=1);  ///<创建一個音源
        virtual void                Delete(AudioSourceItem *);                                      ///<删除一个音源

		virtual void				Clear()															///<清除所有音源
		{
			source_list.Clear();
			source_pool.ReleaseAll();
		}

        virtual int                 Update();                                                       ///<刷新,返回仍在發聲的音源數量
    };//class AudioScene
}//namespace hgl
#endif//HGL_AUDIO_SCENE_INCLUDE
