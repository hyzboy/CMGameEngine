struct InfiniteSpotLight
{
    vec3 position;
    vec3 direction;
    vec4 halfVector;
    float exponent;

    vec4 ambient;
    vec4 specular;
    vec4 diffuse;
};

void Light_InfiniteSpot(in int i, in vec3 normal)
{
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor
    float spotAttenuation;
    vec3  Ppli;
    vec3  Sdli;

    nDotVP = max(0.0, dot(normal, normalize(vec3 (InfiniteSpotLightSource[i].position))));
    nDotHV = max(0.0, dot(normal, vec3 (InfiniteSpotLightSource[i].halfVector)));
    Ppli = -normalize(vec3(InfiniteSpotLightSource[i].position));
    Sdli = normalize(vec3(InfiniteSpotLightSource[i].direction));
    spotAttenuation = pow(dot(Ppli, Sdli), InfiniteSpotLightSource[i].exponent);

    if (nDotVP == 0.0)
    {
        pf = 0.0;
    }
    else
    {
        pf = pow(nDotHV, FrontMaterial.shininess);
    }

    Ambient  += InfiniteSpotLightSource[i].ambient * spotAttenuation;
    Diffuse  += InfiniteSpotLightSource[i].diffuse * nDotVP * spotAttenuation;
    Specular += InfiniteSpotLightSource[i].specular * pf * spotAttenuation;
};
