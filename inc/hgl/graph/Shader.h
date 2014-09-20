#ifndef HGL_GRAPH_SHADER_INCLUDE
#define HGL_GRAPH_SHADER_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
namespace hgl
{
	namespace graph
	{
		#define HGL_MAX_SHADER_NAME_LENGTH	128									///<最大Shader名称长度

		/**
		 * 着色程序类型枚举
		 */
		enum ShaderType					///着色程序类型
		{
			stVertex=0,					///<顶点着色程序
			stTessControl,				///<镶嵌控制着色程序(需OpenGL 4.0或ARB_tessellation_shader)
			stTessEval,					///<镶嵌评估着色程序(需OpenGL 4.0或ARB_tessellation_shader)
			stGeometry,					///<几何着色程序
			stFragment,					///<片断着色程序
			stCompute,					///<计算着色程序(需OpenGL 4.3或ARB_compute_shader)

			stEnd
		};//enum ShaderType

		extern const char ShaderName[ShaderType::stEnd][32];															///<着色程序名称

		/**
		* 着色器程序
		*/
		class Shader
		{
		protected:

			Map<UTF8String,int> attrib_location;
			Map<UTF8String,int> uniform_location;

			virtual int _GetAttribLocation(const char *)=0;																///<取得指定属性地址
			virtual int _GetUniformLocation(const char *)=0;															///<取得一个变量的地址

		public:

			Shader()HGL_DEFAULT_MEMFUNC;
			virtual ~Shader()HGL_DEFAULT_MEMFUNC;

			virtual void Clear()=0;																						///<清除着色程序

			virtual bool AddShader(const int shader_type,const char *)=0;												///<增加一个着色程序

					bool AddVertexShader	(const char *code){return AddShader(ShaderType::stVertex,code);}			///<增加一个顶点着色程序
					bool AddGeometryShader	(const char *code){return AddShader(ShaderType::stGeometry,code);}			///<增加一个几何着色程序
					bool AddFragmentShader	(const char *code){return AddShader(ShaderType::stFragment,code);}			///<增加一个片断着色程序
					bool AddComputeShader	(const char *code){return AddShader(ShaderType::stFragment,code);}			///<增加一个计算着色程序

					bool AddTessShader(const char *tess_control_shader,const char *tess_evaluation_shader)				///<增加一个镶嵌着色程序
					{
						if(!AddShader(ShaderType::stTessControl,tess_control_shader		))return(false);
						if(!AddShader(ShaderType::stTessEval,	tess_evaluation_shader	))return(false);
						return(true);
					}

			virtual bool Build()=0;																						///<构建当前添加的着色程序

			virtual bool Use()=0;																						///<使用当前着色程序

			virtual int GetAttribLocation(const char *);																///<取得指定属性地址
			virtual int GetUniformLocation(const char *);																///<取得一个变量的地址

			//bool SetAttrib1f(int,float);
			//bool GetAttrib1f(int,float &);

		public:	//设置一致变量用函数

			virtual bool SetUniform1f(int,float)=0;
			virtual bool SetUniform2f(int,float,float)=0;
			virtual bool SetUniform3f(int,float,float,float)=0;
			virtual bool SetUniform4f(int,float,float,float,float)=0;

			virtual bool SetUniform1i(int,int)=0;
			virtual bool SetUniform2i(int,int,int)=0;
			virtual bool SetUniform3i(int,int,int,int)=0;
			virtual bool SetUniform4i(int,int,int,int,int)=0;

			virtual bool SetUniform1ui(int,unsigned int)=0;
			virtual bool SetUniform2ui(int,unsigned int,unsigned int)=0;
			virtual bool SetUniform3ui(int,unsigned int,unsigned int,unsigned int)=0;
			virtual bool SetUniform4ui(int,unsigned int,unsigned int,unsigned int,unsigned int)=0;

			virtual bool SetUniform1fv(int,const float *)=0;
			virtual bool SetUniform2fv(int,const float *)=0;
			virtual bool SetUniform3fv(int,const float *)=0;
			virtual bool SetUniform4fv(int,const float *)=0;

