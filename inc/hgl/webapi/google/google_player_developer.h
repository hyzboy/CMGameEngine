#ifndef HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE
#define HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE

#include<hgl/type/BaseString.h>

namespace hgl
{
	namespace webapi
	{
		namespace google
		{
			/**
			 * 执行一个购买信息获取
			 * @param packageName 软件包的名称，如“com.hyzgame.wing_of_wind"
			 * @param productId 购买的物品ID，如“sku_money”
			 * @param token Google返回的结帐标识
			 * @param api_key 你的API_KEY
			 * @return >0 购买成功
			 * @return <0 失败错误码
			 */
			int PurchasesProductsGet(const UTF8String &packageName,const UTF8String &productId,const UTF8String &token,const UTF8String &api_key);
		}//namespace google
	}//namespace webapi
}//namespace hgl
#endif//HGL_WEBAPI_GOOGLE_PLAYER_DEVELOPER_API_INCLUDE

