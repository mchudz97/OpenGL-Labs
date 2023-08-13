#version 150 core

struct PointLight {    
    vec3 position;
    float constant;
    float linear;
    float quadratic;  

    vec3 color;
};  
#define NR_POINT_LIGHTS 17  

uniform PointLight pointLights[NR_POINT_LIGHTS];


struct Material {
    sampler2D albedo;
    sampler2D roughness;
    sampler2D ao;
    sampler2D normal;
    sampler2D metallic;
};
uniform Material material;

uniform int alphaTex;
uniform vec3 camPos;

out vec4 outColor;
in vec2 UV;
in vec3 fragPos;
in vec3 Normal;
const float PI = 3.14159265359;


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



vec3 CalcPointLight(PointLight light, vec3 normalTBN, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // attenuation
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
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



    return (kD * albedo / PI + specular) * radiance * NdotL;
} 

void main()
{
    vec3 norm = getNormalFromMap();
    
    vec3 albedo = pow(texture(material.albedo, UV).rgb, vec3(2.2f));
    float metallic = texture(material.metallic, UV).r;
    float roughness = texture(material.roughness, UV).r;
    float ao = texture(material.ao, UV).r;
    vec3 F0 = vec3(.04f);
    F0 = mix(F0, albedo, metallic);

    vec3 cameraDir = normalize(camPos - fragPos);

    vec3 result = vec3(0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, cameraDir, albedo, roughness, metallic, F0);
    }

    outColor = vec4(result, 1.0f);	
}