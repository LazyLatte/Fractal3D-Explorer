#include "menger_renderer.h"

MengerRenderer::MengerRenderer(Menger *menger, unsigned height, unsigned width): Renderer("menger.fs.glsl", height, width){
    this->menger = menger;
    this->xDeg_location = glGetUniformLocation(this->program, "xDeg");
    this->yDeg_location = glGetUniformLocation(this->program, "yDeg");
    this->zDeg_location = glGetUniformLocation(this->program, "zDeg");
}
void MengerRenderer::setUniformParameters() const {
    glUniform1f(this->xDeg_location, this->menger->xDeg);
    glUniform1f(this->yDeg_location, this->menger->yDeg);
    glUniform1f(this->zDeg_location, this->menger->zDeg);
    glUniform1i(this->julia_location, this->menger->julia);
    glUniform3fv(this->juliaOffset_location, 1,  &this->menger->juliaOffset[0]);

    glUniform3fv(this->color_location, 1,  &(this->menger->color.r));
}

float MengerRenderer::DE(glm::vec3 p) const {
    return this->menger->DE(p);
}