#include<hgl/algorithm/VectorMath.h>
#include<cmath>
namespace hgl
{
    namespace algorithm
    {
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