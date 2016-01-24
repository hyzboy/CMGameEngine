#ifndef HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE
#define HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
		/**
		 * UBO变量映射
		 */
		struct UBOValue
		{
			int indices;
			int size;
			int offset;
			int type;
		};//struct UBOValue

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

			void *ReadMap(uint start=0,uint access_size=0);
			void *WriteMap(uint start=0,uint access_size=0);
			void Unmap();
		};//class UBO
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_OPENGL_CORE_UNIFORM_BUFFER_OBJECT_INCLUDE
