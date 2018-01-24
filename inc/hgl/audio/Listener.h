#ifndef HGL_LISTENER_INCLUDE
#define HGL_LISTENER_INCLUDE

#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    /**
     * 收听者方向,用值请参考gluLookAt
     */
    struct ListenerOrientation
    {
        Vector3f direction;
        Vector3f rotation;
    };

    /**
    * 这个类用于管理收听者的一些状态，可以通过它设定统一的音量，收听者的位置、方向等
    */
    class AudioListener                                                                             ///收听者管理类
    {
    protected:

        float gain;

        Vector3f position;
        Vector3f velocity;
        ListenerOrientation orientation;

    public: //属性方法

        const float                 GetGain         ()const{return gain;}                           ///<读取音量增益幅度
        const Vector3f &            GetPosition     ()const{return position;}                       ///<读取音源坐标位置
        const Vector3f &            GetVelocity     ()const{return velocity;}                       ///<读取速率
        const ListenerOrientation & GetOrientation  ()const{return orientation;}                    ///<读取导向

        void SetGain(float);                                                                        ///<设置音量增益幅度
        void SetPosition(const Vector3f &);                                                         ///<设置音源坐标位置
        void SetVelocity(const Vector3f &);                                                         ///<设置速率
        void SetOrientation(const ListenerOrientation &);                                           ///<设置导向

    public: //方法

        AudioListener();
        ~AudioListener();
    };//class AudioListener
}//namespace hgl
#endif//HGL_LISTENER_INCLUDE
