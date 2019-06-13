#include<hgl/graph/Renderable.h>
#include<hgl/graph/ShaderManage.h>
#include"ShaderDefine.h"
#include<hgl/type/Smart.h>
#include<hgl/type/Map.h>
#include<hgl/graph/GL/glew.h>

namespace hgl
{
    namespace graph
    {
        const uint OpenGLCorePrim[]=
        {
            GL_POINTS,
            GL_LINES,
            GL_LINE_STRIP,
            GL_LINE_LOOP,
            GL_TRIANGLES,
            GL_TRIANGLE_STRIP,
            GL_TRIANGLE_FAN,
            GL_LINES_ADJACENCY,
            GL_LINE_STRIP_ADJACENCY,
            GL_TRIANGLES_ADJACENCY,
            GL_TRIANGLE_STRIP_ADJACENCY,
            GL_PATCHES,

            HGL_PRIM_RECTANGLE
        };//const uint OpenGLCorePrim[]

        constexpr int OpenGLCorePrimCount=sizeof(OpenGLCorePrim)/sizeof(uint);

        bool CheckPrim(uint prim)
        {
            const uint *p=OpenGLCorePrim;

            for(int i=0;i<OpenGLCorePrimCount;i++)
                if(*p++==prim)return(true);

            return(false);
        }
    }//namespace graph

    namespace graph
    {
        void Renderable::Init()
        {
            glCreateVertexArrays(1,&vao);

            location=new int[HGL_MAX_VERTEX_ATTRIBS];

            hgl_set(location,-1,HGL_MAX_VERTEX_ATTRIBS);
        }

        void Renderable::Clear()
        {
            delete[] location;

            glDeleteVertexArrays(1,&vao);
        }

        /**
        * 设置缓冲区对应的shader变量
        * @param vbt 顶点缓冲区类型
        * @param shader_locaiton shader变量地址
        * @param enabled 是否立即启用
        */
        bool Renderable::SetShaderLocation(VertexBufferType vbt,unsigned int shader_location)
        {
            if(vbt<vbtVertex||vbt>=HGL_MAX_VERTEX_ATTRIBS)
            {
                LOG_ERROR(OS_TEXT("Renderable::SetShaderLocation设置的数据类型错误：")+OSString(vbt));
                return(false);
            }

            location[vbt]=shader_location;

            return(true);
        }

