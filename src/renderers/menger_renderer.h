#ifndef MENGERRENDERER_H
#define MENGERRENDERER_H
#define GLM_FORCE_SWIZZLE
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "renderer.h"
#include "../fractals/menger.h"
class MengerRenderer: public Renderer {
    private:
        Menger *menger;
        GLint xDeg_location;
        GLint yDeg_location;
        GLint zDeg_location;
    public:
        MengerRenderer(Menger *menger, unsigned height, unsigned width);
        void setUniformParameters() const override;
};


#endif