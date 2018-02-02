#ifndef HGL_AUDIO_MANAGE_INCLUDE
#define HGL_AUDIO_MANAGE_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
    class AudioSource;
    class AudioBuffer;

    /**
    * 简单的音频播放管理，为一般应用的简单操作工具
    */
    class AudioManage
    {
        struct AudioItem
        {
            AudioSource *source;
            AudioBuffer *buffer;

            AudioItem();
            ~AudioItem();

            bool Check();
            void Play(const os_char *,float);
        };//struct AudioItem

        ObjectList<AudioItem> Items;

    public:

        /**
        * 本类构造函数
        * @param count 创建的音源数量,默认为8
        */
        AudioManage(int count=8);
        ~AudioManage();

        /**
        * 播放一个音效
        * @param filename 文件名
        * @param gain 音量，默认为1
        */
        bool Play(const os_char *filename,float gain=1);
    };//class AudioManage
}//namespace hgl
#endif//HGL_AUDIO_MANAGE_INCLUDEE
