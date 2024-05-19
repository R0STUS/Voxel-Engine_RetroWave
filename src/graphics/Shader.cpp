#include "Shader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../coders/GLSLExtension.h"

namespace fs = std::filesystem;

GLSLExtension* Shader::preprocessor = new GLSLExtension();

Shader::Shader(unsigned int id) : id(id){
}

Shader::~Shader(){
    glDeleteProgram(id);
}

void Shader::use(){
    glUseProgram(id);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uniform1i(std::string name, int x){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform1i(transformLoc, x);
}

void Shader::uniform1f(std::string name, float x){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform1f(transformLoc, x);
}

void Shader::uniform2f(std::string name, float x, float y){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform2f(transformLoc, x, y);
}

void Shader::uniform2f(std::string name, glm::vec2 xy){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform2f(transformLoc, xy.x, xy.y);
}

void Shader::uniform2i(std::string name, glm::ivec2 xy){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform2i(transformLoc, xy.x, xy.y);
}

void Shader::uniform3f(std::string name, float x, float y, float z){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform3f(transformLoc, x,y,z);
}

void Shader::uniform3f(std::string name, glm::vec3 xyz){
    GLuint transformLoc = glGetUniformLocation(id, name.c_str());
    glUniform3f(transformLoc, xyz.x, xyz.y, xyz.z);
}


Shader* Shader::create(
    std::string vertexFile, 
    std::string fragmentFile,
    std::string vertexCode,
    std::string fragmentCode
) {
    vertexCode = preprocessor->process(fs::path(vertexFile), vertexCode);
    fragmentCode = preprocessor->process(fs::path(fragmentFile), fragmentCode);

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "SHADER::VERTEX: compilation failed: " << vertexFile << std::endl;
        std::cerr << infoLog << std::endl;
        return nullptr;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "SHADER::FRAGMENT: compilation failed: " << vertexFile << std::endl;
        std::cerr << infoLog << std::endl;
        return nullptr;
    }

    // Shader Program
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "SHADER::PROGRAM: linking failed" << std::endl;
        std::cerr << infoLog << std::endl;

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return new Shader(id);
}
