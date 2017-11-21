#include<hgl/graph/UBO.h>
#include<hgl/type/Stack.h>
#include<hgl/graph/GL/glew.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/VertexBuffer.h>

// UBO/SSBO 区别
// 1.UBO位于显卡常量区，所以速度稍快，但容量小。一般为几十几百K。
//   SSBO位于通用显存区，容量不受限制
//
// 2.UBO 在shader中为只读,SSBO可读可写
// 3.任何BUFFER都可以绑到SSBO上操作

namespace hgl
{
    namespace graph
    {
        namespace
        {
            //绑定点是全局不可重用的，所以使用一个全局计数
            static Stack<int> ubo_binding_stack;                                               ///<绑定点计数

            static int max_ubo_binding=0;
            static int max_ubo_size=0;                                                          ///<UBO最大容量，测试了一下，Quadro K600是64k
        }//namespace

        namespace ubo
        {
            bool Init()
            {
                glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,&max_ubo_binding);
                glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&max_ubo_size);

                if(max_ubo_binding<=0)
                    return(false);

                for(int i=0;i<max_ubo_binding;i++)
                    ubo_binding_stack.Push(i);

                return(true);
            }

            int GetMaxBinding()
            {
                return max_ubo_binding;
            }

            int GetMaxSize()
            {
                return max_ubo_size;
            }

            int AcquireBindingPoint()
            {
                int result;

                if(ubo_binding_stack.Pop(result))
                    return result;

                return -1;
            }

