#include<hgl/graph/Render.h>
#include<hgl/graph/Texture.h>
#include<glew/include/GL/glew.h>

namespace hgl
{
	namespace graph
	{
		//贴图数量
        int HGL_MAX_TEXTURE_UNITS	=0;		//最大贴图数量

        int HGL_MAX_VS_TEXTURE_UNITS =0;     ///<Vertex Shader中的最大纹理单元
        int HGL_MAX_FS_TEXTURE_UNITS =0;     ///<Fragment Shader中的最大纹理单元
        int HGL_MAX_GS_TEXTURE_UNITS =0;     ///<Geometry Shader中的最大纹理单元
        int HGL_MAX_TCS_TEXTURE_UNITS=0;     ///<Tess Control Shader中的最大纹理单元
        int HGL_MAX_TES_TEXTURE_UNITS=0;     ///<Tess Evaluation Shader中的最大纹理单元
        int HGL_MAX_CS_TEXTURE_UNITS =0;     ///<Compute Shader中的最大纹理单元

        int HGL_MAX_TEXTURE_SIZE     =0;     ///<最大纹理尺寸
        int HGL_MAX_CUBE_MAP_SIZE    =0;     ///<最大CubeMap纹理尺寸

		namespace
		{
			static int current_active_texture=0;
			static uint *texture_state_format=0;
			static uint *texture_index=0;

			const GLenum tex_format_list[]=
			{
				GL_TEXTURE_1D,
				GL_TEXTURE_2D,
				GL_TEXTURE_3D,
				GL_TEXTURE_1D_ARRAY,
				GL_TEXTURE_2D_ARRAY,
				GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_CUBE_MAP_ARRAY
			};

			const GLenum tex_format_bind_list[]=
			{
				GL_TEXTURE_BINDING_1D,
				GL_TEXTURE_BINDING_2D,
				GL_TEXTURE_BINDING_3D,
				GL_TEXTURE_BINDING_1D_ARRAY,
				GL_TEXTURE_BINDING_2D_ARRAY,
				GL_TEXTURE_BINDING_CUBE_MAP,
				GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
			};

			const int tex_format_count=sizeof(tex_format_list)/sizeof(GLenum);

			void InitStateTexture(int max_num)
			{
				glGetIntegerv(GL_ACTIVE_TEXTURE,&current_active_texture);		//取得活动贴图

				memset(texture_state_format,0,sizeof(uint)*max_num);
				memset(texture_index,0,sizeof(uint)*max_num);

				for(int i=0;i<max_num;i++)
				{
					for(int j=0;j<tex_format_count;j++)
					{
						int value=0;

						glGetIntegerv(tex_format_bind_list[j],&value);

						if(!value)
							continue;

						texture_state_format[i]=tex_format_list[j];				//如果是绑定了这种贴图
						texture_index[i]=value;
						break;
					}
				}
			}
		}//namespace

		/**
		* 绑定贴图
		* @param active 活动贴图编号
		* @param type 贴图类型
		* @param index 贴图编号
		* @return 是否成功
		*/
		bool BindTexture(int active,unsigned int type,unsigned int index)
		{
			if(active<0||active>=HGL_MAX_TEXTURE_UNITS)return(false);

			if(type	==texture_state_format[active]					//本身就是这个格式
			 &&index==texture_index[active])						//也绑定的是这个贴图
					return(true);

#ifdef HGL_OPENGL_USE_DSA
				glBindTextureUnit(active,index);						//注：使用数字的纹理号active，而不是GL_TEXTURE0+active
#else
				if(current_active_texture!=active)						//如果活动贴图编号不一致
				{
					glActiveTexture(GL_TEXTURE0+active);				//切换活动贴图

					current_active_texture=active;
				}

				if(texture_state_format[active]!=type)					//如果格式和之前不一样
                    texture_state_format[active]=type;

				glBindTexture(type,index);
#endif//HGL_OPENGL_USE_DSA

			texture_index[active]=index;

			return(true);
		}

		bool BindTextures(int first_active,int count,unsigned int *texture_index)
        {
            if(count<=0||!texture_index)
                return(false);

            glBindTextures(first_active,count,texture_index);
            return(true);
        }

		namespace OpenGLCore
		{
			void InitTexture()
			{

                glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,            &HGL_MAX_VS_TEXTURE_UNITS);         //vertex shader 可用最大纹理数量
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,                   &HGL_MAX_FS_TEXTURE_UNITS);         //fragment shader 可用最大纹理数量
                glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,          &HGL_MAX_GS_TEXTURE_UNITS);         //geometry shader 可用最大纹理数量,OpenGL 3.2

				if(GLEW_VERSION_4_0)
				{
					glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS,      &HGL_MAX_TCS_TEXTURE_UNITS);        //tess control shader 可用最大纹理数量,OpenGL 4
					glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS,   &HGL_MAX_TES_TEXTURE_UNITS);        //tess evaluation shader 可用最大纹理数量,OpenGL 4

					if(GLEW_VERSION_4_3)
					glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS,           &HGL_MAX_CS_TEXTURE_UNITS);         //compute shader 可用最大纹理数量,OpenGL 4.3
				}

                glGetIntegerv(GL_MAX_TEXTURE_SIZE,                          &HGL_MAX_TEXTURE_SIZE);             //最大贴图尺寸
                glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,                 &HGL_MAX_CUBE_MAP_SIZE);            //最大CubeMap贴图尺寸

				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,          &HGL_MAX_TEXTURE_UNITS);

				texture_state_format=new uint[HGL_MAX_TEXTURE_UNITS];
				texture_index=new uint[HGL_MAX_TEXTURE_UNITS];

				InitStateTexture(HGL_MAX_TEXTURE_UNITS);										//初始化状态机

				glHint(GL_TEXTURE_COMPRESSION_HINT,GL_NICEST);									//压缩贴图时用最佳质量
			}

			void CloseTexture()
			{
				SAFE_CLEAR_ARRAY(texture_state_format);
				SAFE_CLEAR_ARRAY(texture_index);
			}
		}//namespace OpenGLCore
	}//namespace graph
}//namespace hgl
