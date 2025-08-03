#pragma once
#include "fractal.h"

class Mandelbox: public Fractal3D {
    private:
        static constexpr int iter = 16;
        static constexpr float bailout2 = 64.0; 
    public:
        Mandelbox(){
            this->params["scale"].value = MANDELBOX_SCALE_DEFAULT_VALUE;
            this->params["scale"].min = MANDELBOX_SCALE_MIN_VALUE;
            this->params["scale"].max = MANDELBOX_SCALE_MAX_VALUE;

            this->params["minR"].value = MANDELBOX_MINR_DEFAULT_VALUE;
            this->params["minR"].min = MANDELBOX_MINR_MIN_VALUE;
            this->params["minR"].max = MANDELBOX_MINR_MAX_VALUE;

            this->params["fold"].value = MANDELBOX_FOLD_DEFAULT_VALUE;
            this->params["fold"].min = MANDELBOX_FOLD_MIN_VALUE;
            this->params["fold"].max = MANDELBOX_FOLD_MAX_VALUE;
        }

        float DE(glm::vec3 p) const override {
            float scale = this->params.at("scale").value;
            float minR = this->params.at("minR").value;
            float fold = this->params.at("fold").value;

            float minRadius2 = minR * minR;
            float fixedRadius2 = 1.0;
            
            glm::vec3 c = this->julia ? toVec3(this->juliaOffset) : p;
            glm::vec3 v = p;
            float dr = 1.0;
            float r2;  

            for(int i=0; i<Mandelbox::iter; i++){
                v = glm::clamp(v, glm::vec3(-1.0), glm::vec3(1.0)) * 2.0f - v;
                v *= fold;
                r2 = glm::dot(v, v);
                if(r2 < minRadius2){
                    float factor = fixedRadius2 / minRadius2;
                    v *= factor;
                    dr *= factor;
                }else if(r2 < fixedRadius2){
                    float factor = fixedRadius2 / r2;
                    v *= factor;
                    dr *= factor;
                }

                v = c + scale * v;
                dr = dr * glm::abs(scale) + 1.0f;
                if(r2 > Mandelbox::bailout2) break;
            }
            return glm::sqrt(r2) / glm::abs(dr);
        }

        const char * getName() const override { return "MANDELBOX"; };
};