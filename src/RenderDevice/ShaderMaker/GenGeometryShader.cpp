#include"GenGeometryShader.h"
#include<hgl/graph/RenderState.h>

namespace hgl
{
    namespace graph
    {
        //以primitive为输入，同样以primitive为输出

        //输入支持
        //* GL_POINTS

        //* GL_LINES
        //* GL_LINES_LOOP
        //* GL_LINES_STRIP

        //* GL_TRIANGLES
        //* GL_TRIANGLE_STRIP
        //* GL_TRIANGLE_FAN

        //* GL_LINES_ADJACENCY_EXT
        //* GL_LINE_STRIP_ADJACENCY_EXT

        //* GL_TRIANGLES_ADJACENCY_EXT
        //* GL_TRIANGLE_STRIP_ADJACENCY_EXT
        //
        //输出支持
        //* GL_POINTS
        //* GL_LINES_STRIP
        //* GL_TRIANGLE_STRIP

		constexpr char prim_name[][32]=
		{
			"points",						// 0
			"lines",						// 1
			"line_loop",					// 2
			"line_trip",					// 3
			"triangles",					// 4
			"triangle_strip",				// 5
			"triangle_fan",					// 6
			"",								// 7 quad
			"",								// 8 quad_strip
			"",								// 9 polygon
			"lines_adjacency",				// a
			"line_strip_adjacency",			// b
			"triangles_adjacency",			// c
			"triangle_strip_adjacency",		// d
			"patches"						// e
		};

		namespace shadergen
		{
			gs::gs(uint ip,uint op,uint mv)
			{
				in_prim=ip;
				out_prim=op;
				max_vertices=mv;
			}

			void gs::add_main_end()
			{
				char mv_str[8];

				hgl::utos(mv_str,8,max_vertices);

				add("layout (");add(prim_name[in_prim]);add(") in;\n"
					"layout (");add(prim_name[out_prim]);add(",max_vertices=");add(mv_str);add(") out;\n"
					"\n"
					"void main(void)\n"
					"{\n"
					"\tgl_Position=vec4(gl_in[0].gl_Position.xy,vec2(0,0));EmitVertex();\n"
					"\tgl_Position=vec4(gl_in[0].gl_Position.x,gl_in[0].gl_Position.y+gl_in[0].gl_Position.w,vec2(0,0));EmitVertex();\n"
					"\tgl_Position=vec4(gl_in[0].gl_Position.x+gl_in[0].gl_Position.z,gl_in[0].gl_Position.y,vec2(0,0));EmitVertex();\n"
					"\tgl_Position=vec4(gl_in[0].gl_Position.x+gl_in[0].gl_Position.z,gl_in[0].gl_Position.y+gl_in[0].gl_Position.w,vec2(0,0));EmitVertex();\n"
					"\n"
					"\tEndPrimitive();\n"
					"}");
			}
		}//namespace shadergen

		/**
		* 生成Fragment Shader代码
		* @param state 渲染状态
		* @return 生成的Fragment Shader代码
		* @return NULL 生成失败
		*/
#ifdef _DEBUG
		char *MakeGeometryShader(const RenderState *state,const OSString &filename)
#else
		char *MakeGeometryShader(const RenderState *state)
#endif//_DEBUG
		{
			if(!state)return(nullptr);

			if(!state->rect_primivate)		//目前就这一种用法，所以这么判断
				return(nullptr);

			shadergen::gs code(GL_POINTS,GL_TRIANGLE_STRIP,4);

			code.add_version(330);        //OpenGL 3.3

			code.add_main_end();

#ifdef _DEBUG
			code.debug_out(filename+OS_TEXT(".gs.glsl"));
#endif//_DEBUG

			return code.end_get();
		}
    }//namespace graph
}//namespace hgl
