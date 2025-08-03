#pragma once
#include "fractal.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

class Sierpinski: public Fractal3D {
    private:
        static constexpr int iter = 16;
        static constexpr float scale = 2.0f;
        static constexpr glm::vec3 offset = glm::vec3(1.0f);
    public:
        Sierpinski(){
            this->params["xDeg"].value = SIERPINSKI_XDEG_DEFAULT_VALUE;
            this->params["xDeg"].min = SIERPINSKI_XDEG_MIN_VALUE;
            this->params["xDeg"].max = SIERPINSKI_XDEG_MAX_VALUE;

            this->params["yDeg"].value = SIERPINSKI_YDEG_DEFAULT_VALUE;
            this->params["yDeg"].min = SIERPINSKI_YDEG_MIN_VALUE;
            this->params["yDeg"].max = SIERPINSKI_YDEG_MAX_VALUE;

            this->params["zDeg"].value = SIERPINSKI_ZDEG_DEFAULT_VALUE;
            this->params["zDeg"].min = SIERPINSKI_ZDEG_MIN_VALUE;
            this->params["zDeg"].max = SIERPINSKI_ZDEG_MAX_VALUE;
        }

        float DE(glm::vec3 p) const override {
            float xDeg = this->params.at("xDeg").value;
            float yDeg = this->params.at("yDeg").value;
            float zDeg = this->params.at("zDeg").value;

            glm::vec3 c = this->julia ? toVec3(this->juliaOffset) : glm::vec3(0.0f);
            glm::vec3 v = p;
            for(int i=0; i<Sierpinski::iter; i++){
                if(v.x+v.y<0.0) v = glm::vec3(-v.y, -v.x, v.z); 
                if(v.x+v.z<0.0) v = glm::vec3(-v.z, v.y, -v.x);
                if(v.y+v.z<0.0) v = glm::vec3(v.x, -v.z, -v.y); 
                v = v * Sierpinski::scale - Sierpinski::offset * (Sierpinski::scale - 1.0f);
                v = glm::rotateX(v, glm::radians(xDeg));
                v = glm::rotateY(v, glm::radians(yDeg));
                v = glm::rotateZ(v, glm::radians(zDeg));
                v += c;
            }
            return glm::length(v)* glm::pow(Sierpinski::scale, -float(Sierpinski::iter));
        }

        const char * getName() const override { return "SIERPINSKI"; };
};


