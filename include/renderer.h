#pragma once
#include <GL/glew.h>
#include "shader.h"
#include "fractals/fractal.h"

class Renderer {
    public:
        Renderer(IFractal* fractal, int width, int height);
        
        void resize(int width, int height);

        void draw(int width, int height) const;
        void render2Screen() const;
        void switchFractal(IFractal *newFractal);
        IFractal *getFractal() const { return this->fractal; }
        GLuint getTexture() const { return this->texture; }

    private:
        GLuint vao, texture;
        Shader *fractalShader, *screenShader;
        IFractal* fractal;
};