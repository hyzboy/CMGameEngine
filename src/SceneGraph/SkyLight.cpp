#include<hgl/graph/SkyLight.h>

namespace hgl
{
	namespace scene
	{
		//天空色要素
		//季节：春、夏、秋、冬
		//地理环境：沙漠、森林、海边、湖泊、黄土高原、草原、..........
		//天气环境：少云、多云、雨量........

		//现阶段仅按一个简单规则实现

		const float SkyLightColor[48][3]=
		{
			DEF_RGB_U8_TO_F(  0,  0,  0),		//0
			DEF_RGB_U8_TO_F(  0,  0,  0),		//1
			DEF_RGB_U8_TO_F(  0,  0,  0),		//2
			DEF_RGB_U8_TO_F(  0,  0,  0),		//3
			DEF_RGB_U8_TO_F(  0,  0,  0),		//4
			DEF_RGB_U8_TO_F(  0,  0,  0),		//5
			DEF_RGB_U8_TO_F(  0,  0,  0),		//6
			DEF_RGB_U8_TO_F(  0,  0,  0),		//7
			DEF_RGB_U8_TO_F(  0,  0,  0),		//8
			DEF_RGB_U8_TO_F(  0,  0,  0),		//9
			DEF_RGB_U8_TO_F(  0,  0,  0),		//10
			DEF_RGB_U8_TO_F( 15, 15, 21),		//11
			DEF_RGB_U8_TO_F( 86, 87,116),		//12
			DEF_RGB_U8_TO_F(145,151,168),		//13
			DEF_RGB_U8_TO_F(204,214,220),		//14
			DEF_RGB_U8_TO_F(238,250,250),		//15
			DEF_RGB_U8_TO_F(243,251,251),		//16
			DEF_RGB_U8_TO_F(248,253,253),		//17
			DEF_RGB_U8_TO_F(243,251,251),		//18
			DEF_RGB_U8_TO_F(255,255,255),		//19
			DEF_RGB_U8_TO_F(255,255,255),		//20
			DEF_RGB_U8_TO_F(255,255,255),		//21
			DEF_RGB_U8_TO_F(255,255,255),		//22
			DEF_RGB_U8_TO_F(255,255,255),		//23
			DEF_RGB_U8_TO_F(255,255,255),		//24
			DEF_RGB_U8_TO_F(255,255,255),		//25
			DEF_RGB_U8_TO_F(255,255,255),		//26
			DEF_RGB_U8_TO_F(255,255,255),		//27
			DEF_RGB_U8_TO_F(255,255,255),		//28
			DEF_RGB_U8_TO_F(255,255,255),		//29
			DEF_RGB_U8_TO_F(255,255,255),		//30
			DEF_RGB_U8_TO_F(255,255,255),		//31
			DEF_RGB_U8_TO_F(241,224,172),		//32
			DEF_RGB_U8_TO_F(224,189, 89),		//33
			DEF_RGB_U8_TO_F(127,124, 53),		//34
			DEF_RGB_U8_TO_F(131, 60, 17),		//35
			DEF_RGB_U8_TO_F(102, 38,  8),		//36
			DEF_RGB_U8_TO_F( 76, 28,  6),		//37
			DEF_RGB_U8_TO_F( 50, 19,  4),		//38
			DEF_RGB_U8_TO_F( 24,  9,  2),		//39
			DEF_RGB_U8_TO_F(  0,  0,  0),		//40
			DEF_RGB_U8_TO_F(  0,  0,  0),		//41
			DEF_RGB_U8_TO_F(  0,  0,  0),		//42
			DEF_RGB_U8_TO_F(  0,  0,  0),		//43
			DEF_RGB_U8_TO_F(  0,  0,  0),		//44
			DEF_RGB_U8_TO_F(  0,  0,  0),		//45
			DEF_RGB_U8_TO_F(  0,  0,  0),		//46
			DEF_RGB_U8_TO_F(  0,  0,  0)		//47
		};

		/**
		* 根据时间产生天空灯光数据
		* @param time 时间,以小时为单位
		*/
		void SkyLight::FromTime(double time)
		{
			while(time<0)time+=24;

			while(time>=24)time-=24;

			if(time<=10||time>=40)		//晚上
				diffuse.Set(0,0,0);
			else
			if(time>=19&&time<=31)		//白天
				diffuse.Set(1,1,1);
			else						//渐变实现
			{
				const int index=time;

				time-=index;

				diffuse=SkyLightColor[index];
				diffuse.To(SkyLightColor[index+1],time);
			}

			//direction=
		}
	}//namespace scene
}//namespace hgl

