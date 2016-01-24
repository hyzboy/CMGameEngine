#include"GLSL.h"
#include<hgl/graph/Shader.h>
#include<hgl/LogInfo.h>
#include<hgl/Other.h>
#include<hgl/type/Smart.h>
#include<glew/include/GL/glew.h>
#include<malloc.h>

namespace hgl
{
	namespace graph
	{
		uint OpenGLShaderType[ShaderType::stEnd]=
		{
			GL_VERTEX_SHADER,					///<顶点着色程序
			GL_TESS_CONTROL_SHADER,				///<镶嵌控制着色程序(需OpenGL 4.0或ARB_tessellation_shader)
			GL_TESS_EVALUATION_SHADER,			///<镶嵌评估着色程序(需OpenGL 4.0或ARB_tessellation_shader)
			GL_GEOMETRY_SHADER,					///<几何着色程序
			GL_FRAGMENT_SHADER,					///<片断着色程序
			GL_COMPUTE_SHADER					///<计算着色程序(需OpenGL 4.3或ARB_compute_shader)
		};

		const char ShaderName[ShaderType::stEnd][32]=
		{
			"Vertex",
			"TessControl",
			"TessEvaluation",
			"Geometry",
			"Fragment",
			"Compute"
		};

		namespace
		{
			/**
			* 编译一段shader
			* @param type 类型
			* @param name 名称
			* @param shader_source 代码
			* @return 编译成功的shader句柄
			* @return 0 编译失败
			*/
			GLuint CompileShader(GLenum type,const char *name,const char *shader_source)
			{
				if(!shader_source)return(0);

				GLuint shader=glCreateShader(type);

				glShaderSource(shader,1,&shader_source,0);

				glCompileShader(shader);

				GLint compiled,log_length;

				glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);

				if(compiled)
					return(shader);

				glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&log_length);

				GLint char_writen;
				char *log=new char[log_length];

				glGetShaderInfoLog(shader,log_length,&char_writen,log);

				LOG_HINT(UTF8String(name)+U8_TEXT(" shader compile error\n\n")+ UTF8String(log));

				delete[] log;

                LOG_ERROR(shader_source);

				glDeleteShader(shader);

