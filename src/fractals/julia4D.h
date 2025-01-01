#ifndef JULIA4D_H
#define JULIA4D_H
#include "fractal.h"
#include <glm/gtc/quaternion.hpp>
#include <cstdio>
static glm::quat qPow(glm::quat q, unsigned power){
    if(power == 1) return q;
    glm::quat q_half_pow = qPow(q, power >> 1);
    if(power % 2){
        return  q_half_pow * q_half_pow * q;
    }
    return q_half_pow * q_half_pow;
}
static glm::vec4 qMul(glm::vec4 q1, glm::vec4 q2){
    return glm::vec4(
        q1.x * q2.x - q1.y * q2.y - q1.z * q2.z - q1.w * q2.w,
        q1.x * q2.y + q1.y * q2.x + q1.z * q2.w - q1.w * q2.z,
        q1.x * q2.z - q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
        q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x
    );
}
class Julia4D: public Fractal4D {
    public:
        unsigned power;
        float qk;
        Julia4D(): power(2), qk(0.0f){
            this->julia = true;
            this->juliaOffset = glm::quat(-0.1f, 0.6f, 0.9f,-0.3f); // (i, j, k, 1)
            glm::quat q = glm::quat(this->juliaOffset.x, this->juliaOffset.y, this->juliaOffset.z, this->juliaOffset.w);
            glm::quat test = q * q;
            printf("%f %f %f %f\n", test.x, test.y, test.z, test.w);
            glm::vec4 q_to_v = glm::vec4(this->juliaOffset.x, this->juliaOffset.y, this->juliaOffset.z, this->juliaOffset.w);
            glm::vec4 test2 = qMul(q_to_v, q_to_v);
            printf("%f %f %f %f\n", test2.x, test2.y, test2.z, test2.w);

        }
        float DE(glm::vec3 p) const override {
            const int iter = 24;
            const float bailout = 64.0f;
            glm::quat v = glm::quat(p.y, p.z, this->qk, p.x);
            glm::quat c = this->julia ? this->juliaOffset : v;
            float dr2 = 1.0f;             
            float r2 = glm::dot(v, v);  
            for (int i = 0; i < iter; i++) {
                glm::quat v_pow_minus_one = qPow(v, this->power - 1);
                dr2 *= (this->power * this->power * glm::dot(v_pow_minus_one, v_pow_minus_one));
                v = v * v_pow_minus_one + c;
                
                r2 = glm::dot(v, v);
                if(r2 > bailout) break;
            }
            return 0.25f * glm::log(r2) * glm::sqrt(r2 / dr2);
        }
};

#endif /* JULIA4D_H */