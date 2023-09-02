#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
}; 

struct DirLight {
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
    vec2 TexCoords;
    vec3 T;
    vec3 B;
    vec3 N;
} fs_in;

// Could also be replaced with a SSBO (Shader Storage Buffer Object) (TO - DO)
// The idea here is to define a maximum number of lights to render in the scene,
// and then only render the maximum number of lights that there are currently,
// as long as this number is less than the maximum.
#define MAX_LIGHTS_TO_RENDER 128

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS_TO_RENDER];
uniform Material material;
uniform int numLights;
uniform bool blinn;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec2 TexCoords, mat3 TBN, vec3 FragPos, vec3 tangentFragPos, vec3 tangentViewPos);
vec3 CalcPointLight(PointLight light, vec3 normal, vec2 TexCoords, mat3 TBN, vec3 FragPos, vec3 tangentFragPos, vec3 tangentViewPos);

void main()
{
    // Generate TBN matrix
    mat3 TBN = transpose(mat3(fs_in.T, fs_in.B, fs_in.N));

    // Obtain normal from normal map in range [0,1]
    vec3 norm = texture(material.normal, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    norm = normalize(norm * 2.0 - 1.0);

    // Calculate view and fragment tangents
    vec3 tangentViewPos = TBN * viewPos;
    vec3 tangentFragPos = TBN * fs_in.FragPos;

    // 1. Directional Light:
    vec3 result = CalcDirLight(dirLight, norm, fs_in.TexCoords, TBN, fs_in.FragPos, tangentFragPos,  tangentViewPos);

    // 2. Point lights:
    for (int i = 0; i < numLights; i++)
        result += CalcPointLight(pointLights[i], norm, fs_in.TexCoords, TBN, fs_in.FragPos, tangentFragPos,  tangentViewPos); 
    
    // Output
    FragColor = vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec2 TexCoords, mat3 TBN, vec3 FragPos, vec3 tangentFragPos, vec3 tangentViewPos)
{
    // Get diffuse color:
    vec3 color = texture(material.diffuse, TexCoords).rgb;

    // Calculate ambient:
    vec3 ambient = light.ambient * color;

    // Calculate diffuse:
    vec3 lightDir = normalize(-light.direction);
    vec3 tangentLightPos = TBN * (FragPos - lightDir);
    lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    // Calculate specular:
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    float spec;
    if (blinn)
    {
        spec = 0.0f;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    vec3 specular = light.specular * spec;
    
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec2 TexCoords, mat3 TBN, vec3 FragPos, vec3 tangentFragPos, vec3 tangentViewPos)
{
    // Get diffuse color:
    vec3 color = texture(material.diffuse, TexCoords).rgb;

    // Calculate ambient:
    vec3 ambient = light.ambient * color;

    // Calculate diffuse:
    vec3 tangentLightPos = TBN * light.position;
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Calculate specular:
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    float spec;
    if (blinn)
    {
        spec = 0.0f;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    vec3 specular = light.specular * spec;

    // Attenuation:
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Combine results:
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}