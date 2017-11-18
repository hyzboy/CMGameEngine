vec4 uint32_to_rgba8(uint value)
{
    return vec4((float)  (value      & 0x000000ff) / 255,
                (float) ((value>> 8) & 0x000000ff) / 255,
                (float) ((value>>16) & 0x000000ff) / 255,
                (float) ((value>>24) & 0x000000ff) / 255);
}
