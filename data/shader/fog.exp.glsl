float fog_exp(const float dist,const float density)
{
    return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}
