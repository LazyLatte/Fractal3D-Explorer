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
    glm::vec4 offset = glm::vec4(this->julia4d->juliaOffset.w, this->julia4d->juliaOffset.x, this->julia4d->juliaOffset.y, this->julia4d->juliaOffset.z);
    glUniform4fv(this->juliaOffset_location, 1,  &offset[0]);

    glUniform3fv(this->color_location, 1,  &(this->julia4d->color.r));
}

void Julia4DRenderer::uniformParametersUI() const {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Setting", NULL);
    ImGui::SliderInt("power", (int *)&this->julia4d->power, 2, 9);
    ImGui::SliderFloat("qk", &this->julia4d->qk, -2.0f, 2.0f);

    ImGui::Checkbox("julia", &this->julia4d->julia);
    ImGui::SliderFloat("w-julia", &this->julia4d->juliaOffset.w, -2.0f, 2.0f);
    ImGui::SliderFloat("x-julia", &this->julia4d->juliaOffset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("y-julia", &this->julia4d->juliaOffset.y, -2.0f, 2.0f);
    ImGui::SliderFloat("z-julia", &this->julia4d->juliaOffset.z, -2.0f, 2.0f);

    ImGui::SliderFloat("r", &this->julia4d->color.r, 0.0f, 1.0f);
    ImGui::SliderFloat("g", &this->julia4d->color.g, 0.0f, 1.0f);
    ImGui::SliderFloat("b",&this->julia4d->color.b, 0.0f, 1.0f);
    ImGui::End();
}

float Julia4DRenderer::DE(glm::vec3 p) const {
    return this->julia4d->DE(p);
}