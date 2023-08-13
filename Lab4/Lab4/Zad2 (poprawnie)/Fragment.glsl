#version 330 core

in vec3 v_color;
in vec3 v_normal;
uniform int uRESOLUTION[2];
uniform int mode;
uniform vec3 camDir;
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
        col = vec3(v_color[2], rand * v_color[0], rand * v_color[1]);
    }
    else if(mode % 3 == 2)
    {
        col = vec3(.76, .49, .62);
    }
    else
    {
        col = smoothstep(.15, .9, vec3(1, 1, 1) - v_color);
    }
    vec3 norm = normalize(v_normal);
    vec3 lightDir = vec3(-1.0, 0.0, 2.0);
    vec3 lightColor = vec3(1, .8, .1);
    float ambStr = .5;
    vec4 amb = vec4(ambStr * lightColor, 1.0);
    vec4 diffuse = vec4(max(dot(norm, lightDir), 0) * lightColor, 1);
    vec3 refDir = reflect(-lightDir, norm);
    float specStr =.5;
    vec4 spec = pow(max(dot(camDir, refDir), 0.0), 4) * specStr * vec4(lightColor, 1.0);
    outColor = (amb + diffuse + spec) * vec4(col, 1.0);
}