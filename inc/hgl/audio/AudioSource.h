#ifndef HGL_AUDIO_SOURCE_INCLUDE
#define HGL_AUDIO_SOURCE_INCLUDE

#include<hgl/audio/ConeAngle.h>
#include<hgl/audio/AudioBuffer.h>

namespace hgl
{
    class AudioListener;

	/**
	* 音频源，指的是一个发声源，要发声必须创建至少一个发声源。而这个类就是管理发声源所用的。
	*/
	class AudioSource                                                                       ///音频源类
	{
		#include<hgl/audio/AudioSource.Attrib.h>

	public: //属性

		Property<uint>		Index;

		Property<double>	CurTime;														///<当前播放到的时间

		Property<int>		State;                                                          ///<音源状态
		Property<float>		MinGain;                                                        ///<最小增益
		Property<float>		MaxGain;                                                        ///<最大增益

		Property<bool>		Loop;                                                           ///<是否循环播放虚拟变量

		Property<float>		Pitch;                                                          ///<播放频率
		Property<float>		Gain;                                                           ///<音量增益幅度
		Property<float>		ConeGain;                                                       ///<音源锥增益

		Property<uint>		DistanceModel;													///<距离衰减模型(默认钳位倒数距离模型)
		Property<float>		RolloffFactor;                                                  ///<音源衰减因(>=0,默认1.0)

		Property<float>		DopplerFactor;													///<多普勒因子(默认1.0)
		Property<float>		SpeedOfSound;													///<声音传播速度(单位为米，默认值343.3，最小精度0.0001米)

	public:	//属性方法

		const Vector3f &GetPosition(){return position;}
		const Vector3f &GetVelocity(){return velocity;}
		const Vector3f &GetDirection(){return direction;}

		const void GetDistance(float &ref_distance,float &max_distance)const;
		const ConeAngle &GetAngle(){return angle;}

		void SetPosition(const Vector3f &);
		void SetVelocity(const Vector3f &);
		void SetDirection(const Vector3f &);
		void SetDistance(const float &ref_distance,const float &max_distance);
		void SetAngle(const ConeAngle &);

	public: //方法

		AudioSource(bool=false);                                                                    ///<本类构造函数
		AudioSource(AudioBuffer *);																	///<本类构造函数
		virtual ~AudioSource();                                                                     ///<本类析构函数

		virtual bool Play(bool=false);                                                              ///<播放当前音源
		virtual void Pause();                                                                       ///<暂停播放
		virtual void Resume();																		///<继续播放
		virtual void Stop();                                                                        ///<停止播放
		virtual void Rewind();                                                                      ///<重绕播放

		virtual bool Create();                                                                      ///<创建音源
		virtual void Close();                                                                       ///<关闭音源

		bool Link(AudioBuffer *);                                                                   ///<绑定一个音频缓冲区
		void Unlink();                                                                              ///<解除绑定
	};//class AudioSource
}//namespace hgl
#endif//HGL_AUDIO_SOURCE_INCLUDE
