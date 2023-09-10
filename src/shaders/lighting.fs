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
    mat3 TBN;
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

uniform bool useNormalMap;
uniform bool useHeightMap;

uniform float heightScale;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 TangentFragPos, vec2 texCoords);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 TangentFragPos, vec2 texCoords);
float PointShadowCalculation(PointLight light, vec3 fragPos);
vec3 CalcDirLightNoNormal(DirLight light, vec3 norm, vec3 viewDir, vec2 texCoords);
float ShadowCalculationNoNormal(vec4 fragPosLightSpace);
vec3 CalcPointLightNoNormal(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords);
float PointShadowCalculationNoNormal(PointLight light, vec3 fragPos);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
    if(useNormalMap)
    {
        // Properties:
        // Calculate normal from normal map provided by model:
        vec3 normal = texture(normalTexture, fs_in.TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        vec3 TangentViewPos = fs_in.TBN * viewPos;
        vec3 TangentFragPos = fs_in.TBN * fs_in.FragPos;
        vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
        vec2 texCoords = fs_in.TexCoords;
        if(useHeightMap)
        {
            texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
            if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
                discard;
        }

        // 1. Perform Directional Lighting and Shading:
        vec3 result = CalcDirLight(dirLight, normal, viewDir, TangentFragPos, texCoords);

        // 2. Perform Point Lighting and Shading:
        for(int i = 0; i < n_pointLights; i++)
            result += CalcPointLight(pointLight[i], normal, fs_in.FragPos, viewDir, TangentFragPos, texCoords);
    
        // 3. Output result:
        FragColor = vec4(result, 1.0);

        // 4. Gamma correction:
        // FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
    }
    else
    {
        // Properties:
        vec3 norm = normalize(fs_in.Normal);
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec2 texCoords = fs_in.TexCoords;
        if(useHeightMap)
        {
            texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
            if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
                discard;
        }

        // 1. Perform Directional Lighting and Shading:
        vec3 result = CalcDirLightNoNormal(dirLight, norm, viewDir, texCoords);

        // 2. Perform Point Lighting and Shading:
        for(int i = 0; i < n_pointLights; i++)
            result += CalcPointLightNoNormal(pointLight[i], norm, fs_in.FragPos, viewDir, texCoords);
    
        // 3. Output result:
        FragColor = vec4(result, 1.0);

        // 4. Gamma correction:
        // FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
    }
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 TangentFragPos, vec2 texCoords)
{
    // Ambient
    vec3 ambient = dirLight.ambient * texture(diffuseTexture, texCoords).rgb;
    // Diffuse
    // vec3 lightDir = normalize(-dirLight.direction);
    vec3 TangentLightPos = fs_in.TBN * dirLight.position;
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * texture(diffuseTexture, texCoords).rgb;
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = dirLight.specular * spec * texture(specularTexture, texCoords).rgb;    
    // vec3 specular = dirLight.specular * spec;    
    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, norm);                      
    
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm)
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
    vec3 normal = normalize(norm);
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

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 TangentFragPos, vec2 texCoords)
{
    // vec3 lightDir = normalize(light.position - fragPos);
    vec3 TangentLightPos = fs_in.TBN * light.position;
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    // Blinn-Phong:
    float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularTexture, texCoords));
    // vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // Calculate shadow:
    float shadow = PointShadowCalculation(light, fragPos);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
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


vec3 CalcDirLightNoNormal(DirLight light, vec3 norm, vec3 viewDir, vec2 texCoords)
{
    // Ambient
    vec3 ambient = dirLight.ambient * texture(diffuseTexture, texCoords).rgb;
    // Diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * texture(diffuseTexture, texCoords).rgb;
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = dirLight.specular * spec * texture(specularTexture, texCoords).rgb;    
    // Calculate shadow
    float shadow = ShadowCalculationNoNormal(fs_in.FragPosLightSpace);                      
    
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculationNoNormal(vec4 fragPosLightSpace)
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

vec3 CalcPointLightNoNormal(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
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
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(specularTexture, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // Calculate shadow:
    float shadow = PointShadowCalculationNoNormal(light, fragPos);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
    // return (ambient + diffuse + specular);
}


float PointShadowCalculationNoNormal(PointLight light, vec3 fragPos)
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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(heightTexture, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(heightTexture, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(heightTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}