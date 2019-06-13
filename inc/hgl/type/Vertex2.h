#ifndef HGL_VERTEX_2_INCLUDE
#define HGL_VERTEX_2_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/CompOperator.h>
namespace hgl
{
    /**
    * 2D顶点坐标类模板
    */
    template<typename T> class Vertex2                                                                      ///XY 2D顶点坐标类模板
    {
    public:

        T x,y;                                                                                              ///<坐标值

    public:

        Vertex2<T>(){x=y=0;}                                                                                ///<本类构造函数
        Vertex2<T>(T nx,T ny){x=nx,y=ny;}                                                                   ///<本类构造函数
        template<typename N>
        Vertex2<T>(const Vertex2<N> &v){x=v.x,y=v.y;}                                                       ///<本类构造函数

        void    Zero(){x=0,y=0;}                                                                            ///<将x和y全清为0
        void    One(){x=1,y=1;}                                                                             ///<将x和y全清为1

        void    Set(float nx,float ny){x=nx,y=ny;}                                                          ///<设置坐标值为nx,ny

                                void    Flip(float vx,float vy){FlipX(vy);FlipY(vx);}                       ///<对应vx,vy做翻转
        template<typename N>    void    Flip(const Vertex2<N> &v){Flip(v.x,v.y);}                           ///<对应v做翻转

                                void    FlipX(){y=-y;}                                                      ///<对应X轴翻转
                                void    FlipY(){x=-x;}                                                      ///<对应Y轴翻转
                                void    FlipX(float v){y=v-(y-v);}                                          ///<对应指定X轴做翻转
                                void    FlipY(float v){x=v-(x-v);}                                          ///<对应指定Y轴做翻转

                                void    Scale(double,double,double);                                        ///<相对某一点做缩放
                                void    Scale(double,double,double,double);                                 ///<相对某一点做缩放
        template<typename N>    void    Scale(const Vertex2<N> &,double);                                   ///<相对某一点做缩放
        template<typename N>    void    Scale(const Vertex2<N> &,double,double);                            ///<相对某一点做指定比例的缩放

                                void    Rotate(double,double,double);                                       ///<绕指定坐标旋转指定角度
        template<typename N>    void    Rotate(const Vertex2<N> &v,double ang){Rotate(v.x,v.y,ang);}        ///<绕指定坐标旋转指定角度

                                void    Move(float vx,float vy){x+=vx;y+=vy;}                               ///<移动
        template<typename N1,typename N2>
                                void    Scale(const Vertex2<N1> &v,const Vertex2<N2> &l){Scale(v,l.x,l.y);} ///<相对某一点做指定比例的缩放

                                float   Length()const;                                                      ///<计算当前点与零点之间的距离
                                float   Length(float vx,float vy)const;                                     ///<计算当前点与指定点之间的距离
                                float   Angle(float vx,float vy)const;                                      ///<计算当前点与指定点之间的角度

        template<typename N>    float   Length(const Vertex2<N> &v)const{return(Length(v.x,v.y));}          ///<计算当前点与指定点之间的距离
        template<typename N>    float   Angle(const Vertex2<N> &v)const{return(Angle(v.x,v.y));}            ///<计算当前点与指定点之间的角度
                                void    Normalize();

        template<typename N>    Vertex2<T>  ResizeLength(const Vertex2<N> &,double)const;                   ///<调整这个方向上两点的距离
        template<typename N>    Vertex2<T>  ToLength(const Vertex2<N> &,double)const;                       ///<按这个方向将两点的距离调到指定长度

        template<typename N>    Vertex2<T>  To(const Vertex2<N> &,double)const;

    public: //操作符重载

        template<typename N> bool operator == (const Vertex2<N> &v)const
        {
            if(x!=v.x)return(false);
            if(y!=v.y)return(false);
            return(true);
        }

        template<typename N> bool operator != (const Vertex2<N> &v)const
        {
            if(x!=v.x)return(true);
            if(y!=v.y)return(true);
            return(false);
        }

                                Vertex2<T> &operator = (const float *v){x=*v++;y=*v;return *this;}

        template<typename N>    Vertex2<T> &operator +=(const Vertex2<N> &v){x+=v.x;y+=v.y;return *this;}
        template<typename N>    Vertex2<T> &operator -=(const Vertex2<N> &v){x-=v.x;y-=v.y;return *this;}
        template<typename N>    Vertex2<T> &operator *=(const Vertex2<N> &v){x*=v.x;y*=v.y;return *this;}
        template<typename N>    Vertex2<T> &operator /=(const Vertex2<N> &v){x/=v.x;y/=v.y;return *this;}

                                Vertex2<T> &operator *=(const float v){x*=v;y*=v;return *this;}
                                Vertex2<T> &operator /=(const float v){x/=v;y/=v;return *this;}

        template<typename N>    Vertex2<T>  operator + (const Vertex2<N> &v)const{return(Vertex2<T>(x+v.x,y+v.y));}
        template<typename N>    Vertex2<T>  operator - (const Vertex2<N> &v)const{return(Vertex2<T>(x-v.x,y-v.y));}
        template<typename N>    Vertex2<T>  operator * (const Vertex2<N> &v)const{return(Vertex2<T>(x*v.x,y*v.y));}
        template<typename N>    Vertex2<T>  operator / (const Vertex2<N> &v)const{return(Vertex2<T>(x/v.x,y/v.y));}

                                Vertex2<T>  operator * (const float v)const{return(Vertex2<T>(x*v,y*v));}
                                Vertex2<T>  operator / (const float v)const{return(Vertex2<T>(x/v,y/v));}

                                Vertex2<T>  operator - () const {return(Vertex2<T>(-x,-y));}

        operator       T *() const {return((      T *)this);}                               ///<使得本类可以直接当做T *使用
        operator const T *() const {return((const T *)this);}                               ///<使得本类可以直接当做const T *使用
    };//class Vertex2

    typedef Vertex2<int> Vertex2i;
    typedef Vertex2<uint> Vertex2ui;
    typedef Vertex2<float> Vertex2f;
    typedef Vertex2<double> Vertex2d;

    typedef Vertex2<int8> Vertex2b;
    typedef Vertex2<uint8> Vertex2ub;
    typedef Vertex2<int16> Vertex2s;
    typedef Vertex2<uint16> Vertex2us;
}//namespace hgl
#include<hgl/type/Vertex2.cpp>
#endif//HGL_VERTEX_2_INCLUDE
