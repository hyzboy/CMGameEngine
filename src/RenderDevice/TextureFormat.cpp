#include<hgl/graph/TextureFormat.h>
namespace hgl
{
    namespace graph
    {
        //贴图数据源格式
        const TextureFormat TextureFormatInfoList[]=
        {
            {HGL_SF_NONE,           "",             false,  0,  0,                                              GL_RGBA,        GL_UNSIGNED_BYTE            ,0,0},

            {HGL_SF_R8,             "R8",           false,  0,  GL_R8,                                          GL_RED,         GL_UNSIGNED_BYTE            ,1,1},
            {HGL_SF_RG8,            "RG8",          false,  0,  GL_RG8,                                         GL_RG,          GL_UNSIGNED_BYTE            ,2,2},
            {HGL_SF_RGB8,           "RGB8",         false,  0,  GL_RGB8,                                        GL_RGB,         GL_UNSIGNED_BYTE            ,3,3},
            {HGL_SF_RGBA8,          "RGBA8",        false,  0,  GL_RGBA8,                                       GL_RGBA,        GL_UNSIGNED_BYTE            ,4,4},

            {HGL_SF_SRGB8,          "SRGB8",        false,  0,  GL_SRGB8,                                       GL_SRGB,        GL_UNSIGNED_BYTE            ,4,4},
            {HGL_SF_SRGBA8,         "SRGBA8",       false,  0,  GL_SRGB8_ALPHA8,                                GL_SRGB_ALPHA,  GL_UNSIGNED_BYTE            ,5,5},

            {HGL_SF_R16,            "R16",          false,  0,  GL_R16,                                         GL_RED,         GL_UNSIGNED_SHORT           ,2,2},
            {HGL_SF_RG16,           "RG16",         false,  0,  GL_RG16,                                        GL_RG,          GL_UNSIGNED_SHORT           ,4,4},
            {HGL_SF_RGB16,          "RGB16",        false,  0,  GL_RGB16,                                       GL_RGB,         GL_UNSIGNED_SHORT           ,6,6},
            {HGL_SF_RGBA16,         "RGBA16",       false,  0,  GL_RGBA16,                                      GL_RGBA,        GL_UNSIGNED_SHORT           ,8,8},

            {HGL_SF_R16F,           "R16F",         false,  0,  GL_R16F,                                        GL_RED,         GL_FLOAT                    ,2,2},
            {HGL_SF_RG16F,          "RG16F",        false,  0,  GL_RG16F,                                       GL_RG,          GL_FLOAT                    ,4,4},
            {HGL_SF_RGB16F,         "RGB16F",       false,  0,  GL_RGB16F,                                      GL_RGB,         GL_FLOAT                    ,6,6},
            {HGL_SF_RGBA16F,        "RGBA16F",      false,  0,  GL_RGBA16F,                                     GL_RGBA,        GL_FLOAT                    ,8,8},

            {HGL_SF_R32F,           "R32F",         false,  0,  GL_R32F,                                        GL_RED,         GL_FLOAT                    ,4,4},
            {HGL_SF_RG32F,          "RG32F",        false,  0,  GL_RG32F,                                       GL_RG,          GL_FLOAT                    ,8,8},
            {HGL_SF_RGB32F,         "RGB32F",       false,  0,  GL_RGB32F,                                      GL_RGB,         GL_FLOAT                    ,12,12},
            {HGL_SF_RGBA32F,        "RGBA32F",      false,  0,  GL_RGBA32F,                                     GL_RGBA,        GL_FLOAT                    ,16,16},

            {HGL_SF_UNCOMPRESSED,   "",             false,  0,  0,                                              0,              0                           ,0,0},

            {HGL_SF_R3_G3_B2,       "R3_G3_B2",     false,  0,  GL_R3_G3_B2,                                    GL_RGB,         GL_UNSIGNED_BYTE_3_3_2      ,1,1},
            {HGL_SF_RGB5,           "RGB5",         false,  0,  GL_RGB5,                                        GL_RGB,         GL_UNSIGNED_SHORT_5_5_5_1   ,2,2},

            {HGL_SF_RGBA4,          "RGBA4",        false,  0,  GL_RGBA4,                                       GL_RGBA,        GL_UNSIGNED_SHORT_4_4_4_4   ,2,2},
            {HGL_SF_RGB5_A1,        "RGB5_A1",      false,  0,  GL_RGB5_A1,                                     GL_RGBA,        GL_UNSIGNED_SHORT_5_5_5_1   ,2,2},
            {HGL_SF_RGB10_A2,       "RGB10_A2",     false,  0,  GL_RGB10_A2,                                    GL_RGBA,        GL_UNSIGNED_INT_10_10_10_2  ,4,4},

            {HGL_SF_RG11F_B10F,     "RG11F_B10F",   false,  0,  GL_R11F_G11F_B10F,                              GL_RGB,         GL_UNSIGNED_INT_10F_11F_11F_REV ,4,4},
            {HGL_SF_RGB9_E5,        "RGB9_E5",      false,  0,  GL_RGB9_E5,                                     GL_RGB,         GL_UNSIGNED_INT_5_9_9_9_REV ,4,4},

            {HGL_SF_DEPTH,          "",             false,  0,  0,                                              0,              0                           ,0,0},

            {HGL_SF_DEPTH16,        "DEPTH16",        false,    0,    GL_DEPTH_COMPONENT16,                            GL_DEPTH,        GL_UNSIGNED_SHORT            ,2,2},
            {HGL_SF_DEPTH24,        "DEPTH24",        false,    0,    GL_DEPTH_COMPONENT24,                            GL_DEPTH,        GL_UNSIGNED_BYTE            ,3,3},
            {HGL_SF_DEPTH32,        "DEPTH32",        false,    0,    GL_DEPTH_COMPONENT32,                            GL_DEPTH,        GL_UNSIGNED_INT                ,4,4},

            {HGL_SF_DEPTH32F,        "DEPTH32F",        false,    0,    GL_DEPTH_COMPONENT32F,                            GL_DEPTH,        GL_FLOAT                    ,4,4},

            {HGL_SF_INDEX,          "",             false,  0,  0,                                              0,              0                           ,0,0},

            {HGL_SF_INDEX_16_RGB,   "16RGB",        false,  16, GL_LUMINANCE4,                                  GL_RGB,         GL_UNSIGNED_BYTE            ,0,0},
            {HGL_SF_INDEX_16_RGBA,  "16RGBA",       false,  16, GL_LUMINANCE4,                                  GL_RGBA,        GL_UNSIGNED_BYTE            ,0,0},
            {HGL_SF_INDEX_256_RGB,  "256RGB",       false,  256,GL_LUMINANCE8,                                  GL_RGB,         GL_UNSIGNED_BYTE            ,0,0},
            {HGL_SF_INDEX_256_RGBA, "256RGBA",      false,  256,GL_LUMINANCE8,                                  GL_RGBA,        GL_UNSIGNED_BYTE            ,0,0},

            {HGL_SF_COMPRESSED,     "",             false,  0,  0,                                              0,              0                           ,0,0},
/*
            {HGL_SF_CR,             "CR",           true,   0,  GL_COMPRESSED_RED,                              GL_RED,         0,0,0},             //注：GL_COMPRESSED RED/RG/RGB/RGBA/SRGB/SRGBA这6个格式
            {HGL_SF_CRG,            "CRG",          true,   0,  GL_COMPRESSED_RG,                               GL_RG,          0,0,0},             //只能用于glTexImage，由于显卡并不一定确定最终用什么算法，
            {HGL_SF_CRGB,           "CRGB",         true,   0,  GL_COMPRESSED_RGB,                              GL_RGB,         0,0,0},             //所以用glGetCompressTexImage取出的数据，不能在glCompressTexImage中用
            {HGL_SF_CRGBA,          "CRGBA",        true,   0,  GL_COMPRESSED_RGBA,                             GL_RGBA,        0,0,0},

            {HGL_SF_CSRGB,          "CSRGB",        true,   0,  GL_COMPRESSED_SRGB,                             GL_SRGB,        0,0,0},
            {HGL_SF_CSRGBA,         "CSRGBA",       true,   0,  GL_COMPRESSED_SRGB_ALPHA,                       GL_SRGB_ALPHA,  0,0,0},
*/
            {HGL_SF_DXT1RGB,        "DXT1RGB",      true,   0,  GL_COMPRESSED_RGB_S3TC_DXT1_EXT,                GL_RGB,         0,0,0},
            {HGL_SF_DXT1RGBA,       "DXT1RGBA",     true,   0,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,               GL_RGBA,        0,0,0},
            {HGL_SF_DXT3RGBA,       "DXT3RGBA",     true,   0,  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,               GL_RGBA,        0,0,0},
            {HGL_SF_DXT5RGBA,       "DXT5RGBA",     true,   0,  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,               GL_RGBA,        0,0,0},

            {HGL_SF_LATC1,          "LATC1",        true,   0,  GL_COMPRESSED_LUMINANCE_LATC1_EXT,              GL_RED,         0,0,0},
            {HGL_SF_LATC1s,         "LATC1s",       true,   0,  GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT,       GL_RED,         0,0,0},
            {HGL_SF_LATC2,          "LATC2",        true,   0,  GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,        GL_RG,          0,0,0},
            {HGL_SF_LATC2s,         "LATC2s",       true,   0,  GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT, GL_RG,          0,0,0},

            {HGL_SF_RGTC1,          "RGTC1",        true,   0,  GL_COMPRESSED_RED_RGTC1,                        GL_RED,         0,0,0},
            {HGL_SF_RGTC1s,         "RGTC1s",       true,   0,  GL_COMPRESSED_SIGNED_RED_RGTC1,                 GL_RED,         0,0,0},
            {HGL_SF_RGTC2,          "RGTC2",        true,   0,  GL_COMPRESSED_RG_RGTC2,                         GL_RG,          0,0,0},
            {HGL_SF_RGTC2s,         "RGTC2s",       true,   0,  GL_COMPRESSED_SIGNED_RG_RGTC2,                  GL_RG,          0,0,0},

            {HGL_SF_BPTC_RGBf,      "BPTC_RGBf",    true,   0,  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,            GL_RGB,         0,0,0},
            {HGL_SF_BPTC_RGBuf,     "BPTC_RGBuf",   true,   0,  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,          GL_RGB,         0,0,0},
            {HGL_SF_BPTC_RGBA,      "BPTC_RGBA",    true,   0,  GL_COMPRESSED_RGBA_BPTC_UNORM,                  GL_RGBA,        0,0,0},
            {HGL_SF_BPTC_SRGBA,     "BPTC_SRGBA",   true,   0,  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,            GL_SRGB_ALPHA,  0,0,0},

            {HGL_SF_ETC2_RGB8,        "ETC2_RGB8",    true,    0,    GL_COMPRESSED_RGB8_ETC2,                        GL_RGB,         0,0,0},
            {HGL_SF_ETC2_SRGB8,     "ETC2_SRGB8",    true,    0,    GL_COMPRESSED_SRGB8_ETC2,                        GL_SRGB,        0,0,0},
            {HGL_SF_ETC2_RGB8A1,    "ETC2_RGB8A1",    true,    0,    GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,    GL_RGBA,        0,0,0},
            {HGL_SF_ETC2_SRGB8A1,   "ETC2_SRGB8A1",    true,    0,    GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,    GL_SRGB_ALPHA,  0,0,0},
            {HGL_SF_ETC2_RGBA8,     "ETC2_RGBA8",    true,    0,    GL_COMPRESSED_RGBA8_ETC2_EAC,                    GL_RGBA,        0,0,0},
            {HGL_SF_ETC2_SRGBA8,    "ETC2_SRGBA8",    true,    0,    GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,            GL_SRGB_ALPHA,    0,0,0},

            {HGL_SF_EAC_R11,        "EAC_R11",        true,    0,    GL_COMPRESSED_R11_EAC,                            GL_RED,         0,0,0},
            {HGL_SF_EAC_R11s,       "EAC_R11s",        true,    0,    GL_COMPRESSED_SIGNED_R11_EAC,                    GL_RED,         0,0,0},
            {HGL_SF_EAC_RG11,       "EAC_RG11",        true,    0,    GL_COMPRESSED_RG11_EAC,                            GL_RG,             0,0,0},
            {HGL_SF_EAC_RG11s,      "EAC_RG11s",    true,    0,    GL_COMPRESSED_SIGNED_RG11_EAC,                    GL_RG,             0,0,0}
        };//const TextureFormat TextureFormatInfoList[]

        const int TextureFormatCount=sizeof(TextureFormatInfoList)/sizeof(TextureFormat);

        bool TextureSourceFormatCheck(const TextureSourceFormat &tsf)
        {
            if(tsf<=HGL_SF_NONE
             ||tsf>=HGL_SF_END
             ||tsf==HGL_SF_UNCOMPRESSED
             ||tsf==HGL_SF_INDEX
             ||tsf==HGL_SF_COMPRESSED)
                return(false);

            return(true);
        }

        bool TextureSourceFormatDepthCheck(const TextureSourceFormat &tsf)
        {
            return (tsf>HGL_SF_DEPTH&&tsf<HGL_SF_INDEX);
        }

        TSF GetTextureFormatEnum(const char *str)
        {
            char fmt[17];

            memcpy(fmt,str,16);
            fmt[16]=0;

            for(int i=0;i<TextureFormatCount;i++)
                if(::strcmp(fmt,TextureFormatInfoList[i].name)==0)
                    return(TextureFormatInfoList[i].tsf);

            return(HGL_SF_NONE);
        }
    }//namespace graph
}//namespace hgl
