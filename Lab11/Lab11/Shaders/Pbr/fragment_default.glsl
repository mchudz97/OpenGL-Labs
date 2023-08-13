#version 150 core

struct PointLight {    
    vec3 position;
    float str;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};  
#define NR_POINT_LIGHTS 16 

uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirLight 
{
    vec3 position;
    vec3 color;
};
uniform DirLight dirLight;

struct Material {
    sampler2D albedo;
    sampler2D roughness;
    sampler2D ao;
    sampler2D normal;
    sampler2D metallic;
};
uniform Material material;

uniform vec3 camPos;

out vec4 outColor;
in vec2 UV;
in vec3 fragPos;
in vec3 Normal;
in vec4 fragPosLightSpace;
const float PI = 3.14159265359;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform bool irradianceEnabled;
uniform samplerCube depthMap[NR_POINT_LIGHTS];
uniform float far_plane[NR_POINT_LIGHTS];
uniform sampler2D depthMapDir;
uniform bool dirShadowEnabled;
uniform bool pointShadowEnabled;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, UV).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    vec2 st1 = dFdx(UV);
    vec2 st2 = dFdy(UV);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float ShadowCalculationDirectional()
{
    if(!dirShadowEnabled)
        return 0;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0;
    float closestDepth = texture(depthMapDir, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.1 * (1.0 - dot(Normal, dirLight.position)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMapDir, 0);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(depthMapDir, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

float ShadowCalculation(PointLight pointLight, vec3 fragPos, samplerCube currMap, float currFarPlane)
{
    if(!pointShadowEnabled)
        return 0;
    vec3 fragToLight = fragPos - pointLight.position;
    float closestDepth = texture(currMap, fragToLight).r;
    closestDepth *= currFarPlane;
    float currentDepth = length(fragToLight);
    float bias = max(0.1 * (1.0 - dot(Normal, fragToLight)), 0.05);  
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        

    return shadow;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 




vec3 CalcPointLight(PointLight light, vec3 normalTBN, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic, vec3 F0, samplerCube currMap, float currFarPlane)
{
    if(light.str <= 0)
        return vec3(0, 0, 0);
    float shadow = ShadowCalculation(light, fragPos, currMap, currFarPlane);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // attenuation
    
    float dist = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)) * light.str;   
    vec3 radiance = light.color * attenuation;
    float NDF = DistributionGGX(normalTBN, halfwayDir, roughness);
    float G = GeometrySmith(normalTBN, viewDir, lightDir, roughness);
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    vec3 numerator = NDF * G * F; 
    float denominator = 4 * max(dot(normalTBN, viewDir), 0.0) * max(dot(normalTBN, lightDir), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    vec3 kD = vec3(1.0) - F;
    kD *= 1,0 - metallic;
    float NdotL = max(dot(normalTBN, lightDir), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow);
} 

vec3 CalcDirLight(DirLight light, vec3 normalTBN, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 direction = normalize(light.position);
    vec3 halfwayDir = normalize(direction + viewDir);
    vec3 radiance = light.color;
    float NDF = DistributionGGX(normalTBN, halfwayDir, roughness);
    float G = GeometrySmith(normalTBN, viewDir, direction, roughness);
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    vec3 numerator = NDF * G * F; 
    float denominator = 4 * max(dot(normalTBN, viewDir), 0.0) * max(dot(normalTBN, direction), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    vec3 kD = vec3(1.0) - F;
    kD *= 1,0 - metallic;
    float NdotL = max(dot(normalTBN, direction), 0.0);
    float shadow = ShadowCalculationDirectional();
    return (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow);
} 

void main()
{

    vec3 norm = getNormalFromMap();
    vec3 cameraDir = normalize(camPos - fragPos);
    vec3 reflection = reflect(-cameraDir, norm);
    
    vec3 albedo = pow(texture(material.albedo, UV).rgb, vec3(2.2f));
    float metallic = texture(material.metallic, UV).r;
    float roughness = texture(material.roughness, UV).r;
    float ao = texture(material.ao, UV).r;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, 0.0);

    vec3 color = CalcDirLight(dirLight, norm, fragPos, cameraDir, albedo, roughness, metallic, F0);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        color += CalcPointLight(pointLights[i], norm, fragPos, cameraDir, albedo, roughness, metallic, F0, depthMap[i], far_plane[i]);
    }

    if(irradianceEnabled)
    {
        vec3 irr = texture(irradianceMap, norm).rgb;
        vec3 diffuse = irr * albedo;
        vec3 kS = fresnelSchlickRoughness(max(dot(norm, cameraDir), 0.0),F0, roughness);
        vec3 kD = (1.0 - kS) * (1.0 - metallic);
    

        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(prefilterMap, reflection,  roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf  = texture(brdfLUT, vec2(max(dot(norm, cameraDir), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);
    
        vec3 ambient = (kD * diffuse + specular) * ao;
        color += ambient;
    }

    color = color / (color + 1.0);
    color = pow(color, vec3(1.0/2.2));

    outColor = vec4(color, 1.0f);
    
}