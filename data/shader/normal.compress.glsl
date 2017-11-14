vec2 normal_3to2(vec3 normal)
{
    return normalize(normal.xy)*sqrt(normal.z*0.5+0.5);
}
