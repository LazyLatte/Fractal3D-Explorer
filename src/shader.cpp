#include "shader.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <filesystem>
std::filesystem::path shaderDir = std::filesystem::current_path().parent_path() / "shaders";

std::string readShaderCode(std::string filename){
    std::ifstream in;
    std::filesystem::path shaderPath = shaderDir / filename;
    in.open(shaderPath);
    if(!in.is_open()){
        std::cout << "Failed to open file " << shaderPath << std::endl;
        return "";
    }
    std::string shaderCode = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    std::regex re("#include[ \t]+<([a-zA-Z][a-zA-Z0-9]*.glsl)>");
    std::smatch m;

    while(std::regex_search(shaderCode, m, re)){
        std::string includedGLSL = readShaderCode(m[1]);
        
        shaderCode = std::regex_replace(shaderCode, std::regex(m[0].str()), includedGLSL);
    }
    in.close();
    return shaderCode;
}

Shader::Shader(std::string vs_filename, std::string fs_filename){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string shaderSource = readShaderCode(vs_filename);
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(vertexShader, 1, s, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vs_filename << "\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    };

    shaderSource = readShaderCode(fs_filename);
    s[0] = shaderSource.c_str();
    glShaderSource(fragmentShader, 1, s, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << fs_filename << "\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    };


    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(std::string fractal_name){
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string shaderSource = "#version 430\n#define " + fractal_name + "\n" + readShaderCode("raymarching.comp");
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(computeShader, 1, s, NULL);
    glCompileShader(computeShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED: " << "raymarching.comp" << "\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    };

    this->program = glCreateProgram();
    glAttachShader(this->program, computeShader);
    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glDeleteShader(computeShader);
}

Shader::~Shader(){
    glDeleteProgram(this->program);
}

void Shader::setLocation(const char* name){
    this->locations[name] = glGetUniformLocation(this->program, name);
}

void Shader::setBool(const char* name, bool b){
    glUniform1i(this->locations[name], b);
}

void Shader::setInt(const char *name, int val){
    glUniform1i(this->locations[name], val);
}

void Shader::setUint(const char* name, unsigned int val){
    glUniform1ui(this->locations[name], val);
}

void Shader::setFloat(const char* name, float val){
    glUniform1f(this->locations[name], val);
}
void Shader::setVec2(const char* name, float *val){
    glUniform2fv(this->locations[name], 1, val);
}

void Shader::setVec3(const char* name, float *val){
    glUniform3fv(this->locations[name], 1, val);
}

void Shader::setnVec3(const char* name, unsigned n, float *val){
    glUniform3fv(this->locations[name], n, val);
}

void Shader::setVec4(const char* name, float *val){
    glUniform4fv(this->locations[name], 1, val);
}

void Shader::setMat4(const char* name, const float *val){
    glUniformMatrix4fv(this->locations[name], 1, GL_FALSE, val);
}

void Shader::use(){
    glUseProgram(this->program);
}

void Shader::del(){
    glDeleteProgram(this->program);
}
