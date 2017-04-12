#include"FindPath_Line.h"

namespace hgl
{
    namespace findpath
    {
        __inline double abs(double v)
        {
            return v>0?v:-v;
        }

        LineFindPath::LineFindPath(int w,int h):FindPath(w,h)
        {
        }

        bool LineFindPath::PutPixel(int x,int y)
        {
            if(CheckPoint(x,y,px,py))
            {
                Steps.Add(x,y);            //添加到路径中

                px=x;
                py=y;

                return(true);
            }
            else
            {
                return(false);
            }
        }

        bool LineFindPath::Line(int x1,int y1,int x2,int y2)
        {
            double dx,dy,e,x,y;

            x=x1;
            y=y1;
            dx=x2-x1;
            dy=y2-y1;
            px=x;
            py=y;

            e=abs(dx)>abs(dy)?abs(dx):abs(dy);

            for(int i=0;i<e;i++)        //标准DDALine是i=1,i<=e，但我们是倒画
            {
                if(!PutPixel(x+dx*(i/e),y+dy*(i/e)))
                    return(false);
            }

            return(true);
        }

        bool LineFindPath::find_path()
        {
            if(!Line(EndX,EndY,StartX,StartY))
                return(false);

            return(true);
        }

        FindPath *CreateLine(int w,int h)
        {
            return(new LineFindPath(w,h));
        }
    }//namespace findpath
}//namespace hgl
