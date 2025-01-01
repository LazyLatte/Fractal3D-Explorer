#include "julia4D_renderer.h"

Julia4DRenderer::Julia4DRenderer(Julia4D *julia4d, unsigned height, unsigned width): Renderer("julia4d.fs.glsl", height, width){
    this->julia4d = julia4d;
    this->power_location = glGetUniformLocation(this->program, "power");
    this->qk_location = glGetUniformLocation(this->program, "qk");
}
void Julia4DRenderer::setUniformParameters() const {
    glUniform1ui(this->power_location, this->julia4d->power);
    glUniform1f(this->qk_location, this->julia4d->qk);
    glUniform1i(this->julia_location, this->julia4d->julia);
    glUniform4fv(this->juliaOffset_location, 1,  &this->julia4d->juliaOffset[0]);

    glUniform3fv(this->color_location, 1,  &(this->julia4d->color.r));
}

float Julia4DRenderer::DE(glm::vec3 p) const {
    return this->julia4d->DE(p);
}