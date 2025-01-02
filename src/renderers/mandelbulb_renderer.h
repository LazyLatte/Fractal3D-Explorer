#ifndef MANDELBULBRENDERER_H
#define MANDELBULBRENDERER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "renderer.h"
#include "../fractals/mandelbulb.h"
class MandelbulbRenderer: public Renderer {
    private:
        Mandelbulb *mbulb;
        GLint power_location;
    public:
        MandelbulbRenderer(Mandelbulb *mbulb, unsigned height, unsigned width);
        void setUniformParameters() const override;
        void uniformParametersUI() const override;
        float DE(glm::vec3 p) const override;
};


#endif