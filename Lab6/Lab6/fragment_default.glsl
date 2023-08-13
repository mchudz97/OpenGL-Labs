#version 150 core

struct PointLight {    
    vec3 position;
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 16  

uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};
uniform Material material;

uniform int alphaTex;
uniform vec3 camPos;

out vec4 outColor;
in mat3 vTBN;
smooth in vec2 UV;
in vec3 fragPos;

uniform samplerCube depthMap[NR_POINT_LIGHTS];
uniform float far_plane;

float ShadowCalculation(PointLight pointLight, vec3 fragPos, samplerCube currMap)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - pointLight.position;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(currMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = vTBN * normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 8);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));

    return (ambient + diffuse + specular);
}  


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, samplerCube currMap)
{
    vec3 lightDir = vTBN * normalize(light.position - fragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + (1.0 - ShadowCalculation(light, fragPos, currMap)) * (diffuse + specular));
} 

void main()
{
    vec3 norm = texture(material.normal, UV).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 cameraDir = vTBN * normalize(camPos - fragPos);

    vec3 result = CalcDirLight(dirLight, norm, cameraDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, cameraDir, depthMap[i]);
    }

    outColor = vec4(result, 1.0f);	
}