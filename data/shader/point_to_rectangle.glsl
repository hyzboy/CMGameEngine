ayout (points) in;
layout (triangle_strip,max_vertices=4) out;

void main()
{
	vec2 lt=gl_in[0].gl_Position.xy;
	vec2 rb=gl_in[0].gl_Position.zw;

	gl_Position=vec4(lt,         vec2(0,1));EmitVertex();
	gl_Position=vec4(lt.x,rb.y,  vec2(0,1));EmitVertex();
	gl_Position=vec4(rb.x,lt.y,  vec2(0,1));EmitVertex();
	gl_Position=vec4(rb,         vec2(0,1));EmitVertex();

	EndPrimitive();
}
