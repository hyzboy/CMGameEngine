#include<hgl/graph/Shader.h>

namespace hgl
{
	namespace graph
	{
		/**
		* 取得属性索引地址
		* @param name 属性名称
		* @return 索引地址
		* @return -1 出错
		*/
		int Shader::GetAttribLocation(const char *name)
		{
			if(!name)return(-1);

			int result;

			//if(!attrib_location.Get(name,result))
			{
				result=_GetAttribLocation(name);

				//attrib_location.Add(name,result);
			}

			return result;
		}

		/**
		* 取得一个变量的地址
		* @param name 变量名称
		* @return 地址
		* @return -1 出错
		*/
		int Shader::GetUniformLocation(const char *name)													///<取得一个变量的地址
		{
			if(!name)return(-1);

			int result;

			//if(!uniform_location.Get(name,result))
			{
				result=_GetUniformLocation(name);

				//uniform_location.Add(name,result);
			}

			return result;
		}

		/**
        * 取得一个变量的地址块索引
        * @param name 变量名称
        * @return 地址块索引
        * @return -1 出错
        */
        int Shader::GetUniformBlockIndex(const char *name)                                                    ///<取得一个变量的地址
        {
            if(!name)return(-1);

            int result;

            //if(!uniform_block_index.Get(name,result))
            {
                result=_GetUniformBlockIndex(name);

                //uniform_block_index.Add(name,result);
            }

            return result;
        }


		#define HGL_GLSL_SetUniform1234(func,type)	bool Shader::SetUniform1##func(const char *uniform_name,type v0)	\
													{	\
														const int location=GetUniformLocation(uniform_name);	\
														\
														if(location==-1)return(false);	\
														\
														return SetUniform1##func(location,v0);	\
													}	\
													\
													bool Shader::SetUniform2##func(const char *uniform_name,type v0,type v1)	\
													{	\
														const int location=GetUniformLocation(uniform_name);	\
														\
														if(location==-1)return(false);	\
														\
														return SetUniform2##func(location,v0,v1);	\
													}	\
													\
													bool Shader::SetUniform3##func(const char *uniform_name,type v0,type v1,type v2)	\
													{	\
														const int location=GetUniformLocation(uniform_name);	\
														\
														if(location==-1)return(false);	\
														\
														return SetUniform3##func(location,v0,v1,v2);	\
													}	\
													\
													bool Shader::SetUniform4##func(const char *uniform_name,type v0,type v1,type v2,type v3)	\
													{	\
														const int location=GetUniformLocation(uniform_name);	\
														\
														if(location==-1)return(false);	\
														\
														return SetUniform4##func(location,v0,v1,v2,v3);	\
													}
		HGL_GLSL_SetUniform1234(f,float);
		HGL_GLSL_SetUniform1234(i,int);
		HGL_GLSL_SetUniform1234(ui,unsigned int);

		#undef HGL_GLSL_SetUniform1234

		#define HGL_GLSL_SetUniformPointer(func,type)	bool Shader::SetUniform##func(const char *uniform_name,const type *value)	\
														{	\
															const int location=GetUniformLocation(uniform_name);	\
															\
															if(location==-1)return(false);	\
															\
															return SetUniform##func(location,value);	\
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

		#define HGL_GLSL_SetUniformMatrixPointer(func)	bool Shader::SetUniformMatrix##func(const char *uniform_name,const float *mat)	\
														{	\
															const int location=GetUniformLocation(uniform_name);	\
															\
															if(location==-1)return(false);	\
															\
															return SetUniformMatrix##func(location,mat);	\
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

        bool Shader::BindUniformBlock(const char *name,const int block_binding)
        {
            if(!name||!(*name))
            {
                LOG_ERROR(U8_TEXT("ShaderDataBlock name error:"));
                return(false);
            }

            if(block_binding<=0)
            {
                LOG_ERROR(U8_TEXT("ShaderDataBlock BlockBinding error,name:")+UTF8String(name));
                return(false);
            }

            const int block_index=_GetUniformBlockIndex(name);

            if(block_index<=0)
            {
                LOG_ERROR(U8_TEXT("ShaderDataBlock name error:")+UTF8String(name));
                return(false);
            }

            return _BindUniformBlock(block_index,block_binding);
        }
	}//namespace graph
}//namespace hgl
