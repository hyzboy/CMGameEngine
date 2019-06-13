#include"GenVertexShader.h"
#include"../ShaderDefine.h"
#include<hgl/graph/Renderable.h>
#include<hgl/graph/Render.h>
#include<hgl/LogInfo.h>
#include<stdio.h>
#include<string.h>

namespace hgl
{
    namespace graph
    {
        namespace shadergen
        {
            vs::vs()
            {
                vertex_format=0;

                color_material=false;

                light_mode=HGL_NONE_LIGHT;
                sun_light=false;

                mvp_matrix=false;

                in_normal=sitNone;
                in_vertex_color=sitNone;

                vertex_type=0;

                in_tex_count=0;
                in_texcoord_count=0;
                out_texcoord_count=0;

                hgl_zero(in_tex             );
                hgl_zero(in_texcoord_num    );
                hgl_zero(in_vertex_buffer   );
                hgl_zero(out_texcoord       );

                up_axis=HGL_AXIS_NONE;
            }

            void vs::add_mvp(bool normal)
            {
                //add_uniform_mat4(HGL_VS_PROJECTION_MATRIX);
                add_uniform_mat4(HGL_VS_MVP_MATRIX);                //传proj*mv，mv两个而不是proj,mv两个，以省去在vs中计算proj*mv

                if(normal)
                    add_uniform_mat3(HGL_VS_NORMAL_MATRIX);

                mvp_matrix=true;
            }

            void vs::add_layout_in(int vb_index,const char *name,int num)
            {
                add("layout(location=");
                add(UTF8String(vb_index));
                add(") ");
                add_in_fv(name,num);
            }

            void vs::add_in_vertex(int num)
            {
                vertex_type=num;

                add_layout_in(vbtIndex,HGL_VS_VERTEX,num);

                in_vertex_buffer[vbtVertex]=true;
            }

            void vs::add_in_normal()
            {
                add_layout_in(vbtNormal,HGL_VS_NORMAL,3);

                add_out_vec3(HGL_FS_NORMAL);

                in_normal=sitVertexAttrib;

                in_vertex_buffer[vbtNormal]=true;
            }

            void vs::add_uniform_normal()
            {
                add_uniform_vec3(HGL_VS_NORMAL);
                add_out_vec3(HGL_FS_NORMAL);

                in_normal=sitUniform;
            }

            int vs::SetVertexPixelCompoment(PixelCompoment fmt)
            {
                if(fmt<=HGL_PC_NONE||fmt>=HGL_PC_END)return(0);

                if(fmt==HGL_PC_ALPHA            ){vertex_color_to_vec4="vec4(1.0,1.0,1.0," HGL_VS_COLOR ")";return(1);}
                if(fmt==HGL_PC_LUMINANCE        ){vertex_color_to_vec4="vec4(vec3(" HGL_VS_COLOR "),1.0)";return(1);}
                if(fmt==HGL_PC_LUMINANCE_ALPHA  ){vertex_color_to_vec4="vec4(vec3(" HGL_VS_COLOR ".x)," HGL_VS_COLOR ".y)";return(2);}
                if(fmt==HGL_PC_RGB              ){vertex_color_to_vec4="vec4(" HGL_VS_COLOR ",1.0)";return(3);}
                if(fmt==HGL_PC_RGBA             ){vertex_color_to_vec4=HGL_VS_COLOR;return(4);}

                LOG_ERROR(OS_TEXT("输入的颜色格式无法处理：")+OSString(fmt));
                return(0);
            }

            void vs::set_color_material()
            {
                color_material=true;

                add_uniform_vec4(HGL_MATERIAL_COLOR);
            }

            void vs::add_in_color(PixelCompoment fmt)
            {
                const int num=SetVertexPixelCompoment(fmt);

                if(num<=0)
                {
                    LOG_ERROR(OS_TEXT("顶点色输入无格式信息或格式信息错误"));
                    return;
                }

                in_vertex_color=sitVertexAttrib;

                add_layout_in(vbtColor,HGL_VS_COLOR,num);
                add_out_fv(HGL_FS_COLOR,4);

                in_vertex_buffer[vbtColor]=true;
            }

