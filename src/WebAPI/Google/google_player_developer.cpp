#include<hgl/webapi/google/google_player_developer.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace webapi
	{
		namespace google
		{
			int PurchasesProductsGet(const UTF8String &packageName,const UTF8String &productId,const UTF8String &token,const UTF8String &api_key)
			{
				UTF8String url=U8_TEXT("https://www.googleapis.com/androidpublisher/v2/applications/")+packageName+U8_TEXT("/purchases/products/")+productId+U8_TEXT("/tokens/")+token+U8_TEXT("?key=")+api_key;

				static int count=1;

				io::FileOutputStream fos;

				if(!fos.CreateTrunc(OS_TEXT("/home/showhand/log/purchases.products.get.")+OSString(++count)+OS_TEXT(".txt")))
				{
					LOG_ERROR("Create WebAPI Google purchases.products.get file error!");
					//return(-1);
					return(-1);
				}
				
				fos.WriteFully(url.c_str(),url.Length());
				fos.WriteFully("\n",1);

				network::http::get(&fos,url);

				return 1;
			}
		}//namespace google
	}//namespace webapi
}//namespace hgl
