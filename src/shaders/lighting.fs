#version 330 core

out vec4 FragColor;

struct DirLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

// Could also be replaced with a SSBO (Shader Storage Buffer Object) (TO - DO)
// The idea here is to define a maximum number of lights to render in the scene,
// and then only render the maximum number of lights that there are currently,
// as long as this number is less than the maximum.
#define MAX_LIGHTS_TO_RENDER 128

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;
uniform sampler2D shadowMap;
uniform samplerCube pointShadowMap;

uniform DirLight dirLight;
uniform PointLight pointLight[MAX_LIGHTS_TO_RENDER];
uniform vec3 viewPos;

uniform int n_pointLights;
uniform float far_plane;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float PointShadowCalculation(PointLight light, vec3 fragPos);

void main()
{
    // Properties:
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // 1. Perform Directional Lighting and Shading:
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // 2. Perform Point Lighting and Shading:
    for(int i = 0; i < n_pointLights; i++)
        result += CalcPointLight(pointLight[i], norm, fs_in.FragPos, viewDir);
    
    // 3. Output result:
    FragColor = vec4(result, 1.0);
    // 4. Gamma correction:
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    // Ambient
    vec3 ambient = dirLight.ambient * texture(diffuseTexture, fs_in.TexCoords).rgb;
    // Diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * texture(diffuseTexture, fs_in.TexCoords).rgb;
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = dirLight.specular * spec * texture(specularTexture, fs_in.TexCoords).rgb;    
    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    // Blinn-Phong:
    float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularTexture, fs_in.TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // Calculate shadow:
    float shadow = PointShadowCalculation(light, fragPos);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
    // return (ambient + diffuse + specular);
}


float PointShadowCalculation(PointLight light, vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointShadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    
    return shadow;
}