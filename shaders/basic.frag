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
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(FragPos - light.position);
    float diff_koef = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff_koef * material.diffuse);

    // specular
    vec3 lightpos = light.position;
    float spec_koef = 0;
    for(float i = -0.2; i<=0.2; i+=0.05)
    {
        lightpos.y = light.position.y + i;
        vec3 slightDir = normalize(FragPos - lightpos);
        vec3 reflectDir = reflect(-slightDir, norm);
        vec3 viewDir = normalize(FragPos-viewPos);
        spec_koef += pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess*20.f);
    }
    vec3 specular = light.specular * (spec_koef * material.specular);

    if (wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
        outColor = texture(ourTexture, texCoords) * vec4(ambient + diffuse + specular, 1.0f);
}
