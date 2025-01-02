#ifndef MENGER_H
#define MENGER_H
#include "fractal.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <cstdio>
static float sdBox(glm::vec3 p, glm::vec3 b){
    glm::vec3 d = glm::abs(p) - b;
    return glm::length(glm::max(d, 0.0f)) + glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f);
}
class Menger: public Fractal3D {
    private:
        static constexpr int iter = 32;
        static constexpr float bailout = 2.0f;
        static constexpr float scale = 3.0f;
        static constexpr glm::vec3 offset = glm::vec3(1.0f);
    public:
        float xDeg;
        float yDeg;
        float zDeg;
        Menger(): xDeg(0.0f), yDeg(0.0f), zDeg(0.0f){}
        float DE(glm::vec3 p) const override {
            glm::vec3 c = this->julia ? this->juliaOffset : glm::vec3(0.0f);
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
                v = glm::rotateX(v, glm::radians(this->xDeg));
                v = glm::rotateY(v, glm::radians(this->yDeg));
                v = glm::rotateZ(v, glm::radians(this->zDeg));
                v += c;
            }
            return sdBox(v, glm::vec3(1.0f)) / s;
        }
};

#endif /* MENGER_H */