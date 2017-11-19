vec4 texture_smooth(sampler2D samp, vec2 tc)
{
    vec2 texSize    =textureSize(samp, 0);
    vec2 uvScaled   =tc * texSize + 0.5;
    vec2 uvInt      =floor(uvScaled);
    vec2 uvFrac     =fract(uvScaled);

    uvFrac=smoothstep(0.0, 1.0, uvFrac);

    return texture(samp,(uvInt + uvFrac - 0.5) / texSize);
}
