#ifndef HGL_OPENGL_CORE_GLSL_INCLUDE
#define HGL_OPENGL_CORE_GLSL_INCLUDE

namespace hgl
{
	namespace graph
	{
		#define HGL_MAX_TEX_COORD		32
		#define HGL_MAX_VERTEX_BUFFER	32

		#define HGL_SKY_COLOR					"SkyColor"
		#define HGL_SKY_COLOR_FUNC				"sky_light_compute"

		//vertex shader 矩阵部分
		#define HGL_VS_PROJECTION_MATRIX		"ProjectionMatrix"
		#define HGL_VS_MODELVIEW_MATRIX			"ModelViewMatrix"
		#define HGL_VS_MVP_MATRIX				"ModelViewProjectionMatrix"
		#define HGL_VS_NORMAL_MATRIX			"NormalMatrix"

		//vertex shader 传入部分，必须与VertexBufferName中的名称对应
		#define HGL_VS_NORMAL					"Normal"
		#define HGL_VS_COLOR					"Color"
		#define HGL_VS_VERTEX					"Vertex"

		//fragment shader 部分
		#define HGL_FS_NORMAL					"FragmentNormal"
		#define HGL_FS_COLOR					"FragmentColor"
		#define HGL_FS_LIGHT					"FragmentLight"

		#define HGL_MATERIAL_COLOR				"MaterialColor"

		#define HGL_FS_LIGHT_INTENSITY			"LightIntensity"		//灯光强度

		#define HGL_FS_FRAG_COLOR				"FragColor"

		#define HGL_FS_ALPHA_TEST				"AlphaTest"

		#define HGL_FS_TEXCOORD					"FragmentTexCoord_"
	}//namespace graph
}//namespace hgl
#endif//HGL_OPENGL_CORE_GLSL_INCLUDE
