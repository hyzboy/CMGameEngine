#ifndef HGL_VERTEX_2_CPP
#define HGL_VERTEX_2_CPP

#include<string.h>
#include<math.h>

namespace hgl
{
	template<typename T>
	void Vertex2<T>::Scale(double vx,double vy,double scale)
	{
		x=vx+(x-vx)*scale;
		y=vy+(y-vy)*scale;
	}

	template<typename T>
	void Vertex2<T>::Scale(double vx,double vy,double sx,double sy)
	{
		x=vx+(x-vx)*sx;
		y=vy+(y-vy)*sy;
	}
	
	template<typename T> template<typename N>
	void Vertex2<T>::Scale(const Vertex2<N> &v,double scale)
	{
		x=v.x+(x-v.x)*scale;
		y=v.y+(y-v.y)*scale;
	}

	template<typename T> template<typename N>
	void Vertex2<T>::Scale(const Vertex2<N> &v,double sx,double sy)
	{
		x=v.x+(x-v.x)*sx;
		y=v.y+(y-v.y)*sy;
	}
    
	template<typename T>
	void Vertex2<T>::Rotate(double vx,double vy,double ang)
	{
		double as,ac;
		double nx,ny;

        as=sin(ang*HGL_PI/180.0f);
		ac=cos(ang*HGL_PI/180.0f);

		nx=vx+((x-vx)*ac-(y-vy)*as);
		ny=vy+((x-vx)*as+(y-vy)*ac);

		x=nx;y=ny;
	}
	
	template<typename T>
	float Vertex2<T>::Length() const
	{
		return sqrt(x*x+y*y);
    }

	template<typename T>
	float Vertex2<T>::Length(float vx,float vy) const
	{
		float w=x-vx;
        float h=y-vy;

		return float(sqrt(double(w*w+h*h)));
    }
    
	template<typename T>
	float Vertex2<T>::Angle(float vx,float vy) const
	{
		if(vx==x)
		{
			if(vy>y)return(90);
			else return(270);
		}

		if(vy==y)
		{
			if(vx>x)return(0);
            	else return(180);
		}

		// asin 参数范围必须在 -1 到 +1 之间，否则会报错
		//

		double xx=vx-x;
		double yy=vy-y;

		double ang = atan(yy/xx)/HGL_PI*180.0f;

		if(xx<0)
			return(ang+180);
		else
		{
			if(yy<0)
				return(ang+360);
			else
				return(ang);
		}
	}
	
	template<typename T> template<typename N>
	Vertex2<T> Vertex2<T>::To(const Vertex2<N> &target,double pos)const
	{
		if(pos==0)return(*this);else
		if(pos==1)return(target);else
		{
			Vertex2<T> result;

			result.x=x+(target.x-x)*pos;
			result.y=y+(target.y-y)*pos;

			return(result);
		}
	}
    
	template<typename T>
	void Vertex2<T>::Normalize()
    {
        double size=sqrt(x*x+y*y);

        if(size>0)
        {
            x/=size;
            y/=size;
        }

        x=0;y=0;
	}
	
	template<typename T> template<typename N>
	Vertex2<T> Vertex2<T>::ResizeLength(const Vertex2<N> &target,double scale)const
	{
		Vertex2<T> result;

		result.x=target.x+(x-target.x)*scale;
		result.y=target.y+(y-target.y)*scale;

		return(result);
	}
	
	template<typename T> template<typename N>
	Vertex2<T> Vertex2<T>::ToLength(const Vertex2<N> &target,double new_length)const
	{
		return ResizeLength(target,new_length/Length(target));
	}
}//namespace hgl
#endif//HGL_VERTEX_2_CPP
