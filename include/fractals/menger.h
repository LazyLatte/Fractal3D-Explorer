#pragma once
#include "fractal.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

float opUnion(float a, float b)       { return glm::min(a, b); }
float opIntersection(float a, float b){ return glm::max(a, b); }
float opDifference(float a, float b)  { return glm::max(a, -b); }

class Menger: public Fractal3D {
    private:
        static constexpr int iter = 32;
        static constexpr float bailout = 2.0f;
        static constexpr float scale = 3.0f;
        static constexpr glm::vec3 offset = glm::vec3(1.0f);
    public:
        Menger(){
            this->params["xDeg"].value = MENGER_XDEG_DEFAULT_VALUE;
            this->params["xDeg"].min = MENGER_XDEG_MIN_VALUE;
            this->params["xDeg"].max = MENGER_XDEG_MAX_VALUE;

            this->params["yDeg"].value = MENGER_YDEG_DEFAULT_VALUE;
            this->params["yDeg"].min = MENGER_YDEG_MIN_VALUE;
            this->params["yDeg"].max = MENGER_YDEG_MAX_VALUE;

            this->params["zDeg"].value = MENGER_ZDEG_DEFAULT_VALUE;
            this->params["zDeg"].min = MENGER_ZDEG_MIN_VALUE;
            this->params["zDeg"].max = MENGER_ZDEG_MAX_VALUE;
        }

        float sdBox(glm::vec3 p, glm::vec3 b) const {
            glm::vec3 d = glm::abs(p) - b;
            return glm::length(glm::max(d, 0.0f)) + glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f);
        }

        float DE(glm::vec3 p) const override {
            float xDeg = this->params.at("xDeg").value;
            float yDeg = this->params.at("yDeg").value;
            float zDeg = this->params.at("zDeg").value;

            glm::vec3 c = this->julia ? toVec3(this->juliaOffset) : glm::vec3(0.0f);
            glm::vec3 v = p;
            float s = 1.0f;
            for(int i=0; i<Menger::iter; i++){
                v = glm::abs(v);  
                if(v.x > Menger::bailout || v.y > Menger::bailout || v.z > Menger::bailout) break;
                if(v.x < v.y) v = glm::vec3(v.y, v.x, v.z);
                if(v.x < v.z) v = glm::vec3(v.z, v.y, v.x);
                if(v.y < v.z) v = glm::vec3(v.x, v.z, v.y);
                s *= Menger::scale;
                v = v * Menger::scale - Menger::offset * (Menger::scale - 1.0f);
                float zVal = Menger::offset.z * (Menger::scale - 1.0f);
                if(v.z < -0.5f * zVal) v.z += zVal;
                v = glm::rotateX(v, glm::radians(xDeg));
                v = glm::rotateY(v, glm::radians(yDeg));
                v = glm::rotateZ(v, glm::radians(zDeg));
                v += c;
            }
            return this->sdBox(v, glm::vec3(1.0f)) / s;
        }
        const char * getName() const override { return "MENGER"; };
};