            void vs::add_in_texture(const char *vb_name,uint tex_type,uint pixel_format,int mtc_index)
            {
                MATERIAL_TEXTURE_CHANNEL_NAME mtc_name;

                GetMaterialTextureName(mtc_name,mtc_index);

                add_sampler(mtc_name,tex_type);

                if(pixel_format!=GL_RGBA)
                    tex_sampler[mtc_index].Strcat("vec4(");

                tex_sampler[mtc_index].Strcat("texture(");        //texture会处理过滤,如果不过滤使用texelFetch会更高效
                tex_sampler[mtc_index].Strcat(mtc_name);
                tex_sampler[mtc_index].Strcat(",");
                tex_sampler[mtc_index].Strcat(vb_name);
                tex_sampler[mtc_index].Strcat(")");

                if(pixel_format!=GL_RGBA)
                {
                    if(pixel_format==GL_DEPTH   )tex_sampler[mtc_index].Strcat(".rrr,1)");else
                    if(pixel_format==GL_RED     )tex_sampler[mtc_index].Strcat(".rrr,1)");else
                    if(pixel_format==GL_RG      )tex_sampler[mtc_index].Strcat(".rrr,b)");else        //一般是luminance+a
                    if(pixel_format==GL_RGB     )tex_sampler[mtc_index].Strcat(".rgb,1)");else
                    {
                        //还有srgb,srgba就暂时不知道了如果处理了
                    }
                }

                in_tex[mtc_index]=true;
                in_tex_count++;
            }

            /**
            * 添加一个纹理坐标输入
            * @param vbt 输入纹理坐标缓冲区编号
            * @param vb 输入纹理坐标缓冲区名称
            * @param coord_num 坐标维数
            * @param mtc_index 目标纹理通道编号
            */
            void vs::add_in_texcoord(int vbt,const char *vb_name,int coord_num,int mtc_index)
            {
                if(mtc_index<0||mtc_index>=HGL_MAX_TEX_COORD)return;

                in_texcoord_num[mtc_index]=coord_num;

                in_texcoord_count++;

                if(in_vertex_buffer[vbt])return;                                            //已经定义过了，多个数据共用一个贴图坐标时有可能产生这种情况

                add_in_fv(vb_name,coord_num);                                               //用vb_name是为了让输入的顶点数据依旧使用TexCoord0,1,2,3这样的名字以便于调试

                in_vertex_buffer[vbt]=true;
            }

            void vs::set_light_mode(const LightMode &lm)
            {
                light_mode=lm;
            }

            void vs::set_sun_light(bool sl)
            {
                sun_light=sl;

                if(light_mode==HGL_VERTEX_LIGHT&&sun_light)
                {
                    add_uniform_vec3(HGL_SUN_LIGHT_DIRECTION);
                    add_out_float(HGL_SUN_LIGHT_INTENSITY);
                }
            }

            /**
            * 添加一个输出纹理坐标
            * @param coord_num 坐标维数
            * @param mtc_index 目标纹理通道编号
            * @param source 输入数据源，必须额外指名，因为有可能来源并不是当前通道
            */
            void vs::add_out_texcoord(int coord_num,int mtc_index,const char *source)
            {
                if(mtc_index<0||mtc_index>=HGL_MAX_TEX_COORD)return;

                MATERIAL_TEXTURE_CHANNEL_NAME mtc_name;

                GetMaterialTextureName(mtc_name,mtc_index);

                out_texcoord_count++;
                out_texcoord[mtc_index]=true;

                out_texcoord_name[mtc_index].Strcat(HGL_FS_TEXCOORD);
                out_texcoord_name[mtc_index].Strcat(mtc_name);

                out_texcoord_source[mtc_index].Strcat(source);

                add_out_fv(out_texcoord_name[mtc_index],coord_num);
            }

