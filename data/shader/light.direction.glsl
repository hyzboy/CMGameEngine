struct DirectionLight
{
    vec3 direction;
    vec4 halfVector;

    vec4 ambient;
    vec4 specular;
    vec4 diffuse;
};

void Light_Directional(in int i, in vec3 normal)
{
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor

    nDotVP = max(0.0, dot(normal, normalize(vec3 (DirectionLightSource[i].direction))));
    nDotHV = max(0.0, dot(normal, vec3 (DirectionLightSource[i].halfVector)));

    if (nDotVP == 0.0)
    {
        pf = 0.0;
    }
    else
    {
        pf = pow(nDotHV, FrontMaterial.shininess);
    }

    Ambient  += DirectionLightSource[i].ambient;
    Diffuse  += DirectionLightSource[i].diffuse * nDotVP;
    Specular += DirectionLightSource[i].specular * pf;
};
