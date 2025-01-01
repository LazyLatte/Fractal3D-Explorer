#include "mandelbulb_renderer.h"

MandelbulbRenderer::MandelbulbRenderer(Mandelbulb *mbulb, unsigned height, unsigned width): Renderer("mandelbulb.fs.glsl", height, width){
    this->mbulb = mbulb;
    this->power_location = glGetUniformLocation(this->program, "power");
}
void MandelbulbRenderer::setUniformParameters() const {
    glUniform1f(this->power_location, this->mbulb->power);
    glUniform1i(this->julia_location, this->mbulb->julia);
    glUniform3fv(this->juliaOffset_location, 1,  &this->mbulb->juliaOffset[0]);

    glUniform3fv(this->color_location, 1,  &(this->mbulb->color.r));
}

float MandelbulbRenderer::DE(glm::vec3 p) const {
    return this->mbulb->DE(p);
}