            void ReleaseBindingPoint(int bb)
            {
                if(bb==-1)return;

                ubo_binding_stack.Push(bb);
            }
        }//namespace ubo

    #define UBO_VALUE(name,type,C)    class UBO##name:public UBOValue    \
                                    {    \
                                        using UBOValue::UBOValue;    \
                                        \
                                        bool Set##Name(const type *value)    \
                                        {    \
                                            memcpy(this->data,value,C*sizeof(type));    \
                                            return(true);    \
                                        }    \
                                    };

            UBO_VALUE(Float,float,1)
            UBO_VALUE(Float2,float,2)
            UBO_VALUE(Float3,float,3)
            UBO_VALUE(Float4,float,4)

            UBO_VALUE(Integer,int,1)
            UBO_VALUE(Integer2,int,2)
            UBO_VALUE(Integer3,int,3)
            UBO_VALUE(Integer4,int,4)

            UBO_VALUE(UInteger,uint,1)
            UBO_VALUE(UInteger2,uint,2)
            UBO_VALUE(UInteger3,uint,3)
            UBO_VALUE(UInteger4,uint,4)

            UBO_VALUE(Bool,bool,1)
            UBO_VALUE(Bool2,bool,2)
            UBO_VALUE(Bool3,bool,3)
            UBO_VALUE(Bool4,bool,4)

            UBO_VALUE(Mat2,float,4)
            UBO_VALUE(Mat3,float,9)
            UBO_VALUE(Mat4,float,16)

            UBO_VALUE(Mat2x3,float,6)
            UBO_VALUE(Mat2x4,float,8)
            UBO_VALUE(Mat3x2,float,6)
            UBO_VALUE(Mat3x4,float,12)
            UBO_VALUE(Mat4x2,float,8)
            UBO_VALUE(Mat4x3,float,12)
    #undef UBO_VALUE

        GLuint sizeFromUniformType(GLint type)
        {
            #define UNI_CASE(type, numElementsInType, elementType) case type : return(numElementsInType * sizeof(elementType));

            switch(type)
            {
                UNI_CASE(GL_FLOAT,               1, GLfloat);
                UNI_CASE(GL_FLOAT_VEC2,          2, GLfloat);
                UNI_CASE(GL_FLOAT_VEC3,          3, GLfloat);
                UNI_CASE(GL_FLOAT_VEC4,          4, GLfloat);
                UNI_CASE(GL_INT,                 1, GLint);
                UNI_CASE(GL_INT_VEC2,            2, GLint);
                UNI_CASE(GL_INT_VEC3,            3, GLint);
                UNI_CASE(GL_INT_VEC4,            4, GLint);
                UNI_CASE(GL_UNSIGNED_INT,        1, GLuint);
                UNI_CASE(GL_UNSIGNED_INT_VEC2,   2, GLuint);
                UNI_CASE(GL_UNSIGNED_INT_VEC3,   3, GLuint);
                UNI_CASE(GL_UNSIGNED_INT_VEC4,   4, GLuint);
                UNI_CASE(GL_BOOL,                1, GLboolean);
                UNI_CASE(GL_BOOL_VEC2,           2, GLboolean);
                UNI_CASE(GL_BOOL_VEC3,           3, GLboolean);
                UNI_CASE(GL_BOOL_VEC4,           4, GLboolean);
                UNI_CASE(GL_FLOAT_MAT2,          4, GLfloat);
                UNI_CASE(GL_FLOAT_MAT3,          9, GLfloat);
                UNI_CASE(GL_FLOAT_MAT4,         16, GLfloat);
                UNI_CASE(GL_FLOAT_MAT2x3,        6, GLfloat);
                UNI_CASE(GL_FLOAT_MAT2x4,        8, GLfloat);
                UNI_CASE(GL_FLOAT_MAT3x2,        6, GLfloat);
                UNI_CASE(GL_FLOAT_MAT3x4,       12, GLfloat);
                UNI_CASE(GL_FLOAT_MAT4x2,        8, GLfloat);
                UNI_CASE(GL_FLOAT_MAT4x3,       12, GLfloat);
                default : return(0);
            }

            #undef UNI_CASE
        }

        const char *textFromUniformType(GLint type)
        {
            switch(type)
            {
                case GL_FLOAT               : return "GL_FLOAT";
                case GL_FLOAT_VEC2          : return "GL_FLOAT_VEC2";
                case GL_FLOAT_VEC3          : return "GL_FLOAT_VEC3";
                case GL_FLOAT_VEC4          : return "GL_FLOAT_VEC4";
                case GL_INT                 : return "GL_INT";
                case GL_INT_VEC2            : return "GL_INT_VEC2";
                case GL_INT_VEC3            : return "GL_INT_VEC3";
                case GL_INT_VEC4            : return "GL_INT_VEC4";
                case GL_UNSIGNED_INT        : return "GL_UNSIGNED_INT";
                case GL_UNSIGNED_INT_VEC2   : return "GL_UNSIGNED_INT_VEC2";
                case GL_UNSIGNED_INT_VEC3   : return "GL_UNSIGNED_INT_VEC3";
                case GL_UNSIGNED_INT_VEC4   : return "GL_UNSIGNED_INT_VEC4";
                case GL_BOOL                : return "GL_BOOL";
                case GL_BOOL_VEC2           : return "GL_BOOL_VEC2";
                case GL_BOOL_VEC3           : return "GL_BOOL_VEC3";
                case GL_BOOL_VEC4           : return "GL_BOOL_VEC4";
                case GL_FLOAT_MAT2          : return "GL_FLOAT_MAT2";
                case GL_FLOAT_MAT3          : return "GL_FLOAT_MAT3";
                case GL_FLOAT_MAT4          : return "GL_FLOAT_MAT4";
                case GL_FLOAT_MAT2x3        : return "GL_FLOAT_MAT2x3";
                case GL_FLOAT_MAT2x4        : return "GL_FLOAT_MAT2x4";
                case GL_FLOAT_MAT3x2        : return "GL_FLOAT_MAT3x2";
                case GL_FLOAT_MAT3x4        : return "GL_FLOAT_MAT3x4";
                case GL_FLOAT_MAT4x2        : return "GL_FLOAT_MAT4x2";
                case GL_FLOAT_MAT4x3        : return "GL_FLOAT_MAT4x3";
                default                     : return "Unknown";
            }
        }

        UBOValue *CreateUBOValue(GLint type,char *buffer)
        {
            switch(type)
            {
#define CREATE_UBOVALUE(type,uvb)    case GL_##type:return(new UBO##uvb(buffer));

                CREATE_UBOVALUE(FLOAT,              Float)
                CREATE_UBOVALUE(FLOAT_VEC2,         Float2)
                CREATE_UBOVALUE(FLOAT_VEC3,         Float3)
                CREATE_UBOVALUE(FLOAT_VEC4,         Float4)

                CREATE_UBOVALUE(INT,                Integer)
                CREATE_UBOVALUE(INT_VEC2,           Integer2)
                CREATE_UBOVALUE(INT_VEC3,           Integer3)
                CREATE_UBOVALUE(INT_VEC4,           Integer4)

                CREATE_UBOVALUE(UNSIGNED_INT,       UInteger)
                CREATE_UBOVALUE(UNSIGNED_INT_VEC2,  UInteger2)
                CREATE_UBOVALUE(UNSIGNED_INT_VEC3,  UInteger3)
                CREATE_UBOVALUE(UNSIGNED_INT_VEC4,  UInteger4)

                CREATE_UBOVALUE(BOOL,               Bool)
                CREATE_UBOVALUE(BOOL_VEC2,          Bool2)
                CREATE_UBOVALUE(BOOL_VEC3,          Bool3)
                CREATE_UBOVALUE(BOOL_VEC4,          Bool4)

                CREATE_UBOVALUE(FLOAT_MAT2,         Mat2)
                CREATE_UBOVALUE(FLOAT_MAT3,         Mat3)
                CREATE_UBOVALUE(FLOAT_MAT4,         Mat4)
                CREATE_UBOVALUE(FLOAT_MAT2x3,       Mat2x3)
                CREATE_UBOVALUE(FLOAT_MAT2x4,       Mat2x4)
                CREATE_UBOVALUE(FLOAT_MAT3x2,       Mat3x2)
                CREATE_UBOVALUE(FLOAT_MAT3x4,       Mat3x4)
                CREATE_UBOVALUE(FLOAT_MAT4x2,       Mat4x2)
                CREATE_UBOVALUE(FLOAT_MAT4x3,       Mat4x3)

#undef CREATE_UBOVALUE
                default : return nullptr;
            }
        }

        UBO::UBO(const UTF8String &n,uint p,uint i,int bp,uint l)
        {
            block_name=n;
            program=p;
            block_index=i;
            binding_point=bp;
            level=l;

            use_map=0;
            
            glUniformBlockBinding(program,block_index,binding_point);

            glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_DATA_SIZE,&size);
            glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,&uniform_count);

            uniform_indices         =new int[uniform_count];
            uniform_name_size       =new int[uniform_count];
            uniform_name            =new char *[uniform_count];
            uniform_size            =new int[uniform_count];
            uniform_offset          =new int[uniform_count];
            uniform_type            =new int[uniform_count];
            uniform_array_stride    =new int[uniform_count];
            uniform_matrix_stride   =new int[uniform_count];

            glGetActiveUniformBlockiv(program,block_index,GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,uniform_indices);
            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_NAME_LENGTH,uniform_name_size);

            for(int i=0;i<uniform_count;i++)
            {
                uniform_name[i]=new char[uniform_name_size[i]+1];

                glGetActiveUniformName(program,uniform_indices[i],uniform_name_size[i],uniform_name_size+i,uniform_name[i]);

                uniform_name[i][uniform_name_size[i]]=0;
            }

            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_SIZE,          uniform_size            );
            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_OFFSET,        uniform_offset          );
            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_TYPE,          uniform_type            );
            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_ARRAY_STRIDE,  uniform_array_stride    );
            glGetActiveUniformsiv(program,uniform_count,(GLuint *)uniform_indices,GL_UNIFORM_MATRIX_STRIDE, uniform_matrix_stride   );

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
            glDeleteBuffers(1,&ubo);
        }

        /**
         * 只读访问一块数据
         * @param start 起始字节
         * @param access_size 要访问的长度
         */
        void *UBO::ReadMap(int start,int access_size)
        {
            if(use_map!=0)
                return(nullptr);

            use_map=GL_MAP_READ_BIT;

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
        void *UBO::WriteMap(int start,int access_size)
        {
            if(use_map!=0)
                return(nullptr);

            use_map=GL_MAP_WRITE_BIT;

            glBindBufferBase(GL_UNIFORM_BUFFER,binding_point,ubo);

            if(start==0&&access_size==0)
                access_size=size;

            map_start=start;
            map_size=access_size;

            return glMapNamedBufferRange(ubo,map_start,map_size,GL_MAP_WRITE_BIT|GL_MAP_FLUSH_EXPLICIT_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
        }

        void UBO::Unmap()
        {
            if(use_map==GL_MAP_WRITE_BIT)
                glFlushMappedNamedBufferRange(ubo,map_start,map_size);

            use_map=0;
            glUnmapNamedBuffer(ubo);
        }

        UBOValue *UBO::GetValue(const UTF8String &value_name)
        {
            if(!ubo)return(nullptr);

            UBOValue *uv=ubo_value[value_name];

            if(uv)
                return uv;

            for(int i=0;i<uniform_count;i++)
                if(value_name.Comp(uniform_name[i])==0)
                {
                    uv=CreateUBOValue(uniform_type[i],buffer+uniform_offset[i]);

                    if(uv)
                    {
                        ubo_value.Add(value_name,uv);
                        return uv;
                    }

                    return nullptr;
                }

            return(nullptr);
        }

        void UBO::Commit()
        {
            glNamedBufferData(ubo,size,buffer,level);
        }
    }//namespace graph
}//namespace hgl
