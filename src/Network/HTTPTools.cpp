#include<hgl/network/HTTPTools.h>
#include<hgl/LogInfo.h>
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

					int ss=size*number;

					os->WriteFully(ptr,ss);

					return ss;
				}
			}

			/**
			 * 执行http get指令
			 * @param os 执行后下载到的数据输出流
			 * @param url 要执行的网址
			 * @return 下载下来的数据长度
			 * @return <0 出错
			 */
			int get(io::OutputStream *os,const char *url)
			{
				LOG_INFO(U8_TEXT("http get,url:")+UTF8String(url));

				CURLcode res;

				CURL *curl=curl_easy_init();

				if(!curl)
					return(-1);

				int cur=os->Tell();

				curl_easy_setopt(curl,CURLOPT_URL,url);
				curl_easy_setopt(curl,CURLOPT_WRITEDATA,os);
				curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,http_get_to_output_stream);
				curl_easy_perform(curl);
				curl_easy_cleanup(curl);

				return(os->Tell()-cur);
			}

			int post(io::OutputStream *os,const char *url,const void *post_data,const int post_data_size)
			{
				CURLcode res;

				CURL *curl=curl_easy_init();

				if(!curl)
					return(-1);

				int cur=os->Tell();

				curl_easy_setopt(curl,CURLOPT_URL,url);
				curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post_data);
				curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,post_data_size);
				curl_easy_setopt(curl,CURLOPT_WRITEDATA,os);
				curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,http_get_to_output_stream);
				curl_easy_perform(curl);
				curl_easy_cleanup(curl);

				return(os->Tell()-cur);
			}
		}//namespace http
	}//namespace network
}//namespace hgl
