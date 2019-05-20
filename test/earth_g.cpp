#include<iostream>

#ifdef __GNUC__
using float128 = __float128;
#else
using float128 = long double;
#endif//

constexpr float128 HGL_UNIVERSAL_GRAVITATION = 6.67384e-11;	        //万有引力常数
constexpr float128 HGL_GRAVITATIONAL_ACCELERATION = 9.80665;		//地球重力加速度(牛顿)

constexpr float128 HGL_EARTH_MASS = 5.9722e+24;						//地球质量
constexpr float128 HGL_EARTH_RADIUS = 6371000;						//地球半径(米)

/**
* 星球重力加速度<br>
* 理论上: (地球质量*万有引力常数)/(地球半径的平方)=地球重力加速度
* @param m 星球质量
* @param raidus 星球半径
* @return 星球的重力加速度(牛顿)
*/
template<typename T>
inline T UniversalGravitation(const T m, const T radius)
{
	return (HGL_UNIVERSAL_GRAVITATION*m) / (radius*radius);
}

int main(int,char **)
{
	const float128 earth_g = UniversalGravitation<float128>(HGL_EARTH_MASS, HGL_EARTH_RADIUS);

	std::cout << "earth gravitational: " << earth_g << std::endl;

	return 0;
}
