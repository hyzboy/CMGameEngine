#include<hgl/graph/GL/glew.h>
#include"ShaderDefine.h"
#include<hgl/graph/Render.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/InlineRenderable.h>
#include<hgl/LogInfo.h>
#include<hgl/graph/Renderable.h>
namespace hgl
{
    /**
    * 可改进项：

        2.2D渲染坐标系不调整状态无需modelview矩阵，现传递的是identity，可以想办法免去。
    */

    namespace graph
    {
        bool InitOpenGLAPI();                                                                        //初始化OpenGL API支持

        bool InitTextureAPI();        //初始化贴图API
        bool InitVertexBufferAPI(); //初始化顶点缓冲区API

        namespace OpenGLCore
        {
            void PutOpenGLInfo();    //输出OpenGL特性信息
            void InitScissor();        //初始化裁剪区
            void InitTexture();        //初始化贴图
            void CloseTexture();    //关闭贴图
            void InitBuffer();        //初始化缓冲区
            void InitFace();        //初始化面处理
        }//namespace OpenGLCore

        using namespace OpenGLCore;

        void InitVertexBuffer();    //初始化顶点缓冲区
        bool BindTexture(int active,unsigned int type,unsigned int texture_index);
        void BindTextures(int first_active,int count,unsigned int *texture_index);

        void InitPrimaryRenderBuffer();
        void ClosePrimaryRenderBuffer();

        void InitDefaultMaterial();
        void ClearDefaultMaterial();
        void InitGlobalShaderStorage();
        void ClearGlobalShaderStorage();

        namespace ubo
        {
            bool Init();
        }//namespace ubo

        namespace ssbo
        {
            bool Init();
        }//namespace ssbo

//         void InitFontStorage();
//         void ClearFontStorage();
        
        void CreateInlineTexture();        
        void ClearInlineTexture();
        void InitInlineRenderable();
        void ClearInlineRenderable();

        /**
        * 初始化渲染器
        */
        void InitRender()
        {
            PutOpenGLInfo();

            InitTextureAPI();
            InitVertexBufferAPI();

            InitScissor();
            InitTexture();
            InitBuffer();
            InitFace();

            InitVertexBuffer();

            ubo::Init();
            ssbo::Init();

            InitDefaultMaterial();
            InitGlobalShaderStorage();
            InitPrimaryRenderBuffer();
//             InitFontStorage();
            
            CreateInlineTexture();        
            InitInlineRenderable();
        }

        void CloseRender()
        {
            ClearInlineRenderable();
            ClearInlineTexture();

//             ClearFontStorage();
            ClearDefaultMaterial();
            ClearGlobalShaderStorage();
            CloseTexture();
            ClosePrimaryRenderBuffer();
        }
    }//namespace graph

    namespace graph
    {
        void SetPolygonMode(Material *mat)
        {
            if(mat)
            {
                uint draw_face,fill_mode,depth_func;

                mat->GetDrawMode(draw_face,fill_mode);

                SetPolygonMode(draw_face,fill_mode);

                const uint cull_face=mat->GetCullFace();

                if(cull_face==0)
                    CloseCullFace();
                else
                    OpenCullFace(cull_face);

                if(mat->GetDepthTest(depth_func))
                    OpenDepthTest(depth_func);
                else
                    CloseDepthTest();

                SetDepthMask(mat->GetDepthMask());
            }
            else
            {
                SetPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

                CloseCullFace();

                OpenDepthTest(GL_LESS);
                SetDepthMask(true);
            }
        }

        bool BindShaderVertexAttrib(Renderable *obj,Shader *glsl,int vbt)
        {
            VertexBufferBase *vb=obj->GetVertexBuffer((VertexBufferType)vbt);                       //取得缓冲区

            if(!vb)return(true);                                                                    //没有这个缓冲区

            VERTEX_BUFFER_NAME vertex_buffer_name;

            if(!GetVertexBufferName(vertex_buffer_name,vbt))return(false);

            int location=glsl->GetAttribLocation(vertex_buffer_name);                               //取得缓冲区对应shader属性地址

            if(location==-1)                                                                        //如果取得地址失败
            {
                LOG_HINT(u8"buffer \""+UTF8String(vertex_buffer_name)+u8"\" attrib location =-1");
                return(false);
            }

            obj->SetShaderLocation((VertexBufferType)vbt,location);                                 //设定缓冲区对应glsl属性
//            obj->SetShaderLocation((VertexBufferType)vbt,vbt);                                 //设定缓冲区对应glsl属性(使用了layout方法，所以无需再获取)

            return(true);
        }

