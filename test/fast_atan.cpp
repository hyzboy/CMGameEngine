// fast_tri.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<cmath>
#include<stdlib.h>

#ifdef _MSC_VER
    #include<Windows.h>

    using uint64=unsigned __int64;
    using float128=long double;

    constexpr float128 PI           =3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;
    constexpr float128 PI_2         =1.5707963267948966192313216916397514;
    constexpr float128 PI_4         =0.7853981633974483096156608458198757;
#else
//     extern "C"
//     {
//         #include<quadmath.h>
//     }

    using uint64=unsigned long long;
    using float128=__float128;

    constexpr float128 PI           =3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068q;
    constexpr float128 PI_2         =1.5707963267948966192313216916397514Q;
    constexpr float128 PI_4         =0.7853981633974483096156608458198757Q;
#endif//

    constexpr float128 PI_3_4       =PI*0.75;

float inline ApproxAtan(float z)
{
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;

    return (n1 + n2 * z * z) * z;
}

double ApproxAtan2(double y, double x)
{
    if (x != 0.0f)
    {
        if (abs(x) > abs(y))
        {
            const float z = y / x;
            if (x > 0)
            {
                return ApproxAtan(z);
            }
            else if (y >= 0)
            {
                return ApproxAtan(z) + PI;
            }
            else
            {
                return ApproxAtan(z) - PI;
            }
        }
        else
        {
            const float z = x / y;
            if (y > 0)
            {
                return -ApproxAtan(z) + PI_2;
            }
            else
            {
                return -ApproxAtan(z) - PI_2;
            }
        }
    }
    else
    {
        if (y > 0)
        {
            return PI_2;
        }
        else if (y < 0)
        {
            return -PI_2;
        }
    }

    return 0;
}

constexpr double M_PI_4_P_0273=PI_4+0.273;

double atan2approx(double y,double x)
{
  double absx, absy;
  absy = fabs(y);
  absx = fabs(x);
  short octant = ((x<0) << 2) + ((y<0) << 1 ) + (absx <= absy);
  switch (octant) {
    case 0: {
        if (x == 0 && y == 0)
          return 0;
        double val = absy/absx;
        return (M_PI_4_P_0273 - 0.273*val)*val; //1st octant
        break;
      }
    case 1:{
        if (x == 0 && y == 0)
          return 0.0;
        double val = absx/absy;
        return PI_2 - (M_PI_4_P_0273 - 0.273*val)*val; //2nd octant
        break;
      }
    case 2: {
        double val =absy/absx;
        return -(M_PI_4_P_0273 - 0.273*val)*val; //8th octant
        break;
      }
    case 3: {
        double val =absx/absy;
        return -PI_2 + (M_PI_4_P_0273 - 0.273*val)*val;//7th octant
        break;
      }
    case 4: {
        double val =absy/absx;
        return  PI - (M_PI_4_P_0273 - 0.273*val)*val;  //4th octant
      }
    case 5: {
        double val =absx/absy;
        return  PI_2 + (M_PI_4_P_0273 - 0.273*val)*val;//3rd octant
        break;
      }
    case 6: {
        double val =absy/absx;
        return -PI + (M_PI_4_P_0273 - 0.273*val)*val; //5th octant
        break;
      }
    case 7: {
        double val =absx/absy;
        return -PI_2 - (M_PI_4_P_0273 - 0.273*val)*val; //6th octant
        break;
      }
    default:
      return 0.0;
    }
}

double arctan2(double y, double x)
{
    double abs_y = fabs(y)+1e-10;      // kludge to prevent 0/0 condition
    double angle;
    double r;

    if (x>=0)
    {
        r = (x - abs_y) / (x + abs_y);
        angle = PI_4 - PI_4 * r;
    }
    else
    {
        r = (x + abs_y) / (abs_y - x);
        angle = PI_3_4 - PI_4 * r;
    }

    if (y < 0)
        return(-angle);     // negate if in quad III or IV
    else
        return(angle);
}

double fastAtan2(double y, double x)
{
    const double ay = y < 0 ? -y : y;
    const double ax = x < 0 ? -x : x;
    const double a = (ax < ay ? ax : ay) / (ax > ay ? ax : ay);
    const double s = a * a;
    double r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;
    if (ay > ax) r = 1.57079637 - r;
    if (x < 0) r = PI - r;
    if (y < 0) r = -r;
    return r;
}

inline int toAngle(double value)
{
    return value*180.0f/PI;
}

/**
* 取得当前时间
* @return 当前时间(单位：百万分之一秒)
*/
#ifdef _MSC_VER
uint64 GetMicroTime()
{
    SYSTEMTIME st;

    FILETIME ft;

    GetLocalTime(&st);

    SystemTimeToFileTime(&st, &ft);

    return(*(uint64 *)(&ft));
}
#else
uint64 GetMicroTime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    return (ts.tv_sec*1000*1000)+(ts.tv_nsec/1000);
}
#endif//

struct ATAN_TEST
{
    char name[32];
    double (*func)(double,double);
};

static ATAN_TEST atan_test[]=
{
    {"atan2",atan2},
    {"ApproxAtan2",ApproxAtan2},
    {"atan2approx",atan2approx},
    {"arctan2",arctan2},
    {"fastAtan2",fastAtan2},
    {"",nullptr}
};

int main()
{
    const int COUNT=1024*10;

    struct POINT
    {
        double x,y;
    };

    POINT *data=new POINT[COUNT];

    srand(GetMicroTime());

    for(int i=0;i<36;i++)
    {
        double x=(rand()%1000)-500;
        double y=(rand()%1000)-500;

        std::cout<<"coord: "<<x<<","<<y;

        for(int j=0;;j++)
        {
            if(atan_test[j].func==nullptr)break;

            std::cout<<"\t\t "<<atan_test[j].func(y,x);
        }

        std::cout<<std::endl;
    }

    for(int i=0;i<COUNT;i++)
    {
        data[i].x=(rand()%1000)-500;
        data[i].y=(rand()%1000)-500;

        if(data[i].x==0)data[i].x=1;
        if(data[i].y==0)data[i].y=1;
    }

    uint64 st,et;
    double total=0;

    std::cout<<"------------------------------------"<<std::endl;

    for(int k=0;;k++)
    {
        if(atan_test[k].func==nullptr)break;

        st=GetMicroTime();
        for(int i=0;i<COUNT;i++)
        {
            total+=atan_test[k].func(data[i].y,data[i].x);

            for(int j=0;j<COUNT;j++)
                total+=atan_test[k].func(data[i].y,data[j].x);
        }
        et=GetMicroTime();

        std::cout<<atan_test[k].name<<"\ttotal: "<<total<<"\ttime: "<<(et-st)<<std::endl;
    }

    delete[] data;
    return 0;
}

