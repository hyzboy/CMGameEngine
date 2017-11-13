struct PointLight
{
    vec3 position;
    vec4 halfVector;
    vec3 attenuation'

    vec4 ambient;
    vec4 specular;
    vec4 diffuse;
};

void Light_Point(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3)
{
    float nDotVP;       // normal . light direction
    float nDotHV;       // normal . light half vector
    float pf;           // power factor
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light source
    vec3  VP;           // direction from surface to light position
    vec3  halfVector;   // direction of maximum highlights

    // Compute vector from surface to light position
    VP = vec3 (PointLightSource[i].position) - ecPosition3;

    // Compute distance between surface and light position
    d = length(VP);

    // Normalize the vector from surface to light position
    VP = normalize(VP);

    // Compute attenuation
    attenuation = 1.0 / (PointLightSource[i].attenuation[0] +
        PointLightSource[i].attenuation[1] * d +
        PointLightSource[i].attenuation[2] * d * d);

    halfVector = normalize(VP + eye);
    nDotVP = max(0.0, dot(normal, VP));
    nDotHV = max(0.0, dot(normal, halfVector));

    if (nDotVP == 0.0)
    {
        pf = 0.0;
    }
    else
    {
        pf = pow(nDotHV, FrontMaterial.shininess);
    }

    Ambient  += PointLightSource[i].ambient * attenuation;
    Diffuse  += PointLightSource[i].diffuse * nDotVP * attenuation;
    Specular += PointLightSource[i].specular * pf * attenuation;
};
