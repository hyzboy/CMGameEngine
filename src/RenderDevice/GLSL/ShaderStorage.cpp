#include<hgl/graph/ShaderStorage.h>

namespace hgl
{
    namespace graph
    {
#ifdef _DEBUG
        char *MakeVertexShader(const RenderState *,const os_char *save_filename=nullptr);
        char *MakeFragmentShader(const RenderState *,const os_char *save_filename=nullptr);
#else
        char *MakeVertexShader(bool,const RenderState *);
        char *MakeFragmentShader(const RenderState *);
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
        Shader *CreateShader(const RenderState *state,const os_char *save_filename)
        {
            char *vs,*fs;

            if(save_filename)
            {
                os_char vs_filename[HGL_MAX_PATH];
                os_char fs_filename[HGL_MAX_PATH];

                strcpy(vs_filename,HGL_MAX_PATH,save_filename);strcat(vs_filename,HGL_MAX_PATH,OS_TEXT(".vs.glsl"),8);
                strcpy(fs_filename,HGL_MAX_PATH,save_filename);strcat(fs_filename,HGL_MAX_PATH,OS_TEXT(".fs.glsl"),8);

                vs=MakeVertexShader(state,vs_filename);
                fs=MakeFragmentShader(state,fs_filename);
            }
            else
            {
                vs=MakeVertexShader(state);
                fs=MakeFragmentShader(state);
            }
#else
        Shader *CreateShader(const RenderState *state)
        {
            char *vs=MakeVertexShader(state);
            char *fs=MakeFragmentShader(state);
#endif//_DEBUG

            Shader *shader=nullptr;

            if(vs&&fs)
            {
                shader=new Shader;

                if(!shader->AddVertexShader(vs)
                 ||!shader->AddFragmentShader(fs)
                 ||!shader->Build())
                {
                    delete shader;
                    shader=nullptr;
                }
            }

            delete[] vs;        //delete[] nullptr不是个错误，所以不用检测
            delete[] fs;

            return(shader);
        }

        Shader *ShaderStorage::Create(const RenderState &state)
        {
#ifdef _DEBUG
            return CreateShader(&state,nullptr);
#else
            return CreateShader(&state);
#endif//_DEBUG
        }

        ShaderStorage *global_shader_storage=nullptr;

        void InitGlobalShaderStorage()
        {
            global_shader_storage=new ShaderStorage;
        }

        void ClearGlobalShaderStorage()
        {
            SAFE_CLEAR(global_shader_storage);
        }
    }//namespace graph
}//namespace hgl
