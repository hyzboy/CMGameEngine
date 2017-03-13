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
			mtcNone=-1,

            mtcDiffuse=0,               ///<漫反射
            mtcAmbient,                 ///<环境光
            mtcSpecular,                ///<镜面光
            mtcNormal,                  ///<法线
            mtcTangent,                 ///<切线
            mtcBiNormal,                ///<副法线
            mtcBiTangent=mtcBiNormal,   ///<副切线
            mtcLight,                   ///<光照贴图
            mtcAlpha,                   ///<透明度贴图
			mtcDepth,
            mtcShadow,                  ///<阴影

            mtcPalette,                 ///<调色板

            mtcHeight,                  ///<高度图

            mtcDiffuseSpecular,         ///<漫反射+镜面光

            mtcNormalHeight,            ///<法线+高度
            mtcNormalTangent,           ///<法线+切线

			mtcGBuffer,					///<延迟渲染用GBuffer

            mtcMax                      ///<最大贴图通道
        };//enum MaterialTextureChannels

		const int MATERIAL_TEXTURE_CHANNEL_NAME_MAX_SIZE=32;

		using MATERIAL_TEXTURE_CHANNEL_NAME=char[MATERIAL_TEXTURE_CHANNEL_NAME_MAX_SIZE];

        bool GetMaterialTextureName(MATERIAL_TEXTURE_CHANNEL_NAME &,int);  ///<获取贴图通道名称
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_CHANNELS_INCLUDE
