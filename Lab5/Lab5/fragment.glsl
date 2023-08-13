#version 150 core

struct PointLight {    
    vec3 position;
    vec3 lightColor;
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 2  

uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

uniform sampler2D tex;
uniform int alphaTex;
uniform vec3 camPos;

out vec4 outColor;

in vec2 UV;
in vec3 normal;
in vec3 fragPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 color)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    // combine results
    vec3 ambient  = light.ambient  * vec3(color);
    vec3 diffuse  = light.diffuse  * diff * vec3(color);
    vec3 specular = light.specular * spec * vec3(color);
    return (ambient + diffuse + specular);
}  


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient * vec3(color);
    vec3 diffuse  = light.diffuse  * diff * vec3(color);
    vec3 specular = light.specular * spec * vec3(color);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{
    vec3 norm = normalize(normal);
    vec4 texx = texture(tex, UV);
	vec4 checker = texx;
    vec3 cameraDir = normalize(camPos - fragPos);
	if(alphaTex != 0)
	{
		if(checker[0] < .3 && checker[1] < .3 && checker[2] < .3)
		{
            discard;
		}
	}
    else
    {
        vec3 result = CalcDirLight(dirLight, norm, cameraDir, texx);
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            result += CalcPointLight(pointLights[i], norm, fragPos, cameraDir, texx);
        }

	    checker = vec4(result, 1);

    
    }
   
    outColor = checker;
	
	
}