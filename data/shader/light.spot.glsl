struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 attenuation;
    float coscutoff;
    float exponent;

    vec4 ambient;
    vec4 specular;
    vec4 diffuse;
};

void Light_Spot(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3)
{
    float nDotVP;            // normal . light direction
    float nDotHV;            // normal . light half vector
    float pf;                // power factor
    float spotDot;           // cosine of angle between spotlight
    float spotAttenuation;   // spotlight attenuation factor
    float attenuation;       // computed attenuation factor
    float d;                 // distance from surface to light source
    vec3  VP;                // direction from surface to light position
    vec3  halfVector;        // direction of maximum highlights

    // Compute vector from surface to light position
    VP = vec3 (SpotLightSource[i].position) - ecPosition3;

    // Compute distance between surface and light position
    d = length(VP);

    // Normalize the vector from surface to light position
    VP = normalize(VP);

    // Compute attenuation
    attenuation = 1.0 / (SpotLightSource[i].attenuation[0] +
        SpotLightSource[i].attenuation[1] * d +
        SpotLightSource[i].attenuation[2] * d * d);

    // See if point on surface is inside cone of illumination
    spotDot = dot(-VP, normalize(SpotLightSource[i].direction));

    if (spotDot < SpotLightSource[i].coscutoff)
    {
        spotAttenuation = 0.0; // light adds no contribution
    }
    else
    {
        spotAttenuation = pow(spotDot, SpotLightSource[i].exponent);
    }

    // Combine the spotlight and distance attenuation.
    attenuation *= spotAttenuation;
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

    Ambient  += SpotLightSource[i].ambient * attenuation;
    Diffuse  += SpotLightSource[i].diffuse * nDotVP * attenuation;
    Specular += SpotLightSource[i].specular * pf * attenuation;
};
