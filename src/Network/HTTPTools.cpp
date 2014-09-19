#include<hgl/network/HTTPTools.h>
#include<curl/curl.h>

namespace hgl
{
	namespace network
	{
		namespace http
		{
			namespace
			{
				size_t http_get_to_output_stream(void *ptr,size_t size,size_t number,void *stream)
				{
					io::OutputStream *os=(io::OutputStream *)stream;

					os->WriteFully(ptr,size);
				}
			}

			int get(io::OutputStream *os,const char *url)
			{
				CURLcode res;

				CURL *curl=curl_easy_init();

				if(!curl)
					return(-1);

				auto cur=os->Tell();

				curl_easy_setopt(curl,CURLOPT_URL,url);
				curl_easy_setopt(curl,CURLOPT_WRITEDATA,os);
				curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,http_get_to_output_stream);
				curl_easy_perform(curl);
				curl_easy_cleanup(curl);

				return(os->Tell()-cur);
			}
		}//namespace http
	}//namespace network
}//namespace hgl
