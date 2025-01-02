#ifndef RENDERER_H
#define RENDERER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "imgui.h"
std::string readShaderCode(const char *filename);
class Scene;
class Renderer {
    protected:
        GLuint vao;
        GLuint position_buffer;
        GLuint program;
        GLint pos_location;
        GLint front_location;
        GLint julia_location;
        GLint juliaOffset_location;
        GLint color_location;
        GLint iTime_location;
        Scene *scene;
    public:
        Renderer(const char *fs_filename, int height, int width);
        void setScene(Scene *scene);
        virtual void setUniformParameters() const = 0;
        virtual void uniformParametersUI() const = 0;
        virtual float DE(glm::vec3 p) const = 0;
        void draw(float t) const;
};
#include "../scene.h"

#endif