#ifndef HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE
#define HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE

#include<hgl/type/BaseString.h>

namespace hgl
{
	namespace webapi
	{
		namespace google
		{
			int PurchasesProductsGet(const UTF8String &packageName,const UTF8String &productId,const UTF8String &token);
		}//namespace google
	}//namespace webapi
}//namespace hgl
#endif//HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE

