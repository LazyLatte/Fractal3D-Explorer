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

void MandelbulbRenderer::uniformParametersUI() const {
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Setting", NULL);
    ImGui::SliderFloat("power", &this->mbulb->power, 1.0f, 16.0f);

    ImGui::Checkbox("julia", &this->mbulb->julia);
    ImGui::SliderFloat("x-julia", &this->mbulb->juliaOffset.x, -2.0f, 2.0f);
    ImGui::SliderFloat("y-julia", &this->mbulb->juliaOffset.y, -2.0f, 2.0f);
    ImGui::SliderFloat("z-julia", &this->mbulb->juliaOffset.z, -2.0f, 2.0f);

    ImGui::SliderFloat("r", &this->mbulb->color.r, 0.0f, 1.0f);
    ImGui::SliderFloat("g", &this->mbulb->color.g, 0.0f, 1.0f);
    ImGui::SliderFloat("b",&this->mbulb->color.b, 0.0f, 1.0f);
    ImGui::End();
}

float MandelbulbRenderer::DE(glm::vec3 p) const {
    return this->mbulb->DE(p);
}