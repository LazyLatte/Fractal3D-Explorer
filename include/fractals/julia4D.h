#pragma once
#include "fractal.h"

class Julia4D: public Fractal4D {
    private:
        static constexpr int iter = 24;
        static constexpr float bailout = 8.0f;
    public:
        Julia4D(){
            this->julia = true;
            this->juliaOffset[0] = JULIA4D_JW_DEFAULT_VALUE;
            this->juliaOffset[1] = JULIA4D_JX_DEFAULT_VALUE;
            this->juliaOffset[2] = JULIA4D_JY_DEFAULT_VALUE;
            this->juliaOffset[3] = JULIA4D_JZ_DEFAULT_VALUE;
            //this->juliaOffset = glm::quat(-0.1f, 0.6f, 0.9f,-0.3f); // (w, x, y, z) ([3], [0], [1], [2])

            this->params["power"].value = JULIA4D_POWER_DEFAULT_VALUE;
            this->params["power"].min = JULIA4D_POWER_MIN_VALUE;
            this->params["power"].max = JULIA4D_POWER_MAX_VALUE;

            this->params["qk"].value = JULIA4D_QK_DEFAULT_VALUE;
            this->params["qk"].min = JULIA4D_QK_MIN_VALUE;
            this->params["qk"].max = JULIA4D_QK_MAX_VALUE;

        }
        

        float DE(glm::vec3 p) const override {
            float power = this->params.at("power").value;
            float qk = this->params.at("qk").value;

            glm::vec4 q = glm::vec4(p.x, p.y, p.z, qk);
            glm::vec4 c = this->julia ? toVec4(this->juliaOffset) : q;
            float dr = 1.0f;             
            float r = glm::length(q);  
            for (int i = 0; i < Julia4D::iter; i++) {
                float q0 = q.x;
                glm::vec3 u = glm::vec3(q.y, q.z, q.w);
                float theta = glm::atan(glm::length(u), q0) * power;
                q = c + glm::pow(r, power) * glm::vec4(glm::cos(theta), glm::sin(theta) * u / r);
                dr = power * glm::pow(r, power - 1.0f) * dr + 1.0f;
                
                r = glm::length(q);  
                if(r > Julia4D::bailout) break;
            }
            return 0.5f * glm::log(r) * r / dr;
        }

        const char * getName() const override { return "JULIA4D"; };
};