        bool BindShaderVertexAttrib(Renderable *obj,Shader *glsl,const RenderState *state)
        {
            if (obj->GetBindShader() != glsl)
            {
                //本来这段可以写到OpenGLCoreRenderable类中的，为了一些变数，写在这里
                obj->ClearShaderLocation();                                                         //清除原有shader与顶点缓冲区的绑定

                //非贴图坐标数据
                for (int i = vbtVertex; i < vbtDiffuseTexCoord; i++)
                {
                    if (i == vbtNormal
                        &&state->light_mode==HGL_NONE_LIGHT)continue;                               //有法线但确定不用光照，则不传递法线

                    if (!BindShaderVertexAttrib(obj, glsl, i))
                    {
                        LOG_PROBLEM(OS_TEXT("BindShaderVertexAttrib error,vbt=") + OSString(i));
                        return(false);
                    }
                }

                //设置贴图坐标数据
                for (int i = 0; i < mtcMax; i++)
                {
                    VertexBufferType vbt;

                    if (obj->GetTexCoord(i, &vbt))
                        BindShaderVertexAttrib(obj, glsl, vbt);
                }

                obj->Bind(glsl);                                                                    //缓定glsl与缓冲区
            }

            obj->Use();

            return(true);
        }

        bool BindShaderUniform(Renderable *able,Shader *glsl,const Matrix4f *modelview,const RenderState *state)
        {
            const Material *mat=able->GetMaterial();

            //颜色
            if(state->color_material)
            {
                glsl->SetUniform4fv(HGL_MATERIAL_COLOR,mat->GetColor());
            }

            //AlphaTest
            if(state->alpha_test)
            {
                glsl->SetUniform1f(HGL_FS_ALPHA_TEST,mat->GetAlphaTest());
            }

            //灯光
            //if(state->light_mode>HGL_NONE_LIGHT)
            //{
            //}

            return(true);
        }

        bool BindShaderTexture(Renderable *able,Shader *glsl)
        {
            Material *mat=able->GetMaterial();

            int tex_binding_point=0;						//纹理绑定点，必须按顺序递增排列，与纹理id无关

            if(mat->GetTextureNumber())						//如果有贴图
            {
                MATERIAL_TEXTURE_CHANNEL_NAME mtc_name;

                for(int i=0;i<mtcMax;i++)
                {
                    Texture *tex=mat->GetTexture(i);                                    //取指定通道贴图

                    if(!tex)continue;                                                   //贴图不存在

                    BindTexture(tex_binding_point,tex->GetType(),tex->GetID());         //绑定贴图(在这里我们使用纹理绑定点当做texture active编号)

                    GetMaterialTextureName(mtc_name,i);

                    if(!glsl->SetUniform1i(mtc_name,tex_binding_point))			//设定贴图对应索引
                    {
                        LOG_PROBLEM(u8"attach Shader sampler \""+UTF8String(mtc_name)+u8"\" to texture "+UTF8String(tex_binding_point)+u8" error!");
                        return(false);
                    }

                    tex_binding_point++;
                }
            }

            return(true);
        }

