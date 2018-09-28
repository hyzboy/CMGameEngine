#include<hgl/network/HTTPInputStream.h>
#include<hgl/network/TCPClient.h>
#include<hgl/LogInfo.h>
#include<hgl/type/Smart.h>

namespace hgl
{
	namespace network
	{
        namespace
        {
            constexpr char HTTP_REQUEST_HEADER_BEGIN[]= " HTTP/1.1\n"
                                                        "Host: ";
            constexpr uint HTTP_REQUEST_HEADER_BEGIN_SIZE=sizeof(HTTP_REQUEST_HEADER_BEGIN)-1;

            constexpr char HTTP_REQUEST_HEADER_END[]=   "\n"
                                                        "Accept: */*\n"
                                                        "User-Agent: Mozilla/5.0\n"
                                                        "Connection: Keep-Alive\n\n";

            constexpr uint HTTP_REQUEST_HEADER_END_SIZE=sizeof(HTTP_REQUEST_HEADER_END)-1;

            constexpr uint HTTP_HEADER_BUFFER_SIZE=HGL_SIZE_1KB*4;
        }

		HTTPInputStream::HTTPInputStream()
		{
			tcp=nullptr;

			pos=-1;
			filelength=-1;

            tcp_is=nullptr;

            http_header=new char[HTTP_HEADER_BUFFER_SIZE];
            http_header_size=0;
		}

		HTTPInputStream::~HTTPInputStream()
		{
			Close();
		}

		/**
		* 创建流并打开一个文件
		* @param host 服务器地址 www.hyzgame.org.cn 或 127.0.0.1 之类
		* @param filename 路径及文件名 /download/hgl.rar 之类
		* @return 打开文件是否成功
		*/
		bool HTTPInputStream::Open(IPAddress *host_ip,const UTF8String &filename)
		{
			Close();

            if(!host_ip)
                RETURN_FALSE;

            if(filename.IsEmpty())
                RETURN_FALSE;

            tcp=CreateTCPClient(host_ip);

            char *host_ip_str=host_ip->CreateString();
            SharedArray<char> self_clear(host_ip_str);

            if(!tcp)
			{
				LOG_ERROR(U8_TEXT("连接HTTP服务器失败: ")+UTF8String(host_ip_str));
				RETURN_FALSE;
			}

			//设定为非堵塞模式
			tcp->SetBlock(false);

			//发送HTTP GET请求
            strcpy(http_header,HTTP_HEADER_BUFFER_SIZE,"GET ",4);

			strcat(http_header,HTTP_HEADER_BUFFER_SIZE,filename.c_str(),           filename.Length());
			strcat(http_header,HTTP_HEADER_BUFFER_SIZE,HTTP_REQUEST_HEADER_BEGIN,  HTTP_REQUEST_HEADER_BEGIN_SIZE);
			strcat(http_header,HTTP_HEADER_BUFFER_SIZE,host_ip_str,                strlen(host_ip_str));
			strcat(http_header,HTTP_HEADER_BUFFER_SIZE,HTTP_REQUEST_HEADER_END,    HTTP_REQUEST_HEADER_END_SIZE);

            const uint sendinfo_length=strlen(http_header);

            OutputStream *tcp_os=tcp->GetOutputStream();

            if(tcp_os->WriteFully(http_header,sendinfo_length)!=sendinfo_length)
			{
				LOG_ERROR(U8_TEXT("发送HTTP下载信息失败:")+UTF8String(host_ip_str));
				delete tcp;
                tcp=nullptr;
				RETURN_FALSE;
			}

            *http_header=0;

            tcp_is=tcp->GetInputStream();
			return(true);
		}

		/**
		* 关闭HTTP流
		*/
		void HTTPInputStream::Close()
		{
            pos=0;
            filelength=0;

            SAFE_CLEAR(tcp);

            delete[] http_header;
            http_header_size=0;
		}

		constexpr char HTTP_HEADER_SPLITE_STR[]="\r\n\r\n";
        constexpr uint HTTP_HEADER_SPLITE_STR_SIZE=sizeof(HTTP_HEADER_SPLITE_STR)-1;

        constexpr char HTTP_200_OK[]="200 OK";
        constexpr uint HTTP_200_OK_SIZE=sizeof(HTTP_200_OK)-1;

        constexpr char HTTP_CONTENT_LENGTH[]="Content-Length: ";
        constexpr uint HTTP_CONTENT_LENGTH_SIZE=sizeof(HTTP_CONTENT_LENGTH)-1;

		int HTTPInputStream::PraseHttpHeader()
		{
			char *offset;
			int size;

			offset=strstr(http_header,http_header_size,HTTP_HEADER_SPLITE_STR,HTTP_HEADER_SPLITE_STR_SIZE);

			if(offset!=nullptr)
			{
				*offset=0;

				size=http_header_size-(offset-http_header)-HTTP_HEADER_SPLITE_STR_SIZE;

				if(strstr(http_header,http_header_size,HTTP_200_OK,HTTP_200_OK_SIZE))
				{
					offset=strstr(http_header,http_header_size,HTTP_CONTENT_LENGTH,HTTP_CONTENT_LENGTH_SIZE);

					if(offset)
					{
						offset+=HTTP_CONTENT_LENGTH_SIZE;
						stou(offset,filelength);

                        pos=size;
						return(pos);
					}
					else	//有些HTTP下载不提供文件长度
					{
						return(-1);//这里还没有正确处理，有待增加
					}
				}
				else
				{
					LOG_ERROR(U8_TEXT("HTTP服务器返回错误信息: ")+UTF8String(http_header_str));
					return(-1);
				}
			}

			return(0);
		}

		/**
		* 从HTTP流中读取数据,但实际读取出来的数据长度不固定
		* @param buf 保存读出数据的缓冲区指针
		* @param bufsize 缓冲区长度,最小1024
		* @return >=0 实际读取出来的数据长度
		* @return -1 读取失败
		*/
		int64 HTTPInputStream::Read(void *buf,int64 bufsize)
		{
			if(!tcp)
                RETURN_ERROR(-1);

			int readsize;

			if(filelength==-1)    //HTTP头尚未解析完成
			{
                readsize=tcp_is->Read(http_header+http_header_size,HTTP_HEADER_BUFFER_SIZE-http_header_size);

				if(readsize<0)
				{
					const int error=GetLastSocketError();

					if(error!=10035        //windows 不能立即完成
                     &&error!=0)
					{
						LOG_ERROR(OS_TEXT("网络错误编号: ")+GetSocketErrorString(error));

						Close();
                        RETURN_ERROR(-2);
					}

					return(0);
				}

				http_header_size+=readsize;

				readsize=PraseHttpHeader();
				if(readsize==-1)
				{
					Close();
                    RETURN_ERROR(-3);
				}

				if(pos>0)
                    memcpy(buf,http_header+http_header_size-pos,pos);

				return(pos);
			}
			else
			{
				readsize=tcp_is->Read((char *)buf,bufsize);

				if(readsize==-1)
				{
					int error=GetLastSocketError();

					if(error!=10035
					 &&error!=0)
					{
						LOG_ERROR(OS_TEXT("网络错误编号: ")+GetSocketErrorString(error));

						Close();
                        RETURN_ERROR(-2);
					}

					return(0);
				}

				pos+=readsize;
				return(readsize);
			}
		}
	}//namespace network
}//namespace hgl
