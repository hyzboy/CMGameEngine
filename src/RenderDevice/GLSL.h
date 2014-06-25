#ifndef HGL_OPENGL_CORE_GLSL_INCLUDE
#define HGL_OPENGL_CORE_GLSL_INCLUDE

#include<hgl/graph/Shader.h>
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

		#define HGL_VS_LIGHT_POSITION			"LightPosition"			//光源位置
		#define HGL_VS_GLOBAL_LIGHT_INTENSITY	"GlobalLightIntensity"	//全局环境光强度

		//fragment shader 部分
		#define HGL_FS_NORMAL					"FragmentNormal"
		#define HGL_FS_COLOR					"FragmentColor"

		#define HGL_FS_LIGHT_INTENSITY			"LightIntensity"		//灯光强度

		#define HGL_FS_FRAG_COLOR				"FragColor"

		#define HGL_FS_ALPHA_TEST				"AlphaTest"

		#define HGL_FS_TEXCOORD					"FragmentTexCoord_"

		/**
		* OpenGL Shanding Language
		*/
		class GLSL:public Shader
		{
			unsigned int	program;

			unsigned int	shader_index[ShaderType::stEnd];

		private:

			bool Link();																								///<连接使用当前着色程序

		protected:

			int _GetAttribLocation(const char *);																		///<取得指定属性地址
			int _GetUniformLocation(const char *);																		///<取得一个变量的地址

		public:

			GLSL(){program=0;hgl_zero(shader_index,ShaderType::stEnd);}
			~GLSL(){Clear();}

			void Clear();																								///<清除着色程序

			bool AddShader(const int,const char *shader_code);															///<添加一段着色程序
			bool Build();																								///<构建现有的着色程序

			bool Use();																									///<使用当前着色程序

			//bool SetAttrib1f(int,float);
			//bool GetAttrib1f(int,float &);

		public:	//设置一致变量用函数

			bool SetUniform1f(int,float);
			bool SetUniform2f(int,float,float);
			bool SetUniform3f(int,float,float,float);
			bool SetUniform4f(int,float,float,float,float);

			bool SetUniform1i(int,int);
			bool SetUniform2i(int,int,int);
			bool SetUniform3i(int,int,int,int);
			bool SetUniform4i(int,int,int,int,int);

			bool SetUniform1ui(int,unsigned int);
			bool SetUniform2ui(int,unsigned int,unsigned int);
			bool SetUniform3ui(int,unsigned int,unsigned int,unsigned int);
			bool SetUniform4ui(int,unsigned int,unsigned int,unsigned int,unsigned int);

			bool SetUniform1fv(int,const float *);
			bool SetUniform2fv(int,const float *);
			bool SetUniform3fv(int,const float *);
			bool SetUniform4fv(int,const float *);

			bool SetUniform1iv(int,const int *);
			bool SetUniform2iv(int,const int *);
			bool SetUniform3iv(int,const int *);
			bool SetUniform4iv(int,const int *);

			bool SetUniform1uiv(int,const unsigned int *);
			bool SetUniform2uiv(int,const unsigned int *);
			bool SetUniform3uiv(int,const unsigned int *);
			bool SetUniform4uiv(int,const unsigned int *);

			bool SetUniformMatrix2fv(int,const float *);
			bool SetUniformMatrix3fv(int,const float *);
			bool SetUniformMatrix4fv(int,const float *);

			bool SetUniformMatrix2x3fv(int,const float *);
			bool SetUniformMatrix3x2fv(int,const float *);
			bool SetUniformMatrix2x4fv(int,const float *);
			bool SetUniformMatrix4x2fv(int,const float *);
			bool SetUniformMatrix3x4fv(int,const float *);
			bool SetUniformMatrix4x3fv(int,const float *);
		};//class GLSL
	}//namespace graph
}//namespace hgl
#endif//HGL_OPENGL_CORE_GLSL_INCLUDE
