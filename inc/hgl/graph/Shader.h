#ifndef HGL_GRAPH_SHADER_INCLUDE
#define HGL_GRAPH_SHADER_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
#include<hgl/algorithm/VectorMath.h>
#include<hgl/graph/UBO.h>
#include<hgl/graph/VertexBufferType.h>
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
            unsigned int    program;

            unsigned int    shader_index[ShaderType::stEnd];

        private:

            bool Link();                                                                                                ///<连接使用当前着色程序

		protected:

			Map<UTF8String,int> attrib_location;
			Map<UTF8String,int> uniform_location;
            Map<UTF8String,int> uniform_block_index;
			MapObject<UTF8String,UBO> uniform_block_object;

			int _GetAttribLocation(const char *);																        ///<取得指定属性地址
			int _GetUniformLocation(const char *);															            ///<取得一个变量的地址
			int _GetUniformBlockIndex(const char *);                                                                    ///<取得一个数据块的地址索引

		public:

			Shader(){program=0;hgl_zero(shader_index,ShaderType::stEnd);}
			~Shader(){Clear();}

			void Clear();																						        ///<清除着色程序

			bool AddShader          (const int shader_type,const char *);										        ///<增加一个着色程序

            bool AddVertexShader	(const char *code){return AddShader(ShaderType::stVertex,code);}			        ///<增加一个顶点着色程序
            bool AddGeometryShader	(const char *code){return AddShader(ShaderType::stGeometry,code);}			        ///<增加一个几何着色程序
            bool AddFragmentShader	(const char *code){return AddShader(ShaderType::stFragment,code);}			        ///<增加一个片断着色程序
            bool AddComputeShader	(const char *code){return AddShader(ShaderType::stFragment,code);}			        ///<增加一个计算着色程序

            bool AddTessShader      (const char *tess_control_shader,const char *tess_evaluation_shader)		        ///<增加一个镶嵌着色程序
            {
                if(!AddShader(ShaderType::stTessControl,tess_control_shader		))return(false);
                if(!AddShader(ShaderType::stTessEval,	tess_evaluation_shader	))return(false);
                return(true);
            }

			bool Build();																						        ///<构建当前添加的着色程序

			bool Use();																						            ///<使用当前着色程序

			int GetAttribLocation(const char *);																        ///<取得指定属性地址
			int GetUniformLocation(const char *);																        ///<取得一个变量的地址
            int GetUniformBlockIndex(const char *);                                                                     ///<取得一个数据块索引

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

            bool SetUniform2fv(int index,const Vector2f &v){return SetUniform2fv(index,(const float *)&v);}
            bool SetUniform3fv(int index,const Vector3f &v){return SetUniform3fv(index,(const float *)&v);}
            bool SetUniform4fv(int index,const Vector4f &v){return SetUniform4fv(index,(const float *)&v);}

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

			bool SetUniform2fv(const char *name,const Vector2f &v){return SetUniform2fv(name,(const float *)&v);}
			bool SetUniform3fv(const char *name,const Vector3f &v){return SetUniform3fv(name,(const float *)&v);}
			bool SetUniform4fv(const char *name,const Vector4f &v){return SetUniform4fv(name,(const float *)&v);}

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

            bool SetUniformMatrix3fv(const char *name,const Matrix3f &m){return SetUniformMatrix3fv(name,(const float *)&m);}
			bool SetUniformMatrix4fv(const char *name,const Matrix4f &m){return SetUniformMatrix4fv(name,(const float *)&m);}

			bool SetUniformMatrix2x3fv(const char *,const float *);
			bool SetUniformMatrix3x2fv(const char *,const float *);
			bool SetUniformMatrix2x4fv(const char *,const float *);
			bool SetUniformMatrix4x2fv(const char *,const float *);
			bool SetUniformMatrix3x4fv(const char *,const float *);
			bool SetUniformMatrix4x3fv(const char *,const float *);

        public: //Uniform Block

            UBO *GetUniformBlock(const char *,uint=HGL_DYNAMIC_DRAW);
		};//class Shader
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SHADER_INCLUDE
