float smoothstep(float edge0,float edge1,float x)
{
    x=clamp((x-edge0)/(edge1-edge0),0.0,1.0);

    return x*x*(3.0-2.0*x);
}
