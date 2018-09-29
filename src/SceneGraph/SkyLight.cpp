#include<hgl/graph/Light.h>

namespace hgl
{
    namespace graph
    {
        //天空色要素
        //季节：春、夏、秋、冬
        //地理环境：沙漠、森林、海边、湖泊、黄土高原、草原、..........
        //天气环境：少云、多云、雨量........

        //现阶段仅按一个简单规则实现

        const Color3f SkyLightColor[48]=
        {
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //0
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //1
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //2
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //3
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //4
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //5
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //6
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //7
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //8
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //9
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //10
            DEF_RGB_U8_TO_COLOR3F( 15, 15, 21),        //11
            DEF_RGB_U8_TO_COLOR3F( 86, 87,116),        //12
            DEF_RGB_U8_TO_COLOR3F(145,151,168),        //13
            DEF_RGB_U8_TO_COLOR3F(204,214,220),        //14
            DEF_RGB_U8_TO_COLOR3F(238,250,250),        //15
            DEF_RGB_U8_TO_COLOR3F(243,251,251),        //16
            DEF_RGB_U8_TO_COLOR3F(248,253,253),        //17
            DEF_RGB_U8_TO_COLOR3F(243,251,251),        //18
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //19
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //20
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //21
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //22
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //23
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //24
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //25
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //26
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //27
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //28
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //29
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //30
            DEF_RGB_U8_TO_COLOR3F(255,255,255),        //31
            DEF_RGB_U8_TO_COLOR3F(241,224,172),        //32
            DEF_RGB_U8_TO_COLOR3F(224,189, 89),        //33
            DEF_RGB_U8_TO_COLOR3F(127,124, 53),        //34
            DEF_RGB_U8_TO_COLOR3F(131, 60, 17),        //35
            DEF_RGB_U8_TO_COLOR3F(102, 38,  8),        //36
            DEF_RGB_U8_TO_COLOR3F( 76, 28,  6),        //37
            DEF_RGB_U8_TO_COLOR3F( 50, 19,  4),        //38
            DEF_RGB_U8_TO_COLOR3F( 24,  9,  2),        //39
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //40
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //41
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //42
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //43
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //44
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //45
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0),        //46
            DEF_RGB_U8_TO_COLOR3F(  0,  0,  0)        //47
        };

        /**
        * 根据时间产生天空灯光数据
        * @param time 时间,以秒为单位
        */
        void GetSkyLightColor(Color4f &result,uint time)
        {
            if(time>=HGL_TIME_ONE_DAY)
                time%=HGL_TIME_ONE_DAY;

            time/=HGL_TIME_HALF_HOUR;    //除半小时，理论上结果应该是0-47

            if(time<=10||time>=40)        //晚上
                result=BlackColor3f;
            else
            if(time>=19&&time<=31)        //白天
                result=WhiteColor3f;
            else                        //渐变实现
            {
                const uint index=(uint)time;

                time-=index;

                result=SkyLightColor[index];
                result.To(SkyLightColor[index+1],time);
            }
        }
    }//namespace graph
}//namespace hgl