				return(0);
			}
		}//namespace

		void Shader::Clear()
		{
			for(int i=0;i<ShaderType::stEnd;i++)
				if(shader_index[i])
				{
					glDeleteShader(shader_index[i]);
					shader_index[i]=0;
				}

			if(program)
			{
				glDeleteProgram(program);
				program=0;
			}
		}

		/**
		* 编译一个glsl着色程序
		* @param shader_type 着色程序类型
		* @param shader_code 着色程序代码
		* @return 是否添加编程成功
		*/
		bool Shader::AddShader(const int shader_type,const char *shader_codes)
		{
			if(shader_type<0||shader_type>=ShaderType::stEnd)return(false);
			if(shader_index[shader_type])return(false);

			if(!shader_codes
			 ||!(*shader_codes))return(false);

			shader_index[shader_type]=CompileShader(OpenGLShaderType[shader_type],ShaderName[shader_type],shader_codes);

			if(!shader_index[shader_type])
				return(false);

			return(true);
		}

		bool Shader::Build()
		{
			program=glCreateProgram();

			for(int i=0;i<ShaderType::stEnd;i++)
				if(shader_index[i])
					glAttachShader(program,shader_index[i]);

			return(Link());
		}

		bool Shader::Link()
		{
			if(!program)return(false);

			glLinkProgram(program);

			GLint linked;

			glGetProgramiv(program,GL_LINK_STATUS,&linked);

			if(linked)
				return(true);

			GLint log_length,char_written;
			char *log;

			glGetProgramiv(program,GL_INFO_LOG_LENGTH,&log_length);

			log=new char[log_length];

			glGetProgramInfoLog(program,log_length,&char_written,log);

			LOG_ERROR(u8"Shader program link error\n\n"+UTF8String(log));

			delete[] log;

			Clear();
			return(false);
		}

		bool Shader::Use()																				///<使用当前着色程序
		{
			if(!program)
				return(false);

			glUseProgram(program);
			return(true);
		}

		/**
		* 取得属性索引地址
		* @param name 属性名称
		* @return 索引地址
		* @return -1 出错
		*/
		int Shader::_GetAttribLocation(const char *name)
		{
			if(!program)
			{
				LOG_ERROR(u8"GetAttribLocation("+UTF8String(name)+u8"),program=0");
				return(-1);
			}

			return glGetAttribLocation(program,name);
		}

		//bool Shader::SetAttrib1f(int location,float value)
		//{
		//	if(!program)
		//		return(false);

		//	glVertexAttrib1f(location,value);

		//	return(true);
		//}

		//bool Shader::GetAttrib1f(int location,float &value)
		//{
		//	if(!program)
		//		return(false);

		//	glGetVertexAttribfv(location,GL_CURRENT_VERTEX_ATTRIB,&value);

		//	return(true);
		//}

		/**
		* 取得一个变量的地址
		* @param name 变量名称
		* @return 地址
		* @return -1 出错
		*/
		int Shader::_GetUniformLocation(const char *name)													///<取得一个变量的地址
		{
			if(!program)
			{
				LOG_ERROR(u8"GetUniformLocation("+UTF8String(name)+u8"),program=0");
				return(-1);
			}

			const int result=glGetUniformLocation(program,name);

			if(result==-1)
			{
				const int gl_error=glGetError();

				LOG_ERROR(u8"GetUniformLocation("+UTF8String(name)+u8"),program="+UTF8String(program)+u8",result=-1,gl_error="+UTF8String(gl_error));
			}

			return(result);
		}

		#define HGL_GLSL_CHECK_PROGRAM_AND_LOCATION(func)	if(!program)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniform" #func ",program=0");	\
																return(false);	\
															}	\
															\
															if(location<0)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniform" #func ",location="+UTF8String(location));	\
																return(false);	\
															}

		#define HGL_GLSL_SetUniform1234(func,type)	bool Shader::SetUniform1##func(int location,type v0)	\
													{	\
														HGL_GLSL_CHECK_PROGRAM_AND_LOCATION(1##func)	\
													\
														glUniform1##func(location,v0);	\
														return(true);	\
													}	\
													\
													bool Shader::SetUniform2##func(int location,type v0,type v1)	\
													{	\
														HGL_GLSL_CHECK_PROGRAM_AND_LOCATION(2##func)	\
													\
														glUniform2##func(location,v0,v1);	\
														return(true);	\
													}	\
													\
													bool Shader::SetUniform3##func(int location,type v0,type v1,type v2)	\
													{	\
														HGL_GLSL_CHECK_PROGRAM_AND_LOCATION(3##func)	\
													\
														glUniform3##func(location,v0,v1,v2);	\
														return(true);	\
													}	\
													\
													bool Shader::SetUniform4##func(int location,type v0,type v1,type v2,type v3)	\
													{	\
														HGL_GLSL_CHECK_PROGRAM_AND_LOCATION(4##func)	\
													\
														glUniform4##func(location,v0,v1,v2,v3);	\
														return(true);	\
													}
		HGL_GLSL_SetUniform1234(f,float);
		HGL_GLSL_SetUniform1234(i,int);
		HGL_GLSL_SetUniform1234(ui,unsigned int);

		#undef HGL_GLSL_SetUniform1234

		#define HGL_GLSL_SetUniformPointer(func,type)	bool Shader::SetUniform##func(int location,const type *value)	\
														{	\
															if(!program)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniform" #func ",program=0");	\
																return(false);	\
															}	\
														\
															if(location<0)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniform" #func ",location="+UTF8String(location));	\
																return(false);	\
															}	\
														\
															if(!value)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniform" #func ",value=nullptr");	\
																return(false);	\
															}	\
														\
															glUniform##func(location,1,value);	\
															return(true);	\
														}

		HGL_GLSL_SetUniformPointer(1fv,float);
		HGL_GLSL_SetUniformPointer(2fv,float);
		HGL_GLSL_SetUniformPointer(3fv,float);
		HGL_GLSL_SetUniformPointer(4fv,float);

		HGL_GLSL_SetUniformPointer(1iv,int);
		HGL_GLSL_SetUniformPointer(2iv,int);
		HGL_GLSL_SetUniformPointer(3iv,int);
		HGL_GLSL_SetUniformPointer(4iv,int);

		HGL_GLSL_SetUniformPointer(1uiv,unsigned int);
		HGL_GLSL_SetUniformPointer(2uiv,unsigned int);
		HGL_GLSL_SetUniformPointer(3uiv,unsigned int);
		HGL_GLSL_SetUniformPointer(4uiv,unsigned int);

		#undef HGL_GLSL_SetUniformPointer

		#define HGL_GLSL_SetUniformMatrixPointer(func)	bool Shader::SetUniformMatrix##func(int location,const float *mat)	\
														{	\
															if(!program)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniformMatrix" #func ",program=0");	\
																return(false);	\
															}	\
														\
															if(location<0)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniformMatrix" #func ",location="+UTF8String(location));	\
																return(false);	\
															}	\
														\
															if(!mat)	\
															{	\
																LOG_ERROR(u8"Shader::SetUniformMatrix" #func ",mat=nullptr");	\
																return(false);	\
															}	\
														\
															glUniformMatrix##func(location,1,GL_FALSE,mat);	\
															return(true);	\
														}

		HGL_GLSL_SetUniformMatrixPointer(2fv);
		HGL_GLSL_SetUniformMatrixPointer(3fv);
		HGL_GLSL_SetUniformMatrixPointer(4fv);

		HGL_GLSL_SetUniformMatrixPointer(2x3fv);
		HGL_GLSL_SetUniformMatrixPointer(3x2fv);
		HGL_GLSL_SetUniformMatrixPointer(2x4fv);
		HGL_GLSL_SetUniformMatrixPointer(4x2fv);
		HGL_GLSL_SetUniformMatrixPointer(3x4fv);
		HGL_GLSL_SetUniformMatrixPointer(4x3fv);

		#undef HGL_GLSL_SetUniformMatrixPointer

        int Shader::_GetUniformBlockIndex(const char *uniform_block_name)
        {
            if(!program)
            {
                LOG_ERROR(u8"Shader::BindUniformBlock("+UTF8String(uniform_block_name)+") program=0");
                return(false);
            }

            const int index=glGetUniformBlockIndex(program,uniform_block_name);

            if(index<0)
            {
                LOG_ERROR(u8"Shader::BindUniformBlock("+UTF8String(uniform_block_name)+") block_index error");
                return(false);
            }

            return index;
        }
	}//namespace graph
}//namespace hgl
