uniform vec4 color;

in vec4 fragPos;

void main(void)
{
    gl_FragColor = color; // Цвет по умолчанию

    // Грани на плоскостях x = 1, x = -1
    if (abs(fragPos.x) == 1.0)
    {
        if (distance(vec2(fragPos.yz), vec2(0, 0)) < 0.5)
            gl_FragColor = vec4(1, 1, 0, 1);
        else
            gl_FragColor = vec4(1, 0, 0.1, 1);
    }

     // Грани на плоскостях z = 1, z = -1
    if (abs(fragPos.z) == 1.0)
    {
        if (distance(vec2(fragPos.xy), vec2(0, 0)) < 0.5)
            discard;
        else
            gl_FragColor = vec4(0, 0.6, 0.9, 1);
    }
}
