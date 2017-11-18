uint rgba8_to_uint32(vec4 value)
{
    // Ensure values are in [0..1] and make NaNs become zeros.
    value = min(max(value,0.0f), 1.0f);

    // Each component gets 8 bit.
    value = value * 255 + 0.5f;
    value = floor(value);

    // Pack into one 32 bit uint.
    return ( ((uint)value.x) |
            (((uint)value.y)<< 8) |
            (((uint)value.z)<<16) |
            (((uint)value.w)<<24) );
}
