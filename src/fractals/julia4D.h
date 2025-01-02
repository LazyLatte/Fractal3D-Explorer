#ifndef JULIA4D_H
#define JULIA4D_H
#include "fractal.h"
#include <glm/gtc/quaternion.hpp>
static glm::quat qPow(glm::quat q, unsigned power){
    if(power == 1) return q;
    glm::quat q_half_pow = qPow(q, power >> 1);
    if(power % 2){
        return  q_half_pow * q_half_pow * q;
    }
    return q_half_pow * q_half_pow;
}

class Julia4D: public Fractal4D {
    private:
        static constexpr int iter = 24;
        static constexpr float bailout = 64.0f;
    public:
        unsigned power;
        float qk;
        Julia4D(): power(2), qk(0.2f){
            this->julia = true;
            this->juliaOffset = glm::quat(-0.1f, 0.6f, 0.9f,-0.3f); // (w, x, y, z) ([3], [0], [1], [2])
        }
        float DE(glm::vec3 p) const override {
            glm::quat q = glm::quat(p.x, p.y, p.z, this->qk);
            glm::quat c = this->julia ? this->juliaOffset : q;
            float dr2 = 1.0f;             
            float r2 = glm::dot(q, q);  
            for (int i = 0; i < Julia4D::iter; i++) {
                glm::quat q_pow_minus_one = qPow(q, this->power - 1);
                dr2 *= (this->power * this->power * glm::dot(q_pow_minus_one, q_pow_minus_one));
                q = q * q_pow_minus_one + c;
                
                r2 = glm::dot(q, q);
                if(r2 > Julia4D::bailout) break;
            }
            return 0.25f * glm::log(r2) * glm::sqrt(r2 / dr2);
        }
};

#endif /* JULIA4D_H */