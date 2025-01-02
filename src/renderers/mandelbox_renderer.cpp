#include "mandelbox_renderer.h"

MandelboxRenderer::MandelboxRenderer(Mandelbox *mbox, unsigned height, unsigned width): Renderer("mandelbox.fs.glsl", height, width){
    this->mbox = mbox;
    this->scale_location = glGetUniformLocation(this->program, "scale");
    this->minR_location = glGetUniformLocation(this->program, "minR");
    this->fold_location = glGetUniformLocation(this->program, "fold");
}
void MandelboxRenderer::setUniformParameters() const {
    glUniform1f(this->scale_location, this->mbox->scale);
    glUniform1f(this->minR_location, this->mbox->minR);
    glUniform1f(this->fold_location, this->mbox->fold);
    glUniform1i(this->julia_location, this->mbox->julia);
    glUniform3fv(this->juliaOffset_location, 1,  &this->mbox->juliaOffset[0]);

    glUniform3fv(this->color_location, 1,  &(this->mbox->color.r));
}

void MandelboxRenderer::uniformParametersUI() const {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Setting", NULL);
    ImGui::SliderFloat("scale", &this->mbox->scale, -3, 3);
    ImGui::SliderFloat("minR", &this->mbox->minR, 0, 1);
    ImGui::SliderFloat("fold",&this->mbox->fold, 0, 2);

    ImGui::Checkbox("julia", &this->mbox->julia);
    ImGui::SliderFloat("x-julia", &this->mbox->juliaOffset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("y-julia", &this->mbox->juliaOffset.y, -2.0f, 2.0f);
    ImGui::SliderFloat("z-julia", &this->mbox->juliaOffset.z, -2.0f, 2.0f);

    ImGui::SliderFloat("r", &this->mbox->color.r, 0.0, 1.0);
    ImGui::SliderFloat("g", &this->mbox->color.g, 0.0, 1.0);
    ImGui::SliderFloat("b",&this->mbox->color.b, 0.0, 1.0);
    ImGui::End();
}

float MandelboxRenderer::DE(glm::vec3 p) const {
    return this->mbox->DE(p);
}
