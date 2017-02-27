#ifndef HGL_OPENGL_CORE_GLSL_INCLUDE
#define HGL_OPENGL_CORE_GLSL_INCLUDE

namespace hgl
{
	namespace graph
	{
		#define HGL_MAX_TEX_COORD		32
		#define HGL_MAX_VERTEX_BUFFER	32

		//vertex shader 矩阵部分
		#define HGL_VS_PROJECTION_MATRIX		"ProjectionMatrix"
		#define HGL_VS_MODELVIEW_MATRIX			"ModelViewMatrix"
		#define HGL_VS_MVP_MATRIX				"ModelViewProjectionMatrix"
		#define HGL_VS_NORMAL_MATRIX			"NormalMatrix"

		//vertex shader 传入部分，必须与VertexBufferName中的名称对应
		#define HGL_VS_NORMAL					"Normal"
		#define HGL_VS_COLOR					"Color"
		#define HGL_VS_VERTEX					"Vertex"
		#define HGL_VS_TEXCOORD_GBUFFER			"TexCoord_GBuffer"

        //vertex shader 传入的太阳光数据
        #define HGL_VS_SUN_LIGHT_DIRECTION      "SunLightDirection"    //太阳光方向
        #define HGL_VS_SUN_LIGHT_COLOR          "SunLightColor"        //太阳光颜色

        //fragment shader 部分
		#define HGL_FS_NORMAL					"FragmentNormal"
		#define HGL_FS_COLOR					"FragmentColor"
		#define HGL_FS_POSITION					"FragmentPosition"
		#define HGL_FS_TEXCOORD_GBUFFER			"FragmentTexCoord_GBuffer"

		#define HGL_MATERIAL_COLOR				"MaterialColor"

		#define HGL_FS_FRAG_COLOR				"FragColor"

		#define HGL_FS_ALPHA_TEST				"AlphaTest"

		#define HGL_FS_TEXCOORD					"FragmentTexCoord_"

        #define HGL_FS_SUN_LIGHT_INTENSITY      "FragmentSunLightIdtensity"     //太阳光强度
	}//namespace graph
}//namespace hgl
#endif//HGL_OPENGL_CORE_GLSL_INCLUDE
