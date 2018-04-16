#include<hgl/TypeFunc.h>
#include<cmath>
namespace hgl
{
    namespace algorithm
    {
        double inline Latan(double z)
        {
            constexpr double n1 = 0.97239411f;
            constexpr double n2 = -0.19194795f;

            return (n1 + n2 * z * z) * z;
        }

        double Latan2(double y, double x)
        {
            if (x != 0.0f)
            {
                if (abs(x) > abs(y))
                {
                    const double z = y / x;

                    if (x > 0)
                    {
                        return Latan(z);
                    }
                    else if (y >= 0)
                    {
                        return Latan(z) + HGL_PI;
                    }
                    else
                    {
                        return Latan(z) - HGL_PI;
                    }
                }
                else 
                {
                    const double z = x / y;

                    if (y > 0)
                    {
                        return -Latan(z) + HGL_PI_2;
                    }
                    else
                    {
                        return -Latan(z) - HGL_PI_2;
                    }
                }
            }
            else
            {
                if (y > 0) 
                {
                    return HGL_PI_2;
                }
                else if (y < 0) 
                {
                    return -HGL_PI_2;
                }
            }

            return 0; 
        }
    }//namespace algorithm
}//namespace hgl