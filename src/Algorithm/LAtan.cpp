#include<hgl/algorithm/VectorMath.h>
#include<cmath>
namespace hgl
{
    namespace algorithm
    {
        /**
         * fast atan2
         * from£ºJim Shima
         * date£º1999/04/23
         */
        double Latan2(double y, double x) 
        {
            constexpr double coeff_1 = HGL_PI / 4;
            constexpr double coeff_2 = 3 * coeff_1;

            double abs_y = fabs(y) + 1e-10;      // kludge to prevent 0/0 condition
            double angle;
            double r;

            if (x >= 0) 
            {
                r = (x - abs_y) / (x + abs_y);
                angle = coeff_1 - coeff_1 * r;
            } 
            else 
            {
                r = (x + abs_y) / (abs_y - x);
                angle = coeff_2 - coeff_1 * r;
            }

            if (y < 0) 
                return (-angle);     // negate if in quad III or IV
            else
                return (angle);
        }
    }//namespace algorithm
}//namespace hgl