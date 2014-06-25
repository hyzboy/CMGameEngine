#include<hgl/VectorMath.h>
#include<iostream>

using namespace hgl;
using namespace std;

int main()
{
	Vector3f off(300,270,0);
 	Vector3f up_vector(0,0,1);
 	Vector3f axis=cross(off,up_vector);
 	Vector4f result;

 	axis.normalize();

 	Matrix4f rotate_mat=rotate(29,axis);
;
 	result=Vector4f(off,1.0f)*rotate_mat;

	cout<<"axis: "<<axis[0]<<","<<axis[1]<<","<<axis[2]<<endl;
 	cout<<"result: "<<result[0]<<","<<result[1]<<","<<result[2]<<endl;

	return(0);
}
