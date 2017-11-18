vec3 DecodeR5G6B5(vec2 packed)
{
    // scale up to 8-bit
    packed *= 255.0f;

    // round and split the packed bits
    vec2 split = round(packed) / 8;   // first component at bit 3
    split.y /= 4;               // second component at bit 5

    // unpack (obfuscated yet optimized crap follows)
    vec3 rgb16 = 0.0f.rrr;
    rgb16.gb = frac(split) * 256;
    rgb16.rg += floor(split) * 4;
    rgb16.r *= 2;

    // scale down and return
    rgb16 /= 255.0f;
    return rgb16;
}
