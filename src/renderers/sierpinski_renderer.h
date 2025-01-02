#ifndef SIERPINSKIRENDERER_H
#define SIERPINSKIRENDERER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "renderer.h"
#include "../fractals/sierpinski.h"
class SierpinskiRenderer: public Renderer {
    private:
        Sierpinski *sierpinski;
        GLint xDeg_location;
        GLint yDeg_location;
        GLint zDeg_location;
    public:
        SierpinskiRenderer(Sierpinski *sierpinski, unsigned height, unsigned width);
        void setUniformParameters() const override;
        void uniformParametersUI() const override;
        float DE(glm::vec3 p) const override;
};


#endif /* SIERPINSKIRENDERER_H */