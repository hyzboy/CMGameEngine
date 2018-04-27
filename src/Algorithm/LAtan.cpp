#include<hgl/algorithm/VectorMath.h>
#include<cmath>
namespace hgl
{
    namespace algorithm
    {
        /**
         * fast atan2
         * from: Jim Shima
         * date: 1999/04/23
         */
        double Latan2(double y, double x)
        {
            double abs_y = fabs(y) + 1e-10;      // kludge to prevent 0/0 condition
            double angle;
            double r;

            if (x >= 0)
            {
                r = (x - abs_y) / (x + abs_y);
                angle = HGL_PI_4 - HGL_PI_4 * r;
            }
            else
            {
                r = (x + abs_y) / (abs_y - x);
                angle = HGL_PI_3_4 - HGL_PI_4 * r;
            }

            if (y < 0)
                return (-angle);     // negate if in quad III or IV
            else
                return (angle);
        }
    }//namespace algorithm
}//namespace hgl
