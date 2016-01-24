#ifndef HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE
#define HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/BaseString.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * UBO变量映射
		 */
		class UBOValue
		{
		protected:

			void *data;

		public:

			UBOValue(void *p)
			{
				data=p;
			}

			virtual bool SetFloat		(const float *){return(false);}
			virtual bool SetFloat2		(const float *){return(false);}
			virtual bool SetFloat3		(const float *){return(false);}
			virtual bool SetFloat4		(const float *){return(false);}

			virtual bool SetInteger		(const int *){return(false);}
			virtual bool SetInteger2	(const int *){return(false);}
			virtual bool SetInteger3	(const int *){return(false);}
			virtual bool SetInteger4	(const int *){return(false);}

			virtual bool SetUInteger 	(const uint *){return(false);}
			virtual bool SetUInteger2	(const uint *){return(false);}
			virtual bool SetUInteger3	(const uint *){return(false);}
			virtual bool SetUInteger4	(const uint *){return(false);}

			virtual bool SetBool 		(const bool	*){return(false);}
			virtual bool SetBool2		(const bool *){return(false);}
			virtual bool SetBool3		(const bool *){return(false);}
			virtual bool SetBool4		(const bool *){return(false);}

			virtual bool SetMat2		(const float *){return(false);}
			virtual bool SetMat3		(const float *){return(false);}
			virtual bool SetMat4		(const float *){return(false);}

			virtual bool SetMat2x3		(const float *){return(false);}
			virtual bool SetMat2x4		(const float *){return(false);}
			virtual bool SetMat3x2		(const float *){return(false);}
			virtual bool SetMat3x4		(const float *){return(false);}
			virtual bool SetMat4x2		(const float *){return(false);}
			virtual bool SetMat4x3		(const float *){return(false);}
		};//class UBOValue

		/**
		* OpenGL Core Uniform Buffer Object
		*/
		class UBO
		{
			friend class Shader;

			UBO(const UTF8String &,uint,uint,uint);

		protected:

			UTF8String block_name;										///<数据块名称

			uint ubo;
			int level;
			int size;
			uint program;
			uint block_index;
			uint binding_point;

			char *buffer;

			int uniform_count;
			int *uniform_indices;
			int *uniform_name_size;
			char **uniform_name;
			int *uniform_size;
			int *uniform_offset;
			int *uniform_type;
			int *uniform_array_stride;
			int *uniform_matrix_stride;

			MapObject<UTF8String,UBOValue> ubo_value;					///<变量映射

		public:

			~UBO();

			uint GetSize()const{return size;}							///<取得当前UBO尺寸大小

			UBOValue *GetValue(const UTF8String &);						///<取得某个变量的

			void Commit();												///<提交数据到显存

			void *ReadMap(uint start=0,uint access_size=0);
			void *WriteMap(uint start=0,uint access_size=0);
			void Unmap();
		};//class UBO
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE
