#pragma once
#include "fractal.h"

class Mandelbulb: public Fractal3D {
    private:
        static constexpr int iter = 9;
        static constexpr float bailout = 2.0f;
    public:
        Mandelbulb(){
            this->params["power"].value = MANDELBULB_POWER_DEFAULT_VALUE;
            this->params["power"].min = MANDELBULB_POWER_MIN_VALUE;
            this->params["power"].max = MANDELBULB_POWER_MAX_VALUE;
        }

        float DE(glm::vec3 p) const override {
            float power = this->params.at("power").value;
            glm::vec3 c = this->julia ? toVec3(this->juliaOffset) : p;
            glm::vec3 v = p;
            float dr = 1.0f;             
            float r = glm::length(v);  
            
            for (int i = 0; i < Mandelbulb::iter; i++) {
                float r_pow_n_minus_one = glm::pow(r, power - 1.0f);
                float r_pow_n = r * r_pow_n_minus_one;
                float theta = glm::atan(v.y, v.x) * power;
                float phi = glm::asin(v.z / r) * power;
                dr = power * r_pow_n_minus_one * dr + 1.0f;
                v = c + r_pow_n * glm::vec3(glm::cos(theta) * glm::cos(phi), glm::cos(phi) * glm::sin(theta), -glm::sin(phi));

                r = glm::length(v);     
                if (r > Mandelbulb::bailout) break;  
            }
            
            return 0.5f * glm::log(r) * r / dr;
        }

        const char * getName() const override { return "MANDELBULB"; };
};