            bool vs::add_end(bool draw_rect)
            {
                add("\tvec4 Position;\n");

                if(in_normal)            //有法线要算
                {
//                     add("\n\tvec3 VP;\n");
//                     add("\tvec3 MVNormal;\n");
                }

                if(in_vertex_color)        //有顶点颜色
                {
                    if(!color_material)
                    {
                        add(U8_TEXT("\n\t" HGL_FS_COLOR "=")+vertex_color_to_vec4+U8_TEXT(";\n"));
                    }
                    else
                    {
                        add(U8_TEXT("\n\t" HGL_FS_COLOR "=")+vertex_color_to_vec4+U8_TEXT("*" HGL_MATERIAL_COLOR ";\n"));
                    }
                }

                if(out_texcoord_count)    //有纹理坐标需要输出到fs
                {
                    int count=0;

                    for(int i=0;i<HGL_MAX_TEX_COORD;i++)
                        if(out_texcoord[i])
                            count++;

                    if(count)
                    {
                        add();

                        for(int i=0;i<HGL_MAX_TEX_COORD;i++)
                        {
                            if(!out_texcoord[i])continue;

                            add(U8_TEXT("\t")+out_texcoord_name[i]+U8_TEXT("=")+out_texcoord_source[i]+U8_TEXT(";\n"));
                        }
                    }
                }

                //gl_Position计算
                {
                    UTF8String pos_str="\tPosition=";

                    if(vertex_type==2)
                    {
                        pos_str.Strcat("vec4(");

                        if(up_axis==HGL_AXIS_NONE)                          //非高度图
                        {
                            pos_str.Strcat(HGL_VS_VERTEX ",0.0,1.0);\n");
                        }
                        else if(up_axis==HGL_AXIS_X)                        //X轴向上
                        {
                            pos_str.Strcat(tex_sampler[mtcHeight]);
                            pos_str.Strcat(".r,");

                            pos_str.Strcat(HGL_VS_VERTEX ",1.0);\n");
                        }
                        else if(up_axis==HGL_AXIS_Y)                        //Y轴向上
                        {
                            pos_str.Strcat(HGL_VS_VERTEX ".x,");

                            pos_str.Strcat(tex_sampler[mtcHeight]);
                            pos_str.Strcat(".r," HGL_VS_VERTEX ".y,1.0);\n");
                        }
                        else if(up_axis==HGL_AXIS_Z)                        //Z轴向上
                        {
                            pos_str.Strcat(HGL_VS_VERTEX ",");

                            pos_str.Strcat(tex_sampler[mtcHeight]);
                            pos_str.Strcat(".r,1.0);\n");
                        }
                        else
                        {
                            LOG_ERROR(OS_TEXT("vs::add_end(),生成高度图高度坐标时，发现向上轴标识无法理解"));
                        }
                    }
                    else
                    if(vertex_type==3)
                    {
                        pos_str.Strcat("vec4(" HGL_VS_VERTEX ",1.0);\n");
                    }
                    else
                    if(vertex_type==4)
                    {
                        pos_str.Strcat(HGL_VS_VERTEX ";\n");
                    }
                    else
                    {
                        LOG_ERROR(OS_TEXT("错误的顶点格式!"));
                        return(false);
                    }

                    add();
                    add(pos_str);
                }

                if(in_normal)
                {
 //                    add("\n\tVP=normalize(" HGL_VS_LIGHT_POSITION "-Position*" HGL_VS_NORMAL_MATRIX ");\n");
 //                    add("\tMVNormal=normalize(" HGL_VS_NORMAL "*" HGL_VS_NORMAL_MATRIX ");\n\n");

                    add("\t" HGL_FS_NORMAL "=normalize(" HGL_VS_NORMAL "*" HGL_VS_NORMAL_MATRIX ");\n");

                    //灯光
                    if(light_mode==HGL_VERTEX_LIGHT)
                    {
                        if(sun_light)
                        {
                            add("\n\t" HGL_SUN_LIGHT_INTENSITY "=max(dot(" HGL_FS_NORMAL "," HGL_SUN_LIGHT_DIRECTION "),0.0);");

                            //未完成,不可用
                            //add("\n"
                            //    "\tif(" HGL_SUN_LIGHT_INTENSITY ">0.0)\n"
                            //    "\t\tfloat intSpec=max(dot(normalize(" HGL_SUN_LIGHT_DIRECTION "+normalize(-vec3(" HGL_VS_MODELVIEW_MATRIX "*" HGL_VS_VERTEX ")))," HGL_FS_NORMAL "),0.0);"
                            //    );
                        }
                    }
                }

                add();

                if (!mvp_matrix)
                    add("\tgl_Position=Position;\n");
                else
                {
                    if(draw_rect)
                    {
                        add("\tvec4 lt=vec4(Position.xy,vec2(0,1));\n"
                            "\tvec4 rb=vec4(Position.zw,vec2(0,1));\n"
                            "\n"
                            "\tvec4 lt_fin=lt*" HGL_VS_MVP_MATRIX ";\n"
                            "\tvec4 rb_fin=rb*" HGL_VS_MVP_MATRIX ";\n"
                            "\n"
                            "\tgl_Position=vec4(lt_fin.xy,rb_fin.xy);\n");
                    }
                    else
                    {
                        add("\tgl_Position=Position*" HGL_VS_MVP_MATRIX ";\n");
                    }
                }

                return(true);
            }
        }//namespace shadergen

