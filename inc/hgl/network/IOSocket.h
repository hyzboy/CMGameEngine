#ifndef HGL_NETWORK_IOSOCKET_INCLUDE
#define HGL_NETWORK_IOSOCKET_INCLUDE

#include<hgl/network/Socket.h>

namespace hgl
{
    namespace network
    {
        /**
         * 输入输出用Socket
         */
        class IOSocket:public Socket                                                                ///传输数据用Socket基类
        {
        protected:

            int64 send_total;                                                                       ///<发送字节数统计
            int64 recv_total;                                                                       ///<接收字节数统计

            double recv_time_out;                                                                   ///<数据接收超时
            double last_recv_time;                                                                  ///<最后一次接收数据的时间

        public:

            IOSocket()
            {
                Clear();
            }

            virtual ~IOSocket()=default;

            virtual void    Clear()
            {
                send_total=0;
                recv_total=0;
                recv_time_out=HGL_NETWORK_DOUBLE_TIME_OUT;
                last_recv_time=0;
            }

            virtual void    CloseSocket()override                                                   ///<关闭连接
            {
                Socket::CloseSocket();
                Clear();
            }

            virtual const   double &GetRecvTimeOut()const{return recv_time_out;}                    ///<取得接收数据超时时间
            virtual         void    SetRecvTimeOut(const double to){recv_time_out=to;}              ///<设置接收数据超时时间

        public: //事件函数

            /**
             * 接收数据处理事件函数
             * @param recv_buf_size 缓冲区内的数据长度(注:此参数并不是所有平台都可以获取，所以在值为-1时，需要不停的获取数据直到出错)
             * @param ct 当前时间
             * @return 成功获取的数据字节数
             */
            virtual int     OnRecv(int recv_buf_size=-1,const double ct=0)
            {
                last_recv_time=ct;
                return 0;
            }

            /**
             * 发送数据处理事件函数
             * @param send_buf_size 发送数据缓冲区内可用最大长度(注:此参数并不是所有平台都可以获取，所以在值为-1时，依然可以发送数据)
             * @param left_bytes 剩余未发送的字节数(如果为0，下一次将不参与轮循。此功能待测试是否需要，因为将SOCKET从列表中反复添加删除，也会影响效能)
             * @return 成功发送的数据字节数
             */
            virtual int     OnSend(int send_buf_size/*,int &left_bytes*/)
            {
                return 0;
            }

            /**
             * 错误处理事件函数
             * @param errno_number 错误号
             */
            virtual void    OnError(int errno_number)
            {
            }

            /**
             * 关闭事件函数
             */
            virtual void    OnClose()
            {
            }

            /**
             * 刷新事件函数，由开发者重写实现。这里故意写为纯虚函数，是为了提醒开发者此函数的存在，即便什么都不做，也必须由开发者自行实现。
             * @return 是否正常，如返回否则会让管理器认为此socket出错需要退出
             */
            virtual bool    OnUpdate()=0;

        public: //属性函数

                    const   int64   GetSendTotal()const{return send_total;}                         ///<取得发送字节数统计
                    const   int64   GetRecvTotal()const{return recv_total;}                         ///<取得接收字节数统计

                    const   void    RestartLastRecvTime(){last_recv_time=0;}                        ///<复位最后获取数据时间
                    const   double  GetLastRecvTime()const{return last_recv_time;}                  ///<取得最后获取数据时间
                    const   bool    CheckRecvTimeOut(const double ct)                               ///<检测是否超时
                    {
                        if((last_recv_time>0)
                            &&(last_recv_time+recv_time_out<ct))
                            return(true);

                        return(false);
                    }
        };//class IOSocket
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_IOSOCKET_INCLUDE

