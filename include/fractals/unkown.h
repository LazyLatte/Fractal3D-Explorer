#pragma once
#include "fractal.h"
#include <cmath>
class Unkown: public Fractal3D {
    private:
    public:
        Unkown(){

            this->params["param1"].value = 6.0f;
            this->params["param1"].min = 2.0f;
            this->params["param1"].max = 30.0f;

            this->params["param2"].value = 2.0f;
            this->params["param2"].min = 0.0f;
            this->params["param2"].max = 3.0f;

            this->params["param3"].value = 2.0f;
            this->params["param3"].min = 0.0f;
            this->params["param3"].max = 3.0f;
        }
        
        // float DE(glm::vec3 p) const override {
        //     glm::vec4 v = glm::vec4(p, 1.0);
        //     for(int i=0; i<8; i++){
        //         glm::vec3 tmp = glm::vec3(v);
        //         tmp = glm::mod(tmp - glm::vec3(1.0), glm::vec3(2.0)) - glm::vec3(1.0);
        //         v.x = tmp.x;
        //         v.y = tmp.y;
        //         v.z = tmp.z;
        //         v *= (1.4 / glm::dot(tmp, tmp));
        //     }

        //     return glm::length(glm::vec2(v.x, v.z) / v.w) * 0.25f;
        // }

        float DE(glm::vec3 p) const override {
            float scale = 1.0f;
            for(int i=0; i<6; i++){
                p = glm::mod(p - glm::vec3(1.0f), glm::vec3(2.0f)) - glm::vec3(1.0f);
                p -= glm::sign(p) * 0.04f;
                float k = 0.95f/glm::dot(p,p);
                p  *= k;  
                scale *= k;
            }
            glm::vec2 pxy = glm::vec2(p.x, p.y);
            glm::vec2 pyz = glm::vec2(p.y, p.z);
            glm::vec2 pzx = glm::vec2(p.z, p.x);

            float d1 = glm::sqrt( glm::min( glm::min( glm::dot(pxy, pxy), glm::dot(pyz, pyz) ), glm::dot(pzx,pzx) ) ) - 0.02f;
            float d2 = abs(p.y);
            float dmi = d2;
            if( d1 < d2 ) dmi = d1;
            return 0.5f * dmi / scale;
        }
        const char * getName() const override { return "UNKOWN"; };
};