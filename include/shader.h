#pragma once
#include <iostream>
#include <unordered_map>
#include <GL/glew.h>

class Shader {
    public:
        GLuint program;
        std::unordered_map<std::string, GLint> locations;
        Shader(std::string vs_filename, std::string fs_filename);
        Shader(std::string fractal_name);
        ~Shader();
        void use();
        void del();
        void setBool(const char* name, bool b);
        void setInt(const char* name, int val);
        void setUint(const char* name, unsigned int val);
        void setFloat(const char* name, float val);
        void setVec2(const char* name, float *val);
        void setVec3(const char* name, float *val);
        void setnVec3(const char* name, unsigned n, float *val);
        void setVec4(const char* name, float *val);
        void setMat4(const char* name, const float *val);
        void setLocation(const char* name);
};