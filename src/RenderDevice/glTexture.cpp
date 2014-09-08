#include<hgl/graph/Render.h>
#include<hgl/graph/Texture.h>
#include<GL/glew.h>

namespace hgl
{
	namespace graph
	{
		//贴图数量
			  int	HGL_MAX_TEXTURE_UNITS			=1;							//最大贴图数量
			  int	HGL_MAX_TEXTURE_SIZE			=256;						//最大贴图尺寸
			  int	HGL_MAX_CUBE_MAP_TEXTURE_SIZE	=256;						//最大CubeMap贴图尺寸

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

			if(opengl_version>=4.5)
			{
				glBindTextureUnit(active,index);						//注：使用数字的纹理号active，而不是GL_TEXTURE0+active
			}
			else
			{
				if(current_active_texture!=active)						//如果活动贴图编号不一致
				{
					glActiveTexture(GL_TEXTURE0+active);				//切换活动贴图

					current_active_texture=active;
				}

				if(texture_state_format[active]!=type)					//如果格式和之前不一样
				{
					if(texture_state_format[active])					//如果之前有另一个格式
						glDisable(texture_state_format[active]);		//解除旧的贴图格式

					glEnable(type);										//开启新的格式
					texture_state_format[active]=type;
				}

				glBindTexture(type,index);
			}
			
			texture_index[active]=index;
			
			
			return(true);
		}

		/**
		* 关闭贴图
		* @param active 活动贴图编号
		*/
		bool DisableTexture(int active)
		{
			if(active<0||active>=HGL_MAX_TEXTURE_UNITS)return(false);

			if(texture_state_format[active]==0)return(true);		//本身就没开启

			if(current_active_texture!=active)						//如果活动贴图编号不一致
			{
				glActiveTexture(GL_TEXTURE0+active);				//切换活动贴图

				current_active_texture=active;
			}

			glDisable(texture_state_format[active]);
			texture_state_format[active]=0;

			return(true);
		}

		namespace OpenGLCore
		{
			void InitTexture()
			{
				int v,f,g;

				glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&v);		//vertex shader 可用最大数量
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&f);				//fragment shader 可用最大数量
				glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,&g);		//geometry shader 可用最大数量,OpenGL 3.2

				HGL_MAX_TEXTURE_UNITS=f>v?v:f;

				texture_state_format=new uint[HGL_MAX_TEXTURE_UNITS];
				texture_index=new uint[HGL_MAX_TEXTURE_UNITS];

				InitStateTexture(HGL_MAX_TEXTURE_UNITS);										//初始化状态机

				glGetIntegerv(GL_MAX_TEXTURE_SIZE,			&HGL_MAX_TEXTURE_SIZE);				//最大贴图尺寸
				glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,	&HGL_MAX_CUBE_MAP_TEXTURE_SIZE);	//最大CubeMap贴图尺寸

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
