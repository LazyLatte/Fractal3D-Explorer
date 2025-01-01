#ifndef MENGER_H
#define MENGER_H
#include "fractal.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
class Menger: public Fractal3D {
    public:
        float xDeg;
        float yDeg;
        float zDeg;
        Menger(): xDeg(0.0f), yDeg(0.0f), zDeg(0.0f){}
        float DE(glm::vec3 p) const override {
            const int iter = 32;
            const float bailout = 2.0f;
            const float scale = 3.0f;
            const glm::vec3 offset = glm::vec3(1.0f);
            glm::vec3 c = this->julia ? this->juliaOffset : glm::vec3(0.0f);

            glm::vec3 v = p;
            float s = 1.0f;
            for(int i=0; i<iter; i++){
                v = glm::abs(v);  
                if(v.x > bailout || v.y > bailout || v.z > bailout) break;
                if(v.x < v.y) v.xy = v.yx;
                if(v.x < v.z) v.xz = v.zx;
                if(v.y < v.z) v.yz = v.zy;
                s *= scale;
                v = v * scale - offset * (scale - 1.0f);
                float zVal = offset.z * (scale - 1.0f);
                if(v.z < -0.5f * zVal) v.z += zVal;
                v = glm::rotateX(v, glm::radians(this->xDeg));
                v = glm::rotateY(v, glm::radians(this->yDeg));
                v = glm::rotateZ(v, glm::radians(this->zDeg));
                v += c;
                
            }
            return this->sdBox(v, glm::vec3(1.0f)) / s;
        }
        float sdBox(glm::vec3 p, glm::vec3 b) const {
            glm::vec3 d = glm::abs(p) - b;
            return glm::length(glm::max(d, 0.0f)) + glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f);
        }
};

#endif /* MENGER_H */