#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatVec(const std::string& name, float* vec, int vec_size) const;
    unsigned int ID();

private:
    unsigned int programID;
    void checkCompileErrors(unsigned int shader, std::string type);
};