        void Renderable::ClearShaderLocation()
        {
            for(int i=vbtIndex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
            {
                if(location[i]==-1)continue;

                glDisableVertexArrayAttrib(vao,location[i]);
                location[i]=-1;
            }
        }

        bool Renderable::Bind(Shader *new_shader)
        {
            if(!vao)return(false);

            int binding_index=0;            //这个值必须紧密递增，不能跳着用

            for(int i=vbtVertex;i<HGL_MAX_VERTEX_ATTRIBS;i++)
            {
                if(location[i]==-1)continue;

                VertexBufferBase *vb=va->GetVertexBuffer((VertexBufferType)i);

                if(!vb)
                {
                    LOG_ERROR(OS_TEXT("Shader Location <")+OSString(i)+OS_TEXT(">要求的缓冲区没有数据"));
                    return(false);
                }

                glVertexArrayAttribBinding(vao,                                             //vao obj
                                           location[i],                                     //attrib index
                                           binding_index);                                  //binding index

                if(vb->GetDataType()==HGL_INT   )   glVertexArrayAttribIFormat( vao,location[i],vb->GetComponent(),vb->GetDataType(),0);else
                if(vb->GetDataType()==HGL_DOUBLE)   glVertexArrayAttribLFormat( vao,location[i],vb->GetComponent(),vb->GetDataType(),0);else
                                                    glVertexArrayAttribFormat(  vao,                     //vao obj
                                                                                location[i],             //attrib index
                                                                                vb->GetComponent(),      //size
                                                                                vb->GetDataType(),       //type
                                                                                GL_FALSE,                //normalized
                                                                                0);                      //relative offset

                glEnableVertexArrayAttrib(vao,                            //vao obj
                                          location[i]);                   //attrib index

                glVertexArrayVertexBuffer(vao,                            //vao obj
                                          binding_index,                  //binding index
                                          vb->GetBufferIndex(),           //buffer
                                          0,                              //offset
                                          vb->GetStride());               //stride

                ++binding_index;
            }

            VertexBufferBase *vb_index=va->GetVertexBuffer(vbtIndex);

            if(vb_index)
                 glVertexArrayElementBuffer(vao,                          //vao obj
                                            vb_index->GetBufferIndex());  //buffer

            bind_shader = new_shader;
            return(true);
        }

        bool Renderable::Use()
        {
            if(!vao)return(false);

            glBindVertexArray(vao);
            return(true);
        }

        /**
        * 生成渲染状态
        */
        bool Renderable::MakeRenderState(bool mvp)
        {
            VertexBufferBase *vb_vertex =va->GetVertexBuffer(vbtVertex);

            if(!vb_vertex)return(false);                                    //没顶点，画不了，某些情况没顶点也能画，但我们暂时不支持

            hgl_zero(state);

            state.mvp                   =mvp;
            state.up_axis               =material->GetUpAxis();

            state.vertex_normal         = va->GetVertexBuffer(vbtNormal);
            state.vertex_color          = va->GetVertexBuffer(vbtColor);
            state.vertex_tangent        = va->GetVertexBuffer(vbtTangent);

            state.diffuse_map           = GetTexCoord(mtcDiffuse);
            state.normal_map            = GetTexCoord(mtcNormal);
            state.tangent_map           = GetTexCoord(mtcTangent);
            state.vertex_bi_normal      = GetTexCoord(mtcBiNormal);

            state.color_material        = material->GetColorMaterial();
            state.alpha_test            = material->GetAlphaTest()>0;
            state.outside_discard       = material->GetOutsideDiscard();

            state.vertex_pixel_compoment= va->GetVertexPixelCompoment();
            state.vertex_coord          = vb_vertex->GetComponent();

            state.height_map            = material->GetTexture(mtcHeight);

            for(int i=0;i<mtcMax;i++)
            {
                Texture *tex=material->GetTexture(i);

                if(tex)
                {
                    state.tex_type[i]=tex->GetType();
                    ++state.tex_number;

                    state.tex_pf[i]=tex->GetPixelFormat();
                }

                VertexBufferType vbt;
                VertexBufferBase *vb= GetTexCoord(i,&vbt);

                if(vb)
                {
                    state.vbc[i]=vb->GetComponent();
                    state.vbt[i]=vbt;
                }
            }

            state.light_mode            =material->GetLightMode();

            state.sun_light=true;       //测试用

            state.two_side              =material->GetTwoSide();

            if(!CheckPrim(GetPrimitive()))
            {
                LOG_ERROR(OS_TEXT("错误的图元类型：")+OSString(GetPrimitive()));
                return(false);
            }

            if(GetPrimitive()==HGL_PRIM_RECTANGLE)  //矩形
                state.rect_primivate=true;          //临时设计

            return(true);
        }

#ifdef _DEBUG
        Shader *CreateShader(const RenderState *state,const OSString &save_filename);

        Shader *Renderable::AutoCreateShader(bool mvp,ShaderManage *sm,const OSString &debug_outname)
#else
        Shader *Renderable::AutoCreateShader(bool mvp,ShaderManage *sm)
#endif//_DEBUG
        {
            if(!sm)sm=global_shader_manage;                     //使用全局Shader仓库

            if(shader)                                          //如果有shader存在
            {
                RenderState back_state=state;                   //备份旧状态

                if(!MakeRenderState(mvp))                       //生成新的状态
                    return(nullptr);

                if(back_state==state)                           //如果新旧状态一样
                    return(shader);

                shader=0;                                       //清除旧的shader
            }
            else
            {
                if(!MakeRenderState(mvp))                       //生成新的状态
                    return(nullptr);
            }

#ifdef _DEBUG
            shader=sm->Find(state);

            if(!shader)
            {
                shader=CreateShader(&state,debug_outname);

                if(!shader)
                    return(nullptr);

                sm->Add(state,shader);
            }
#else
            shader=sm->Get(state);
#endif//_DEBUG

            return shader;
        }
    }//namespace graph
}//namespace hgl
