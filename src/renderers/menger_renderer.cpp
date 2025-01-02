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

void MengerRenderer::uniformParametersUI() const {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Setting", NULL);
    ImGui::SliderFloat("xDeg", &this->menger->xDeg, 0.0f, 90.0f);
    ImGui::SliderFloat("yDeg", &this->menger->yDeg, 0.0f, 90.0f);
    ImGui::SliderFloat("zDeg",&this->menger->zDeg, 0.0f, 90.0f);

    ImGui::Checkbox("julia", &this->menger->julia);
    ImGui::SliderFloat("x-julia", &this->menger->juliaOffset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("y-julia", &this->menger->juliaOffset.y, -2.0f, 2.0f);
    ImGui::SliderFloat("z-julia", &this->menger->juliaOffset.z, -2.0f, 2.0f);

    ImGui::SliderFloat("r", &this->menger->color.r, 0.0, 1.0);
    ImGui::SliderFloat("g", &this->menger->color.g, 0.0, 1.0);
    ImGui::SliderFloat("b",&this->menger->color.b, 0.0, 1.0);
    ImGui::End();
}

float MengerRenderer::DE(glm::vec3 p) const {
    return this->menger->DE(p);
}