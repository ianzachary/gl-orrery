#include "Orrery/Shader.h"

Shader::Shader(const char *pVertexPath, const char *pFragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(pVertexPath);
        fShaderFile.open(pFragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID_, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() { 
    glUseProgram(ID_);
}

void Shader::setBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID_, name.c_str()), value); 
}

void Shader::setMat4(const std::string &name, const float* value) const { 
    glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, value); 
}

void Shader::setVec3(const std::string &name, const float* value) const { 
    glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, value); 
}

unsigned int Shader::getID() {
    return ID_;
}