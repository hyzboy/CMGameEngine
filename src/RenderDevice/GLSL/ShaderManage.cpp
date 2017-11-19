#include<hgl/graph/ShaderManage.h>
#include<hgl/type/Smart.h>

namespace hgl
{
    namespace graph
    {
#ifdef _DEBUG
        char *MakeVertexShader(const RenderState *,const OSString &save_filename=nullptr);
        char *MakeFragmentShader(const RenderState *,const OSString &save_filename=nullptr);
		char *MakeGeometryShader(const RenderState *,const OSString &save_filename=nullptr);
#else
        char *MakeVertexShader(bool,const RenderState *);
        char *MakeFragmentShader(const RenderState *);
		char *MakeGeometryShader(const RenderState *);
#endif//_DEBUG

        Shader *CreateShader(    const char *vertex_shader,
                                const char *control_shader,
                                const char *evaluation_shader,
                                const char *geometry_shader,
                                const char *fragment_shader)
        {
            if(!vertex_shader||!fragment_shader)return(nullptr);            //opengl core时代不可能没有vs/fs

            Shader *shader=new Shader;

            if(vertex_shader    ){if(!shader->AddShader(stVertex,     vertex_shader       )){delete shader;return(nullptr);}}
            if(control_shader   ){if(!shader->AddShader(stTessControl,control_shader      )){delete shader;return(nullptr);}}
            if(evaluation_shader){if(!shader->AddShader(stTessEval,   evaluation_shader   )){delete shader;return(nullptr);}}
            if(geometry_shader  ){if(!shader->AddShader(stGeometry,   geometry_shader     )){delete shader;return(nullptr);}}
            if(fragment_shader  ){if(!shader->AddShader(stFragment,   fragment_shader     )){delete shader;return(nullptr);}}

            if(shader->Build())
                return(shader);

            delete shader;
            return(nullptr);
        }

#ifdef _DEBUG
        Shader *CreateShader(const RenderState *state,const OSString &save_filename)
#else
        Shader *CreateShader(const RenderState *state)
#endif//_DEBUG
        {
			SharedArray<char> vs,fs,gs;

#ifdef _DEBUG
            if(save_filename)
            {
                vs=MakeVertexShader(state,save_filename);
                fs=MakeFragmentShader(state,save_filename);
				gs=MakeGeometryShader(state,save_filename);
            }
            else
#endif//_DEBUG
			{
                vs=MakeVertexShader(state);
                fs=MakeFragmentShader(state);
				gs=MakeGeometryShader(state);
            }

            Shader *shader=nullptr;

            if(vs&&fs)
            {
                shader=new Shader;

                if(shader->AddVertexShader(vs))
					if(shader->AddFragmentShader(fs))
					{
						if((!gs)									//没有gs
				 		 ||(gs&&shader->AddGeometryShader(gs)))		//有gs并且编译成功
						{
							if(shader->Build())
								return shader;
						}
					}
            }

			if(shader)
				delete shader;

            return(nullptr);
        }

        Shader *ShaderManage::Create(const RenderState &state)
        {
#ifdef _DEBUG
            return CreateShader(&state,nullptr);
#else
            return CreateShader(&state);
#endif//_DEBUG
        }

        ShaderManage *global_shader_manage=nullptr;

        void InitGlobalShaderStorage()
        {
            global_shader_manage=new ShaderManage;
        }

        void ClearGlobalShaderStorage()
        {
            SAFE_CLEAR(global_shader_manage);
        }
    }//namespace graph
}//namespace hgl
