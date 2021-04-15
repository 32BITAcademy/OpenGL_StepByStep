#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    int type;

    vec3 position;
    vec3 direction;
    float cutOff;

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
#define MAX_LIGHTS 4
uniform int lights_count;
uniform Light light[MAX_LIGHTS];

float getAtten(int i)
{
    float dist = distance(light[i].position, FragPos);
    float attenuation = 1.0 / (light[i].constant + light[i].linear*dist + light[i].quadratic * dist * dist);
    return attenuation;
}

vec3 CalcDiffusePlusSpecular(int i, vec3 lightDir)
{
    vec3 norm = normalize(vertNormal);
    float diff_koef = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light[i].diffuse * (diff_koef * material.diffuse);

    // specular
    vec3 reflectDir = reflect(lightDir, norm);
    vec3 viewDir = normalize(FragPos-viewPos);
    float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light[i].specular * (spec_koef * material.specular);

    return diffuse + specular;
}

void main()
{
    if (wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
    {
        outColor = vec4(0, 0, 0, 0);
        vec3 lresult;
        for (int i = 0; i<lights_count; i++)
        {
            if (light[i].type == 1) // Directional Light
            {
                vec3 lightDir = -light[i].direction;

                vec3 ambient = light[i].ambient * material.ambient;
                vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir);

                lresult = ambient + diffspec;
            }
            else 
            { 
                vec3 lightDir = -normalize(FragPos - light[i].position);
                if (light[i].type == 2) // Point Light
                {
                    float attenuation = getAtten(i);
                    vec3 ambient = light[i].ambient * material.ambient;
                    vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir);

                    lresult = (ambient + diffspec) * attenuation;
                }
                else if (light[i].type == 3) // SpotLight
                {
                    float angle = acos(dot(lightDir, normalize(-light[i].direction)));

                    if (angle <= light[i].cutOff*2.0f)
                    {
                        float koef  = 1.0f;
                        if (angle >= light[i].cutOff)
                        {
                            koef = (light[i].cutOff*2.0f - angle) / light[i].cutOff;
                        }

                        float attenuation = getAtten(i);
                        vec3 ambient = light[i].ambient * material.ambient;
                        vec3 diffspec = CalcDiffusePlusSpecular(i, lightDir) * koef;

                        lresult = (ambient + diffspec) * attenuation;
                    }
                    else
                    {
                        lresult =  material.ambient * light[i].ambient;
                    }
                }
            }
            outColor += texture(ourTexture, texCoords) * vec4(lresult, 1.0f);
        }// end of for
    }
}