        /**
        * 生成Vertex Shader代码
        * @param state 渲染状态
        * @return 生成的Vertex Shader代码
        * @return NULL 生成失败
        */
#ifdef _DEBUG
        char *MakeVertexShader(const RenderState *state,const OSString &filename)
#else
        char *MakeVertexShader(const RenderState *state)
#endif//
        {
            if(!state)return(nullptr);

            shadergen::vs code;

            code.add_version(330);        //OpenGL 3.3

            if(state->mvp)
            {
                code.add_mvp(((state->vertex_normal||state->normal_map)&&state->light_mode>HGL_NONE_LIGHT)?true:false);        //需要法线,则必须传modelview
            }

            //顶点
            {
                code.add_in_vertex(state->vertex_coord);
            }

            //法线
            if(state->vertex_normal)                //使用顶点法线
            {
                code.add_in_normal();

                //灯光
                code.set_light_mode(state->light_mode);
                code.set_sun_light(state->sun_light);
            }

            //颜色
            if(state->vertex_color)                 //使用顶点颜色
            {
                code.add_in_color(state->vertex_pixel_compoment);

                if(state->color_material)            //使用颜色材质传入,但没顶点颜色,也无需使用材质颜色
                {
                    code.set_color_material();
                }
            }

            //纹理
            if(state->tex_number)                    //如果有贴图
            {
                int tex_count=0;
                VERTEX_BUFFER_NAME vbn;

                //高度图
                if(state->height_map)                    //如果是高度图，则是拿vertex当高度图坐标、以及各种纹理坐标
                {                                        //高度图网格永远画在(0,0)-(1,1)的范围内，所以这个坐标同时可以直接用于高图度的纹理坐标，以及漫反射、法线、光照、阴影等贴图上。
                    if(state->vertex_coord!=2)
                    {
                        LOG_ERROR(OS_TEXT("使用高度图，但传入的顶点坐标不是2维数据."));
                        return(nullptr);
                    }

                    int hm_map_count=0;

                    GetVertexBufferName(vbn,vbtVertex);

                    code.add_in_texture(vbn,HGL_TEXTURE_2D,GL_RG,mtcHeight);

                    if(state->tex_type[mtcDiffuse]!=HGL_TEXTURE_2D)                    //漫反射，绘制坐标即贴图坐标
                    {
                        code.add_in_texcoord(vbtVertex,vbn,2,mtcDiffuse);
                        hm_map_count++;
                    }

                    if(hm_map_count)
                        code.add_out_texcoord(2,mtcHeight,vbn);        //将顶点坐标传给漫反射、镜片光、高光等纹理坐标

                    code.add();

                    code.set_up_axis(state->up_axis);
                }

                for(int i=0;i<mtcMax;i++)                            //增加贴图坐标
                {
                    if(!state->vbc[i])continue;

                    const VertexBufferType    vbt    =state->vbt[i];            //当前通道顶点缓冲区类型
                    const uint8               vbc    =state->vbc[i];            //当前通道顶点缓冲区坐标维数

                    GetVertexBufferName(vbn,vbt);

                    code.add_in_texcoord(vbt,vbn,vbc,i);        //第一个参数用vbt而不用i是为了让shader中的texcoord?编号与程序中填写的缓冲区编号一致，便于调试查看
                    code.add_out_texcoord(vbc,i,vbn);           //一般是输出到fragment shader用

                    tex_count++;
                }

                if(tex_count)        //没有加贴图不加这个空行
                    code.add();
            }

            code.add_main_begin();

            code.add_end(state->rect_primivate);                            //收尾各方面的处理

            code.add_main_end();

#ifdef _DEBUG
            code.debug_out(filename+OS_TEXT(".vs.glsl"));
#endif//_DEBUG

            return code.end_get();
        }//char *MakeVertexShader(Renderable *able)
    }//namespace graph
}//namespace hgl
