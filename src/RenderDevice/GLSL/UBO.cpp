#include<hgl/graph/UBO.h>
#include<hgl/type/Stack.h>
#include<glew/include/GL/glew.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/VertexBuffer.h>

//注：较大数据请使用SSBO

namespace hgl
{
	namespace graph
	{
        //绑定点是全局不可重用的，所以使用一个全局计数
        static Stack<uint> uniform_block_binding_stack;                                               ///<绑定点计数

        static int max_uniform_block_binding=0;
        static int max_uniform_block_size=0;

        bool InitUBO()
        {
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,&max_uniform_block_binding);
            glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&max_uniform_block_size);

            if(max_uniform_block_binding<=0)
                return(false);

            for(uint i=0;i<max_uniform_block_binding;i++)
                uniform_block_binding_stack.Push(i);

            return(true);
        }

        uint GetMaxShaderBlockBinding()
        {
            return max_uniform_block_binding;
        }

        uint GetMaxShaderBlockSize()
        {
            return max_uniform_block_size;
        }

        uint AcquireShaderBlockBinding()
        {
            uint result;

            if(uniform_block_binding_stack.Pop(result))
                return result;

            return 0;
        }

        void ReleaseShaderBlockBinding(uint bb)
        {
            if(bb==0)return;

            uniform_block_binding_stack.Push(bb);
        }

        GLuint sizeFromUniformType(GLint type)
		{
			#define UNI_CASE(type, numElementsInType, elementType) case type : return(numElementsInType * sizeof(elementType));

			switch(type)
			{
				UNI_CASE(GL_FLOAT, 1, GLfloat);
				UNI_CASE(GL_FLOAT_VEC2, 2, GLfloat);
				UNI_CASE(GL_FLOAT_VEC3, 3, GLfloat);
				UNI_CASE(GL_FLOAT_VEC4, 4, GLfloat);
				UNI_CASE(GL_INT, 1, GLint);
				UNI_CASE(GL_INT_VEC2, 2, GLint);
				UNI_CASE(GL_INT_VEC3, 3, GLint);
				UNI_CASE(GL_INT_VEC4, 4, GLint);
				UNI_CASE(GL_UNSIGNED_INT, 1, GLuint);
				UNI_CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
				UNI_CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
				UNI_CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
				UNI_CASE(GL_BOOL, 1, GLboolean);
				UNI_CASE(GL_BOOL_VEC2, 2, GLboolean);
				UNI_CASE(GL_BOOL_VEC3, 3, GLboolean);
				UNI_CASE(GL_BOOL_VEC4, 4, GLboolean);
				UNI_CASE(GL_FLOAT_MAT2, 4, GLfloat);
				UNI_CASE(GL_FLOAT_MAT3, 9, GLfloat);
				UNI_CASE(GL_FLOAT_MAT4, 16, GLfloat);
				UNI_CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
				UNI_CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
				UNI_CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
				UNI_CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
				UNI_CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
				UNI_CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
				default : return(0);
			}

			#undef UNI_CASE
		}

		const char *textFromUniformType(GLint type)
		{
			switch(type)
			{
				case GL_FLOAT : return "GL_FLOAT";
				case GL_FLOAT_VEC2 : return "GL_FLOAT_VEC2";
				case GL_FLOAT_VEC3 : return "GL_FLOAT_VEC3";
				case GL_FLOAT_VEC4 : return "GL_FLOAT_VEC4";
				case GL_INT : return "GL_INT";
				case GL_INT_VEC2 : return "GL_INT_VEC2";
				case GL_INT_VEC3 : return "GL_INT_VEC3";
				case GL_INT_VEC4 : return "GL_INT_VEC4";
				case GL_UNSIGNED_INT : return "GL_UNSIGNED_INT";
				case GL_UNSIGNED_INT_VEC2 : return "GL_UNSIGNED_INT_VEC2";
				case GL_UNSIGNED_INT_VEC3 : return "GL_UNSIGNED_INT_VEC3";
				case GL_UNSIGNED_INT_VEC4 : return "GL_UNSIGNED_INT_VEC4";
				case GL_BOOL : return "GL_BOOL";
				case GL_BOOL_VEC2 : return "GL_BOOL_VEC2";
				case GL_BOOL_VEC3 : return "GL_BOOL_VEC3";
				case GL_BOOL_VEC4 : return "GL_BOOL_VEC4";
				case GL_FLOAT_MAT2 : return "GL_FLOAT_MAT2";
				case GL_FLOAT_MAT3 : return "GL_FLOAT_MAT3";
				case GL_FLOAT_MAT4 : return "GL_FLOAT_MAT4";
				case GL_FLOAT_MAT2x3 : return "GL_FLOAT_MAT2x3";
				case GL_FLOAT_MAT2x4 : return "GL_FLOAT_MAT2x4";
				case GL_FLOAT_MAT3x2 : return "GL_FLOAT_MAT3x2";
				case GL_FLOAT_MAT3x4 : return "GL_FLOAT_MAT3x4";
				case GL_FLOAT_MAT4x2 : return "GL_FLOAT_MAT4x2";
				case GL_FLOAT_MAT4x3 : return "GL_FLOAT_MAT4x3";
				default : return "Unknown";
			}
		}

		UBO::UBO(const UTF8String &n,uint p,uint i,uint level)
		{
			block_name=n;
			program=p;
			block_index=i;
			binding_point=AcquireShaderBlockBinding();

			glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_DATA_SIZE,&size);
            glUniformBlockBinding(program,block_index,binding_point);

			glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,&uniform_count);

			uniform_indices		=new int[uniform_count];
			uniform_name_size	=new int[uniform_count];
			uniform_name		=new char *[uniform_count];
			uniform_size		=new int[uniform_count];
			uniform_offset		=new int[uniform_count];
			uniform_type		=new int[uniform_count];
			uniform_array_stride=new int[uniform_count];
			uniform_matrix_stride=new int[uniform_count];

			glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,uniform_indices);
			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_NAME_LENGTH,uniform_name_size);

			for(int i=0;i<uniform_count;i++)
			{
				uniform_name[i]=new char[uniform_name_size[i]+1];

				glGetActiveUniformName(program,uniform_indices[i],uniform_name_size[i],uniform_name_size+i,uniform_name[i]);

				uniform_name[i][uniform_name_size[i]]=0;
			}

			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_SIZE,			uniform_size			);
			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_OFFSET,		uniform_offset			);
			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_TYPE,			uniform_type			);
			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_ARRAY_STRIDE,	uniform_array_stride	);
			glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_MATRIX_STRIDE,	uniform_matrix_stride	);

