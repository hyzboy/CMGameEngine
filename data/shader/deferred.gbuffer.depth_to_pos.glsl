vec3 DepthToPos(in float depth, in vec2 texCoord)
{
    vec4 ndcspace = vec4(texCoord.x * 2.0 - 1.0, texCoord.y * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);

    vec4 temp = inverse(ModelViewMatrix) * inverse(ProjectionMatrix) * ndcspace;

    return temp.xyz / temp.w;
}
