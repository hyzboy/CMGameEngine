vec3 depth_to_pos(in float depth, in vec2 ss_pos)
{
    vec4 ndcspace = vec4(ss_pos.x * 2.0 - 1.0, ss_pos.y * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);

    vec4 temp = inverse(ModelViewMatrix) * inverse(ProjectionMatrix) * ndcspace;

    return temp.xyz / temp.w;
}
