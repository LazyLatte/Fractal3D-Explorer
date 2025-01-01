#ifndef JULIA4DRENDERER_H
#define JULIA4DRENDERER_H
#include <GL/glew.h>
#include "renderer.h"
#include "../fractals/julia4D.h"
class Julia4DRenderer: public Renderer {
    private:
        Julia4D *julia4d;
        GLint power_location;
        GLint qk_location;
    public:
        Julia4DRenderer(Julia4D *julia4d, unsigned height, unsigned width);
        void setUniformParameters() const override;
        float DE(glm::vec3 p) const override;
};


#endif