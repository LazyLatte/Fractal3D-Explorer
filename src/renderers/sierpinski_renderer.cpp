#include "sierpinski_renderer.h"

SierpinskiRenderer::SierpinskiRenderer(Sierpinski *sierpinski, unsigned height, unsigned width): Renderer("sierpinski.fs.glsl", height, width){
    this->sierpinski = sierpinski;
    this->xDeg_location = glGetUniformLocation(this->program, "xDeg");
    this->yDeg_location = glGetUniformLocation(this->program, "yDeg");
    this->zDeg_location = glGetUniformLocation(this->program, "zDeg");
}
void SierpinskiRenderer::setUniformParameters() const {
    glUniform1f(this->xDeg_location, this->sierpinski->xDeg);
    glUniform1f(this->yDeg_location, this->sierpinski->yDeg);
    glUniform1f(this->zDeg_location, this->sierpinski->zDeg);
    glUniform1i(this->julia_location, this->sierpinski->julia);
    glUniform3fv(this->juliaOffset_location, 1,  &this->sierpinski->juliaOffset[0]);

    glUniform3fv(this->color_location, 1,  &(this->sierpinski->color.r));
}

void SierpinskiRenderer::uniformParametersUI() const {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Setting", NULL);
    ImGui::SliderFloat("xDeg", &this->sierpinski->xDeg, 0.0f, 90.0f);
    ImGui::SliderFloat("yDeg", &this->sierpinski->yDeg, 0.0f, 90.0f);
    ImGui::SliderFloat("zDeg",&this->sierpinski->zDeg, 0.0f, 90.0f);

    ImGui::Checkbox("julia", &this->sierpinski->julia);
    ImGui::SliderFloat("x-julia", &this->sierpinski->juliaOffset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("y-julia", &this->sierpinski->juliaOffset.y, -2.0f, 2.0f);
    ImGui::SliderFloat("z-julia", &this->sierpinski->juliaOffset.z, -2.0f, 2.0f);

    ImGui::SliderFloat("r", &this->sierpinski->color.r, 0.0, 1.0);
    ImGui::SliderFloat("g", &this->sierpinski->color.g, 0.0, 1.0);
    ImGui::SliderFloat("b",&this->sierpinski->color.b, 0.0, 1.0);
    ImGui::End();
}

float SierpinskiRenderer::DE(glm::vec3 p) const {
    return this->sierpinski->DE(p);
}