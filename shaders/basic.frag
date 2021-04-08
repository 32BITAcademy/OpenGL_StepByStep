#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 vertColor;
in vec2 texCoords;
in vec3 vertNormal;
in vec3 FragPos;

out vec4 outColor;

uniform sampler2D ourTexture;
uniform bool wireframeMode;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    float dist = distance(light.position, FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient * material.ambient * attenuation;

    // diffuse
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(FragPos - light.position);
    float diff_koef = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff_koef * material.diffuse) * attenuation;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(FragPos-viewPos);
    float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    vec3 specular = light.specular * (spec_koef * material.specular) * attenuation;

    if (wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
        outColor = texture(ourTexture, texCoords) * vec4(ambient + diffuse + specular, 1.0f);
}