#ifdef _DEBUG
			LOG_INFO("uniform block:"+n);
			for(int i=0;i<uniform_count;i++)
			{
				LOG_INFO("\t        index: "+UTF8String(i));
				LOG_INFO("\t      indices: "+UTF8String(uniform_indices[i]));
				LOG_INFO("\t         name: "+UTF8String(uniform_name[i]));
				LOG_INFO("\t         size: "+UTF8String(uniform_size[i]));
				LOG_INFO("\t         type: "+UTF8String(textFromUniformType(uniform_type[i])));
				LOG_INFO("\t        bytes: "+UTF8String(sizeFromUniformType(uniform_type[i])*uniform_size[i]));
				LOG_INFO("\t       offset: "+UTF8String(uniform_offset[i]));
				LOG_INFO("\t array_stride: "+UTF8String(uniform_array_stride[i]));
				LOG_INFO("\tmatrix_stride: "+UTF8String(uniform_matrix_stride[i]));
			}
#endif//_DEBUG

			{
				glCreateBuffers(1,&ubo);
				glBindBuffer(GL_UNIFORM_BUFFER,ubo);

				glNamedBufferData(ubo,size,nullptr,level);

				buffer=new char[size];
				memset(buffer,0,size);
			}
		}

		UBO::~UBO()
		{
			delete[] uniform_matrix_stride;
			delete[] uniform_array_stride;
			delete[] uniform_type;
			delete[] uniform_offset;
			delete[] uniform_size;
			for(int i=0;i<uniform_count;i++)delete[] uniform_name[i];
			delete[] uniform_name;
			delete[] uniform_name_size;
			delete[] uniform_indices;
			delete[] buffer;
			ReleaseShaderBlockBinding(binding_point);
			glDeleteBuffers(1,&ubo);
		}

		/**
		 * 只读访问一块数据
		 * @param start 起始字节
		 * @param access_size 要访问的长度
		 */
		void *UBO::ReadMap(uint start,uint access_size)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER,binding_point,ubo);

			if(start==0&&access_size==0)
				access_size=size;

			return glMapNamedBufferRange(ubo,0,size,GL_MAP_READ_BIT);
		}

		/**
		 * 只写访问一块数据
		 * @param start 起始字节
		 * @param access_size 要访问的长度
		 */
		void *UBO::WriteMap(uint start,uint access_size)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER,binding_point,ubo);

			if(start==0&&access_size==0)
				access_size=size;

			return glMapNamedBufferRange(ubo,0,size,GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
		}

		void UBO::Unmap()
		{
			glUnmapNamedBuffer(ubo);
		}
	}//namespace graph
}//namespace hgl
