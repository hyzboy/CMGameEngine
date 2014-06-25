#include<hgl/audio/AudioManage.h>
#include<hgl/audio/AudioSource.h>
#include<hgl/audio/AudioBuffer.h>
#include<hgl/audio/OpenAL.h>

namespace hgl
{
	AudioManage::AudioItem::AudioItem()
	{
		source=new AudioSource;
		buffer=nullptr;
	}

	AudioManage::AudioItem::~AudioItem()
	{
		source->Unlink();

		delete source;

		if(buffer)
			delete buffer;
	}

	bool AudioManage::AudioItem::Check()
	{
		if(!buffer)return(true);

		if(source->State==AL_PLAYING)return(false);

		source->Unlink();

		delete source;				//这么做的原因是有些声卡上一个音源上播放的数据格式必须一致，否则新格式的数据会发不出来
		source=new AudioSource;

		delete buffer;
		buffer=nullptr;

		return(true);
	}

	void AudioManage::AudioItem::Play(const os_char *filename,float gain)
	{
		if(buffer)
		{	//实质上绝对不可能到这一段
			source->Unlink();
			delete buffer;
		}

		buffer=new AudioBuffer(filename);

		source->Link(buffer);
		source->Gain=gain;
		source->Play(false);
	}
}//namespace hgl

namespace hgl
{
	AudioManage::AudioManage(int count)
	{
		Items.Malloc(count);
	}

	AudioManage::~AudioManage()
	{
	}

	bool AudioManage::Play(const os_char *filename,float gain)
	{
		int n=Items.GetCount();

		while(n--)
		{
			AudioItem *item=Items[n];

			if(item->Check())
			{
				item->Play(filename,gain);
				return(true);
			}
		}

		return(false);
	}
}//namespace hgl
