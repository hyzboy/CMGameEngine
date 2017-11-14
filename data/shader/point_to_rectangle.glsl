layout(points) in;
layout(triangle_strip,max_vertices=4) out;

void main()
{
    vec2 lt=gl_in[0].gl_Position.xy;
    vec2 wh=gl_in[0].gl_Position.zw;

    gl_Position=vec4(lt,                    vec2(0,0));EmitVertex();
    gl_Position=vec4(lt.x,      lt.y+wh.y,  vec2(0,0));EmitVertex();
    gl_Position=vec4(lt.x+wh.x, lt.y,       vec2(0,0));EmitVertex();
    gl_Position=vec4(lt+wh,                 vec2(0,0));EmitVertex();

    EndPrimitive();
}
