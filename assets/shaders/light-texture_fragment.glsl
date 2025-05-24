#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool enabled;
};

struct PosLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
    bool enabled;
};

const int NR_DIR_LIGHTS = 2;
const int NR_POS_LIGHTS = 4;
const int NR_SPOT_LIGHTS = 4;

uniform Material material;
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PosLight posLights[NR_POS_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform sampler2D ourTexture;
uniform bool modulate;

in vec3 FragPos;
in vec3 Normal;
in vec4 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

void calcComponents(vec3 lightDir, vec3 normal, vec3 viewDir,
                    vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular,
                    out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    ambient  = lightAmbient * material.ambient;
    diffuse  = lightDiffuse * diff * material.diffuse;
    specular = lightSpecular * spec * material.specular;
}

float calcAttenuation(float dist, float kc, float kl, float kq)
{
    return 1.0 / (kc + kl * dist + kq * dist * dist);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = -light.direction;
    vec3 ambient, diffuse, specular;
    calcComponents(lightDir, normal, viewDir, light.ambient, light.diffuse, light.specular,
                   ambient, diffuse, specular);
    return ambient + diffuse + specular;
}

vec3 calcPosLight(PosLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 ambient, diffuse, specular;
    calcComponents(lightDir, normal, viewDir, light.ambient, light.diffuse, light.specular,
                   ambient, diffuse, specular);
    float attenuation = calcAttenuation(distance(light.position, fragPos),
                                        light.constant, light.linear, light.quadratic);
    return attenuation * (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 ambient, diffuse, specular;
    calcComponents(lightDir, normal, viewDir, light.ambient, light.diffuse, light.specular,
                   ambient, diffuse, specular);

    float attenuation = calcAttenuation(distance(light.position, fragPos),
                                        light.constant, light.linear, light.quadratic);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return attenuation * (ambient + intensity * (diffuse + specular));
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < NR_DIR_LIGHTS; i++)
        if (dirLights[i].enabled)
            result += calcDirLight(dirLights[i], norm, viewDir);

    for (int i = 0; i < NR_POS_LIGHTS; i++)
        if (posLights[i].enabled)
            result += calcPosLight(posLights[i], norm, FragPos, viewDir);

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
        if (spotLights[i].enabled)
            result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = texture(ourTexture, texCoord)*vec4(result, 1.0);
    if (modulate)
        FragColor *= vertexColor;
        
}
