#ifndef SHADER_H
#define SHADER_H
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    unsigned int ID_;

    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const float* value) const;
    void setVec3(const std::string &name, const float* value) const;
    unsigned int getID();
};
#endif