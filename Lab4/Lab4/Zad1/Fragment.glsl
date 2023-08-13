#version 330 core

in vec3 v_color;
in vec3 v_normal;

uniform int uRESOLUTION[2];
out vec4 outColor;
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{

    vec4 amb = vec4(v_color, 1.0);
    vec4 diffuse = vec4(max(dot(vec3(-1.0, 1.0, 0.0), -v_normal), 0) * vec3(.9,.9,.9), 1);
    outColor = amb + diffuse;
}