#include "Light.h"

using namespace std;

static const Light NoneLight = { "NONE", false, LightType::None, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 0, 0, 0 };

Light::Light(std::string name, bool active)
{
	*this = NoneLight;
	this->name = name;
	this->active = active;
}

Light::Light(std::string name, bool active, LightType type, 
	glm::vec3 position, glm::vec3 direction, float cutOff, 
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
	float constant, float linear, float quadratic) :
	name(name), active(active), type(type), 
	position(position), direction(direction), cutOff(cutOff), 
	ambient(ambient), diffuse(diffuse), specular(specular), 
	constant(constant), linear(linear), quadratic(quadratic)
{
	;
}

void Light::initLikePointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	type = LightType::Point;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::initLikeSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	type = LightType::Spot;
	this->position = position;
	this->direction = direction;
	this->cutOff = cutOff;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::initLikeDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	type = LightType::Directional;
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::initLikeAmbientLight(glm::vec3 ambient)
{
	type = LightType::Ambient;
	this->ambient = ambient;
}

bool Light::isLightOn()
{
	return type > LightType::None && active;
}

void Light::turnOn()
{
	active = true;
}

void Light::turnOff()
{
	active = false;
}

// returns 1 if light is put to shader and 0 if is not
int Light::putInShader(Shader* shader, int lightNumber)
{
	if (!isLightOn()) return 0;

	string num = to_string(lightNumber);

	switch (this->type)
	{
	case LightType::Directional:
		shader->setInt	("light[" + num + "].type",		int(type));
		shader->setVec3	("light[" + num + "].direction",direction);
		shader->setVec3	("light[" + num + "].ambient",	ambient);
		shader->setVec3	("light[" + num + "].diffuse",	diffuse);
		shader->setVec3	("light[" + num + "].specular", specular);
		break;
	case LightType::Point:
		shader->setInt	("light[" + num + "].type",		int(type));
		shader->setVec3	("light[" + num + "].position", position);
		shader->setVec3	("light[" + num + "].ambient",	ambient);
		shader->setVec3	("light[" + num + "].diffuse",	diffuse);
		shader->setVec3	("light[" + num + "].specular", specular);
		shader->setFloat("light[" + num + "].constant", constant);
		shader->setFloat("light[" + num + "].linear",	linear);
		shader->setFloat("light[" + num + "].quadratic",quadratic);
		break;
	case LightType::Spot:
		shader->setInt	("light[" + num + "].type",		int(type));
		shader->setVec3	("light[" + num + "].position", position);
		shader->setVec3	("light[" + num + "].direction",direction);
		shader->setFloat("light[" + num + "].cutOff",	cutOff);
		shader->setVec3	("light[" + num + "].ambient",	ambient);
		shader->setVec3	("light[" + num + "].diffuse",	diffuse);
		shader->setVec3	("light[" + num + "].specular", specular);
		shader->setFloat("light[" + num + "].constant", constant);
		shader->setFloat("light[" + num + "].linear",	linear);
		shader->setFloat("light[" + num + "].quadratic",quadratic);
		break;
	case LightType::Ambient:
		shader->setInt	("light[" + num + "].type",		int(type));
		shader->setVec3	("light[" + num + "].ambient",	ambient);
		break;
	}
	return 1;
}