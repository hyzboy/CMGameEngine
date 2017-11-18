vec2 EncodeR5G6B5(vec3 rgb24)
{
    // scale up to 8-bit
    rgb24 *= 255.0f;

    // remove the 3 LSB of red and blue, and the 2 LSB of green
    vec3 rgb16 = rgb24 / vec3(8, 4, 8);

    // split the green at bit 3 (we'll keep the 6 bits around the split)
    float greenSplit = rgb16.g / 8.0f;

    // pack it up (capital G's are MSB, the rest are LSB)
    vec2 packed;
    packed.x = rgb16.r * 8 + floor(greenSplit);     // rrrrrGGG
    packed.y = frac(greenSplit) * 256 + rgb16.b;        // gggbbbbb

    // scale down and return
    packed /= 255.0f;
    return packed;
}
