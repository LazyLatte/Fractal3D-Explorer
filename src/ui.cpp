#include "ui.h"
#include "imgui.h"

void UIParameterEditor::draw() const {
    for(auto& [key, value] : this->fractal->getParameters()){
        ImGui::SliderFloat(key.c_str(), &value.value, value.min, value.max);
    }

    ImGui::Checkbox("julia", &this->fractal->useJulia());

    int dim = this->fractal->getDimension();
    if(dim == 3){
        ImGui::SliderFloat("x-julia", this->fractal->getJuliaOffset() + 0, -2.0f, 2.0f);
        ImGui::SliderFloat("y-julia", this->fractal->getJuliaOffset() + 1, -2.0f, 2.0f);
        ImGui::SliderFloat("z-julia", this->fractal->getJuliaOffset() + 2, -2.0f, 2.0f);
    }else if(dim == 4){
        ImGui::SliderFloat("w-julia", this->fractal->getJuliaOffset() + 0, -2.0f, 2.0f);
        ImGui::SliderFloat("x-julia", this->fractal->getJuliaOffset() + 1, -2.0f, 2.0f);
        ImGui::SliderFloat("y-julia", this->fractal->getJuliaOffset() + 2, -2.0f, 2.0f);
        ImGui::SliderFloat("z-julia", this->fractal->getJuliaOffset() + 3, -2.0f, 2.0f);
    }

    ImGui::SliderFloat("r", &this->fractal->getColor().r, 0.0, 1.0);
    ImGui::SliderFloat("g", &this->fractal->getColor().g, 0.0, 1.0);
    ImGui::SliderFloat("b", &this->fractal->getColor().b, 0.0, 1.0);
}