        bool BindShaderMatrix(Shader *glsl,const Matrix4f *projection,const Matrix4f *modelview,const RenderState *state)
        {
            if(!state->mvp)
                return(true);

            if(!projection)
                return(false);

            if(modelview)
            {
                const Matrix4f mvp=(*projection)*(*modelview);

                if(!glsl->SetUniformMatrix4fv(HGL_VS_MVP_MATRIX,mvp))
                    return(false);

                if ((state->vertex_normal || state->normal_map) && state->light_mode>HGL_NONE_LIGHT)    //需要法线
                {
                    Matrix3f normal_matrix = modelview->Float3x3Part();             //法线矩阵为3x3

                    if (!glsl->SetUniformMatrix3fv(HGL_VS_NORMAL_MATRIX,normal_matrix))
                        return(false);

                    //随机太阳光方向，调试使用
                    if (state->sun_light)
                    {
                        static Vector3f sun_light_direction=Vector3f(rand(),rand(),rand()).Normalized();

                        glsl->SetUniform3fv(HGL_SUN_LIGHT_DIRECTION, sun_light_direction);
                    }
                }
            }
            else    //没modelview时将projection传为mvp
            {
                if(!glsl->SetUniformMatrix4fv(HGL_VS_MVP_MATRIX,*projection))
                    return(false);
            }

            return(true);
        }

        void SetBlend(Material *mtl)
        {
            if(!mtl)
            {
                glDisable(GL_BLEND);
                return;
            }

            bool blend;
            BlendMode mode;

            blend=mtl->GetBlend(mode);

            if(!blend)
            {
                glDisable(GL_BLEND);
                return;
            }

            glEnable(GL_BLEND);

            glBlendFuncSeparate(mode.rgb.src,
                                mode.rgb.dst,
                                mode.alpha.src,
                                mode.alpha.dst);

            glBlendEquationSeparate(mode.rgb.func,
                                    mode.alpha.func);
        }

        /**
        * 直接渲染一个对象
        * @param obj 可渲染对象
        * @param mvp Projection*ModelView矩阵数据
        * @param modelview ModelView矩阵数据
        * @return 是否执行成功
        */
        bool DirectRender(Renderable *obj,const Matrix4f *projection,const Matrix4f *modelview)
        {
            if(!obj)return(false);

            Shader *    glsl        =obj->GetShader();          //取得glsl
            Material *  mtl         =obj->GetMaterial();        //取得材质
            uint        draw_prim   =obj->GetPrimitive();       //取得图元类型
            int         draw_start;
            int         draw_count;

            if(!glsl||!mtl)return(false);

            VertexBufferBase *vb_index=obj->GetVertexBuffer(vbtIndex);

            obj->GetDrawCount(draw_start,draw_count);                   //取得所需绘制的顶点数

            if(draw_count<=0)return(false);                             //如果数量为0

            SetPolygonMode(mtl);

            //绑定shader
            {
                const RenderState *state=(obj)->GetRenderState();                               //渲染状态

                glsl->Use();                                                                    //启用glsl

                if(!BindShaderVertexAttrib(obj,glsl,state))                                     //绑定shader顶点属性
                {
                    LOG_PROBLEM(OS_TEXT("BindShaderVertexAttrib error"));
                    return(false);
                }

                if(!BindShaderUniform(obj,glsl,modelview,state))                                //绑定shader一致变量
                {
                    LOG_PROBLEM(OS_TEXT("BindShaderUniform error"));
                    return(false);
                }

                if(!BindShaderMatrix(glsl,projection,modelview,state))                          //绑定shader矩阵
                {
                    LOG_PROBLEM(OS_TEXT("BindShaderMatrix error"));
                    return(false);
                }

                if(!BindShaderTexture(obj,glsl))                                                //绑定shader贴图
                {
                    LOG_PROBLEM(OS_TEXT("BindShaderTexture error"));
                    return(false);
                }
            }

            //设置混合
            SetBlend(mtl);

            //绘制
            {
				if(draw_prim==HGL_PRIM_RECTANGLE)
					draw_prim=GL_POINTS;

                if(vb_index)
                    glDrawElements(draw_prim,draw_count,vb_index->GetDataType(),(const void *)(draw_start*vb_index->GetDataBytes()));
                else
                    glDrawArrays(draw_prim,draw_start,draw_count);                                //绘制数据
            }

            return(true);
        }//bool DirectRender(Renderable *obj,const Matrix4f *proj,const Matrix4f *modelview)
    }//namespace graph
}//namespace hgl
