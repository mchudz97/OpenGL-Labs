#version 330 core

in vec3 v_color;
in vec3 v_normal;
uniform int uRESOLUTION[2];
uniform int mode;
out vec4 outColor;
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
    float xPos = gl_FragCoord[0];
	float yPos = gl_FragCoord[1];
    float rand = random(vec2(xPos/float(uRESOLUTION[0]),yPos)/float(uRESOLUTION[0]));
    vec3 col;
    if(mode % 3 == 0)
    {
        col = vec3(v_color[0], v_color[1], v_color[2]);
    }
    else if(mode % 3 == 1)
    {
        col = vec3(rand * v_color[2], rand * v_color[0], rand * v_color[1]);
    }
    else 
    {
        col = vec3(rand * v_color[0], rand * v_color[1], rand * v_color[2]);
    }
    vec4 amb = vec4(col, 1.0);
    vec4 diffuse = vec4(max(dot(vec3(-1.0, 1.0, 0.0), -v_normal), 0) * vec3(.9,.9,.9), 1);
    outColor = amb + diffuse;
}