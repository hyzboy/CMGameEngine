#ifndef HGL_GRAPH_HSV_COLOR_INCLUDE
#define HGL_GRAPH_HSV_COLOR_INCLUDE

#include<hgl/type/Color3f.h>

namespace hgl
{
    namespace graph
    {
        inline void rgb2hsv(Color3f &hsv,const Color3f &rgb)
        {
           double min, max, delta, tmp;

           tmp = rgb.r>rgb.g ? rgb.g : rgb.r;
           min = tmp>rgb.b ? rgb.b : tmp;
           tmp = rgb.r>rgb.g ? rgb.r : rgb.g;
           max = tmp>rgb.b ? tmp : rgb.b;
           hsv.b = max;
           delta = max - min;

           if (max != 0)
               hsv.g = delta / max;
           else
           {
               hsv.g = 0;
               hsv.r = 0;
               return;
           }

           if (delta == 0)
           {
               hsv.r = 0;
               return;
           }
           else if (rgb.r == max)
           {
               if (rgb.g >= rgb.b)
                   hsv.r = (rgb.g - rgb.b) / delta;
               else
                   hsv.r = (rgb.g - rgb.b) / delta + 6.0;
           }
           else if (rgb.g == max)
               hsv.r = 2.0 + (rgb.b - rgb.r) / delta;
           else if (rgb.b == max)
               hsv.r = 4.0 + (rgb.r - rgb.g) / delta;

           hsv.r /= 6.0;
        }
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_HSV_COLOR_INCLUDE
