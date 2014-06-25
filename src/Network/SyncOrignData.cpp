#include<hgl/network/Sync.h>
namespace hgl
{
	namespace network
	{
		SyncOrignData::SyncOrignData()
		{
			data=nullptr;
			size=0;
		}

		SyncOrignData::~SyncOrignData()
		{
			SAFE_CLEAR_ARRAY(data);
		}
	}//namespace network
}//namespace hgl
