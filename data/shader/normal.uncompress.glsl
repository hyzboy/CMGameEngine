vec3 normal_2to3(vec2 n)
{
    vec3 normal;

    normal.z=dot(n,n)*2-1;
    normal.xy=normalize(n)*sqrt(1-normal.z*normal.z);

    return normal;
}
