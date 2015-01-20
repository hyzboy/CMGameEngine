#ifndef HGL_AUDIO_PLAYER_INCLUDE
#define HGL_AUDIO_PLAYER_INCLUDE

// #include<hgl/HAC.H>
#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/audio/OpenAL.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/VectorMath.h>
using namespace openal;
namespace hgl
{
	namespace io
	{
		class InputStream;
	}//namespace io

	struct AudioPlugInInterface;

    /**
    * 使用AudioPlayer创建的音频播放器类，一般用于背景音乐等独占的音频处理。
    * 这个音频播放器将使用一个单独的线程，在播放器被删除时线程也会被关闭。
    */
    class AudioPlayer:public Thread                                                                 ///音频播放器基类
	{
		ThreadMutex lock;

	public:

		enum PlayState        //播放器状态
		{
			psNone=0,
			psPlay,
			psPause,
			psExit
		};

        #include<hgl/audio/AudioPlayer.Attrib.h>

	public: //属性

		Property<uint>		Index;                                                          		///<音源索引
		Property<double>	Time;                                                           		///<音频总时间

		Property<PlayState>	State;																	///<播放器状态
		Property<int>		SourceState;                                                       		///<音源状态
		Property<float>		MinGain;                                                        		///<最小增益
		Property<float>		MaxGain;                                                        		///<最大增益

		Property<bool>		Loop;                                                           		///<是否循环播放虚拟变量

		Property<float>		Pitch;                                                          		///<播放频率
		Property<float>		Gain;                                                           		///<音量增益幅度
		Property<float>		ConeGain;                                                       		///<

		Property<float>		RolloffFactor;                                                  		///<

	public:	//属性方法

		const Vector3f &		GetPosition(){return audiosource.position;} const
		void            		SetPosition(const Vector3f &pos){audiosource.SetPosition(pos);}

		const Vector3f &		GetVelocity(){return audiosource.velocity;} const
		void            		SetVelocity(const Vector3f &vel){audiosource.SetVelocity(vel);}

		const Vector3f &		GetDirection(){return audiosource.direction;} const
		void            		SetDirection(const Vector3f &dir){audiosource.SetDirection(dir);}

		const ReferenceValue & 	GetDistance(){return audiosource.distance;} const
		void            		SetDistance(const ReferenceValue &rv){audiosource.SetDistance(rv);}

		const ConeAngle &      	GetAngle(){return audiosource.angle;} const
		void            		SetAngle(const ConeAngle &ca){audiosource.SetAngle(ca);}

	public:	//方法

		AudioPlayer();
		AudioPlayer(io::InputStream *,int,AudioFileType);
		AudioPlayer(const os_char *,AudioFileType=aftNone);
// 		AudioPlayer(HAC *,const os_char *,AudioFileType=aftNone);
        virtual ~AudioPlayer();

		virtual bool Load(io::InputStream *,int,AudioFileType);										///<从流中加载一个音频文件
		virtual bool Load(const os_char *,AudioFileType=aftNone);									///<加载一个音频文件
// 		virtual bool Load(HAC *,const os_char *,AudioFileType=aftNone);								///<从HAC包中加载一个音频文件

        virtual void Play(bool=true);                                                               ///<播放音频
        virtual void Stop();                                                                        ///<停止播放
        virtual void Pause();                                                                       ///<暂停播放
        virtual void Resume();                                                                      ///<继续播放
		virtual void Clear();                                                                       ///<清除音频数据

		virtual double GetPlayTime();																///<取得已播放时间(单位秒)
		virtual void SetFadeTime(double,double);													///<设置淡入淡出时间

		virtual void AutoGain(float,double);														///<自动音量
	};//class AudioPlayer
}//namespace hgl
#endif//HGL_AUDIO_PLAYER_INCLUDE
