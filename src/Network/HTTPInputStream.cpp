#include<hgl/network/HTTPInputStream.h>
#include<hgl/network/Socket.h>
#include<hgl/LogInfo.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace network
	{
		HTTPInputStream::HTTPInputStream()
		{
			tcp=nullptr;

			http_header=nullptr;
			http_header_size=-1;

			pos=-1;
			filelength=-1;
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
		bool HTTPInputStream::Open(const UTF8String &host,const UTF8String &filename)
		{
			Close();

			//查找服务器
			in_addr iaHost;
			hostent *HostEntry=nullptr;

			memset(&iaHost,0,sizeof(in_addr));

			iaHost.s_addr=inet_addr(host);

			if(iaHost.s_addr==INADDR_NONE)HostEntry=gethostbyname(host);
		//                             else HostEntry=gethostbyaddr((char *)&iaHost,sizeof(in_addr),AF_INET);
		/*
			if(HostEntry==nullptr)
			{
				PutError(u"找不到服务器");
				return(false);
			}*/

			//建立socket连接
			ThisSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

			if(ThisSocket==INVALID_SOCKET)
			{
				PutError(u"创建socket失败！\n");
				ThisSocket=-1;
				return(false);
			}

			sockaddr_in addr;
			servent *ServerEnt=getservbyname("http","tcp");

			addr.sin_family     =AF_INET;

			if(HostEntry)addr.sin_addr.s_addr=((in_addr *)*HostEntry->h_addr_list)->s_addr;
					else addr.sin_addr.s_addr=inet_addr(host);

			addr.sin_port       =ServerEnt?ServerEnt->s_port:htons(80);

			//连接socket
			if(connect(ThisSocket,(sockaddr *)&addr,sizeof(sockaddr_in))==SOCKET_ERROR)
			{
				PutError(u"连接HTTP服务器失败！\n");
				closesocket(ThisSocket);
				ThisSocket=-1;
				return(false);
			}

			//设定为非堵塞模式
			unsigned long par=1;

			ioctlsocket(ThisSocket,FIONBIO,&par);

			//发送HTTP GET请求
			char sendinfo[HGL_SIZE_1MB]="GET ";

			strcat(sendinfo,	filename);
			strcat(sendinfo,	" HTTP/1.1\n"
								"Host: ");
			strcat(sendinfo,	host);
			strcat(sendinfo,	"\n"
								"Accept: */*\n"
								"User-Agent: Mozilla/5.0\n"
								"Connection: Keep-Alive\n\n");

			if(send(ThisSocket,sendinfo,(int)strlen(sendinfo),0)==SOCKET_ERROR)
			{
				PutError(u"发送HTTP下载信息失败！\n");
				closesocket(ThisSocket);
				ThisSocket=-1;
				return(false);
			}

			http_header=new char[HGL_SIZE_1MB];
			memset(http_header,0,HGL_SIZE_1MB);
			http_header_size=0;

			return(true);
		}

		/**
		* 关闭HTTP流
		*/
		void HTTPInputStream::Close()
		{
			if(ThisSocket!=-1)
			{
				if(http_header)
				{
					delete[] http_header;
					http_header=nullptr;
				}

				CloseSocket(ThisSocket);

				ThisSocket=-1;
				pos=-1;
				filelength=-1;
			}
		}

		int HTTPInputStream::Parsehttp_header()
		{
			char *offset;
			int size;

			offset=strstr(http_header,"\r\n\r\n");

			if(offset!=nullptr)
			{
				*offset=0;

				size=http_header_size-(offset-http_header)-4;

				if(strstr(http_header,"200 OK"))
				{
					offset=strstr(http_header,"Content-Length:");

					if(offset)
					{
						offset+=16;
						filelength=atoi(offset);

						return(pos=size);
					}
					else	//有些HTTP下载不提供文件长度
					{
						return(-1);//这里还没有正确处理，有待增加
					}
				}
				else
				{
					ErrorHint(ccpGBK,"HTTP服务器返回错误信息：\n%s",http_header);
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
		int HTTPInputStream::_Read(void *buf,int bufsize)
		{
			int num;
			int readsize;

			if(ThisSocket==-1)return(-1);

			if(filelength==-1)    //HTTP头尚未解析完成
			{
				num=recv(ThisSocket,http_header+http_header_size,1024-http_header_size,0);
				if(num==-1)
				{
					int error=GetLastSocketError();

					if(error!=10035&&error!=0)
					{
						ErrorHint(u"网络错误编号:%d",error);

						Close();
						return(-1);
					}

					return(0);
				}

				http_header_size+=num;

				readsize=Parsehttp_header();
				if(readsize==-1)
				{
					Close();
					return(-1);
				}

				memcpy(buf,http_header+http_header_size-pos,pos);
				return(pos);
			}
			else
			{
				readsize=recv(ThisSocket,(char *)buf,bufsize,0);

				if(readsize==-1)
				{
					int error=GetLastSocketError();

					if(error!=10035
					 &&error!=0)
					{
						ErrorHint(u"网络错误编号:%d",error);

						Close();
						return(-1);
					}

					return(0);
				}

				pos+=readsize;
				return(readsize);
			}
		}
	}//namespace network
}//namespace hgl
