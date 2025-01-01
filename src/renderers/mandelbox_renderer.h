#ifndef MANDELBOXRENDERER_H
#define MANDELBOXRENDERER_H
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "renderer.h"
#include "../fractals/mandelbox.h"
class MandelboxRenderer: public Renderer {
    private:
        Mandelbox *mbox;
        GLint scale_location;
        GLint minR_location;
        GLint fold_location;
    public:
        MandelboxRenderer(Mandelbox *mbox, unsigned height, unsigned width);
        void setUniformParameters() const override;
        float DE(glm::vec3 p) const override;
};


#endif