			virtual bool SetUniform1iv(int,const int *)=0;
			virtual bool SetUniform2iv(int,const int *)=0;
			virtual bool SetUniform3iv(int,const int *)=0;
			virtual bool SetUniform4iv(int,const int *)=0;

			virtual bool SetUniform1uiv(int,const unsigned int *)=0;
			virtual bool SetUniform2uiv(int,const unsigned int *)=0;
			virtual bool SetUniform3uiv(int,const unsigned int *)=0;
			virtual bool SetUniform4uiv(int,const unsigned int *)=0;

			virtual bool SetUniformMatrix2fv(int,const float *)=0;
			virtual bool SetUniformMatrix3fv(int,const float *)=0;
			virtual bool SetUniformMatrix4fv(int,const float *)=0;

			virtual bool SetUniformMatrix2x3fv(int,const float *)=0;
			virtual bool SetUniformMatrix3x2fv(int,const float *)=0;
			virtual bool SetUniformMatrix2x4fv(int,const float *)=0;
			virtual bool SetUniformMatrix4x2fv(int,const float *)=0;
			virtual bool SetUniformMatrix3x4fv(int,const float *)=0;
			virtual bool SetUniformMatrix4x3fv(int,const float *)=0;

		public:

			bool SetUniform1f(const char *,float);
			bool SetUniform2f(const char *,float,float);
			bool SetUniform3f(const char *,float,float,float);
			bool SetUniform4f(const char *,float,float,float,float);

			bool SetUniform1i(const char *,int);
			bool SetUniform2i(const char *,int,int);
			bool SetUniform3i(const char *,int,int,int);
			bool SetUniform4i(const char *,int,int,int,int);

			bool SetUniform1ui(const char *,unsigned int);
			bool SetUniform2ui(const char *,unsigned int,unsigned int);
			bool SetUniform3ui(const char *,unsigned int,unsigned int,unsigned int);
			bool SetUniform4ui(const char *,unsigned int,unsigned int,unsigned int,unsigned int);

			bool SetUniform1fv(const char *,const float *);
			bool SetUniform2fv(const char *,const float *);
			bool SetUniform3fv(const char *,const float *);
			bool SetUniform4fv(const char *,const float *);

			bool SetUniform1iv(const char *,const int *);
			bool SetUniform2iv(const char *,const int *);
			bool SetUniform3iv(const char *,const int *);
			bool SetUniform4iv(const char *,const int *);

			bool SetUniform1uiv(const char *,const unsigned int *);
			bool SetUniform2uiv(const char *,const unsigned int *);
			bool SetUniform3uiv(const char *,const unsigned int *);
			bool SetUniform4uiv(const char *,const unsigned int *);

			bool SetUniformMatrix2fv(const char *,const float *);
			bool SetUniformMatrix3fv(const char *,const float *);
			bool SetUniformMatrix4fv(const char *,const float *);

			bool SetUniformMatrix2x3fv(const char *,const float *);
			bool SetUniformMatrix3x2fv(const char *,const float *);
			bool SetUniformMatrix2x4fv(const char *,const float *);
			bool SetUniformMatrix4x2fv(const char *,const float *);
			bool SetUniformMatrix3x4fv(const char *,const float *);
			bool SetUniformMatrix4x3fv(const char *,const float *);
		};//class Shader

		/**
		* Shader数据块，用于跨Shader提供数据或是大批量提供数据
		*/
		class ShaderDataBlock																		///Shader数据块
		{
		protected:

			char shader_name[HGL_MAX_SHADER_NAME_LENGTH];											///<数据块在Shader中的名称
			int block_size;																			///<数据块长度

		public:

			ShaderDataBlock(const char *,int);
			virtual ~ShaderDataBlock()HGL_DEFAULT_MEMFUNC;

			virtual void SetData(void *)=0;															///<设置数据块内容
			virtual void ChangeData(void *,int,int)=0;												///<更改数据块内容
		};//class ShaderDataBlock
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SHADER_INCLUDE
