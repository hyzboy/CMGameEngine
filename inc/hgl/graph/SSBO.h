#ifndef HGL_GRAPH_OPENGL_CORE_SHADER_STORAGE_BUFFER_OBJECT_INCLUDE
#define HGL_GRAPH_OPENGL_CORE_SHADER_STORAGE_BUFFER_OBJECT_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/BaseString.h>
#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
	namespace graph
	{
		/**
		* OpenGL Core Shader Storage Buffer Object
		*/
		class SSBO
		{
			friend class Shader;

			SSBO(const UTF8String &,uint,uint,uint);

		protected:

			UTF8String block_name;										///<数据块名称

			uint ssbo;
			int level;
			int size;
			uint program;
			uint block_index;
			uint binding_point;

			int use_map;
			int map_start;
			int map_size;

			void *buffer;

		public:

			~SSBO();

			int			GetSize		()const{return size;}				///<取得当前UBO尺寸大小

			bool		Commit		(void *,int);						///<提交数据到显存

			void *		ReadMap		(int start=0,int access_size=0);
			void *		WriteMap	(int start=0,int access_size=0);
			void		Unmap		();
		};//class SSBO
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_OPENGL_CORE_SHADER_STORAGE_BUFFER_OBJECT_INCLUDE
