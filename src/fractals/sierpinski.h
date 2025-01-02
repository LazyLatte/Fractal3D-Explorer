#ifndef SIERPINSKI_H
#define SIERPINSKI_H
#include "fractal.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

class Sierpinski: public Fractal3D {
    private:
        static constexpr int iter = 16;
        static constexpr float scale = 2.0f;
        static constexpr glm::vec3 offset = glm::vec3(1.0f);
    public:
        float xDeg;
        float yDeg;
        float zDeg;
        Sierpinski(): xDeg(0.0f), yDeg(0.0f), zDeg(0.0f){}
        float DE(glm::vec3 p) const override {
            glm::vec3 c = this->julia ? this->juliaOffset : glm::vec3(0.0f);
            glm::vec3 v = p;
            for(int i=0; i<Sierpinski::iter; i++){
                if(v.x+v.y<0.0) v = glm::vec3(-v.y, -v.x, v.z); 
                if(v.x+v.z<0.0) v = glm::vec3(-v.z, v.y, -v.x);
                if(v.y+v.z<0.0) v = glm::vec3(v.x, -v.z, -v.y); 
                v = v * Sierpinski::scale - Sierpinski::offset * (Sierpinski::scale - 1.0f);
                v = glm::rotateX(v, glm::radians(this->xDeg));
                v = glm::rotateY(v, glm::radians(this->yDeg));
                v = glm::rotateZ(v, glm::radians(this->zDeg));
                v += c;
            }
            return glm::length(v)* glm::pow(Sierpinski::scale, -float(Sierpinski::iter));
        }
};

#endif /* SIERPINSKI_H */


