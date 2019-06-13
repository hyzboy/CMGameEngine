#ifndef HGL_GRAPH_TEXTURE_FORMAT_INCLUDE
#define HGL_GRAPH_TEXTURE_FORMAT_INCLUDE

#include<hgl/graph/TextureSourceFormat.h>
#include<hgl/graph/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        //贴图类型,核心模式，肯定支持array/cubemap
        #define HGL_TEXTURE_1D                  GL_TEXTURE_1D
        #define HGL_TEXTURE_2D                  GL_TEXTURE_2D
        #define HGL_TEXTURE_3D                  GL_TEXTURE_3D
        #define HGL_TEXTURE_RECTANGLE           GL_TEXTURE_RECTANGLE            //OpenGL 3.1
        #define HGL_TEXTURE_1D_ARRAY            GL_TEXTURE_1D_ARRAY
        #define HGL_TEXTURE_2D_ARRAY            GL_TEXTURE_2D_ARRAY             //OpenGL 3.0
        #define HGL_TEXTURE_CUBE_MAP            GL_TEXTURE_CUBE_MAP
        #define HGL_TEXTURE_CUBE_MAP_ARRAY      GL_TEXTURE_CUBE_MAP_ARRAY       //OpenGL 4.0
        #define HGL_TEXTURE_2D_MS               GL_TEXTURE_2D_MULTISAMPLE
        #define HGL_TEXTURE_2D_MS_ARRAY         GL_TEXTURE_2D_MULTISAMPLE_ARRAY //OpenGL 3.2

        #define HGL_TEXTURE_BUFFER              GL_TEXTURE_BUFFER               //TBO(OpenGL 3.1)

        #define HGL_TEX_BIND_1D                 GL_TEXTURE_BINDING_1D
        #define HGL_TEX_BIND_2D                 GL_TEXTURE_BINDING_2D
        #define HGL_TEX_BIND_3D                 GL_TEXTURE_BINDING_3D
        #define HGL_TEX_BIND_RECTANGLE          GL_TEXTURE_BINDING_RECTANGLE
        #define HGL_TEX_BIND_1D_ARRAY           GL_TEXTURE_BINDING_1D_ARRAY
        #define HGL_TEX_BIND_2D_ARRAY           GL_TEXTURE_BINDING_2D_ARRAY
        #define HGL_TEX_BIND_CUBE_MAP           GL_TEXTURE_BINDING_CUBE_MAP
        #define HGL_TEX_BIND_CUBE_MAP_ARRAY     GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
        #define HGL_TEX_BIND_2D_MS              GL_TEXTURE_BINDING_2D_MULTISAMPLE
        #define HGL_TEX_BIND_2D_MS_ARRAY        GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
        #define HGL_TEX_BIND_BUFFER             GL_TEXTURE_BINDING_BUFFER

        //采样属性

        //贴图寻址模式
        #define HGL_WRAP_CLAMP                      GL_CLAMP_TO_EDGE
        #define HGL_WRAP_REPEAT                     GL_REPEAT
        #define HGL_WRAP_MIRRORED_REPEAT            GL_MIRRORED_REPEAT

        //贴图过滤模式
        #define HGL_FILTER_NEAREST                  GL_NEAREST
        #define HGL_FILTER_LINEAR                   GL_LINEAR
        #define HGL_FILTER_SMOOTH                   (GL_LINEAR+1)               //(1.此功能为自行实现，使用GL_LINEAR+1只是为了区分。2.OpenGL 4.0以下使用性能较差)

        #define HGL_FILTER_NEAREST_MIPMAP_NEAREST   GL_NEAREST_MIPMAP_NEAREST
        #define HGL_FILTER_LINEAR_MIPMAP_NEAREST    GL_LINEAR_MIPMAP_NEAREST
        #define HGL_FILTER_NEAREST_MIPMAP_LINEAR    GL_NEAREST_MIPMAP_LINEAR
        #define HGL_FILTER_LINEAR_MIPMAP_LINEAR     GL_LINEAR_MIPMAP_LINEAR

        //贴图象素格式
        #define HGL_DEPTH16                     GL_DEPTH_COMPONENT16
        #define HGL_DEPTH24                     GL_DEPTH_COMPONENT24
        #define HGL_DEPTH32                     GL_DEPTH_COMPONENT32

        #define HGL_R8                          GL_R8
        #define HGL_R16                         GL_R16
        #define HGL_RG8                         GL_RG8
        #define HGL_RG16                        GL_RG16

        #define HGL_R3_G3_B2                    GL_R3_G3_B2
        #define HGL_RGB4                        GL_RGB4
        #define HGL_RGB5                        GL_RGB5
        #define HGL_RGB8                        GL_RGB8
        #define HGL_RGB10                       GL_RGB10
        #define HGL_RGB12                       GL_RGB12
        #define HGL_RGB16                       GL_RGB16
        #define HGL_RGBA2                       GL_RGBA2
        #define HGL_RGBA4                       GL_RGBA4
        #define HGL_RGB5_A1                     GL_RGB5_A1
        #define HGL_RGBA8                       GL_RGBA8
        #define HGL_RGB10_A2                    GL_RGB10_A2
        #define HGL_RGBA12                      GL_RGBA12
        #define HGL_RGBA16                      GL_RGBA16
        #define HGL_SRGB8                       GL_SRGB8
        #define HGL_SRGB8_ALPHA8                GL_SRGB8_ALPHA8

        #define HGL_R16F                        GL_R16F
        #define HGL_RG16F                       GL_RG16F
        #define HGL_RGB16F                      GL_RGB16F
        #define HGL_RGBA16F                     GL_RGBA16F

        #define HGL_R32F                        GL_R32F
        #define HGL_RG32F                       GL_RG32F
        #define HGL_RGB32F                      GL_RGB32F
        #define HGL_RGBA32F                     GL_RGBA32F

        #define HGL_RG11F_B10F                  GL_R11F_G11F_B10F
        #define HGL_RGB9_E5                     GL_RGB9_E5

        //压缩贴图格式,核心模式无扩展，所以不用检测
        #define HGL_CR                          GL_COMPRESSED_RED
        #define HGL_CRG                         GL_COMPRESSED_RG
        #define HGL_CRGB                        GL_COMPRESSED_RGB
        #define HGL_CRGBA                       GL_COMPRESSED_RGBA

        #define HGL_CSRGB                       GL_COMPRESSED_SRGB
        #define HGL_CSRGBA                      GL_COMPRESSED_SRGB_ALPHA

        #define HGL_DXT1RGB                     GL_COMPRESSED_RGB_S3TC_DXT1_EXT
        #define HGL_DXT1RGBA                    GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
        #define HGL_DXT3RGBA                    GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
        #define HGL_DXT5RGBA                    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        #define HGL_LATC1                       GL_COMPRESSED_LUMINANCE_LATC1_EXT
        #define HGL_LATC1s                      GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT
        #define HGL_LATC2                       GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT
        #define HGL_LATC2s                      GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT
        #define HGL_RGTC1                       GL_COMPRESSED_RED_RGTC1
        #define HGL_RGTC1s                      GL_COMPRESSED_SIGNED_RED_RGTC1
        #define HGL_RGTC2                       GL_COMPRESSED_RG_RGTC2
        #define HGL_RGTC2s                      GL_COMPRESSED_SIGNED_RG_RGTC2
        #define HGL_BPTC_RGBf                   GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB
        #define HGL_BPTC_RGBuf                  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB
        #define HGL_BPTC_RGBA                   GL_COMPRESSED_RGBA_BPTC_UNORM_ARB
        #define HGL_BPTC_SRGBA                  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB

        inline const TextureFormat *GetTextureFormat(const TSF &tsf)
        {
            if(tsf<=HGL_SF_NONE||tsf>=HGL_SF_END)
                return(nullptr);

            return TextureFormatInfoList+tsf;
        }

        inline const uint GetVideoFormat(const TSF &tsf)
        {
            if (tsf <= HGL_SF_NONE || tsf >= HGL_SF_END)
                return(0);

            return TextureFormatInfoList[tsf].video_format;
        }

        inline const TextureFormat *GetTextureFormat(const char *format)
        {
            const TextureSourceFormat tsf= GetTextureFormatEnum(format);

            if(tsf<=HGL_SF_NONE||tsf>=HGL_SF_END)
                return(nullptr);

            return TextureFormatInfoList+tsf;
        }
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_FORMAT_INCLUDE
