#ifndef HGL_GRAPH_TEXTURE_CHANNELS_INCLUDE
#define HGL_GRAPH_TEXTURE_CHANNELS_INCLUDE

namespace hgl
{
    namespace graph
    {
        /**
        * 贴图通道数据枚举
        */
        enum MaterialTextureChannels
        {
            mtcDiffuse=0,               ///<漫反射
            mtcAmbient,                 ///<环境光
            mtcSpecular,                ///<镜面光
            mtcNormal,                  ///<法线
            mtcTangent,                 ///<切线
            mtcLight,                   ///<光照贴图
            mtcAlpha,                   ///<透明度贴图
            mtcShadow,                  ///<阴影

            mtcPalette,                 ///<调色板

            mtcHeight,                  ///<高度图
            mtcNormalHeight,            ///<法线+高度

            mtcMax                      ///<最大贴图通道
        };//enum MaterialTextureChannels

        extern const char MaterialTextureName[mtcMax][32];  //贴图通道名称(用在fragment shader中)
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_CHANNELS_INCLUDE
