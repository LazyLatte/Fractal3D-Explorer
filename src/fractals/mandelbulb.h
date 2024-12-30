#ifndef MANDELBULB_H
#define MANDELBULB_H
#include "fractal.h"

class Mandelbulb: public Fractal {
    public:
        float power;
        Mandelbulb(): power(8.0f){}
        float DE(glm::vec3 p) const override {
            const int iter = 9;
            const float bailout = 2.0f;
            glm::vec3 c = this->julia ? this->juliaOffset : p;
            glm::vec3 v = p;
            float dr = 1.0f;             
            float r = length(v);  
            
            for (int i = 0; i < iter; i++) {
                float r_pow_n_minus_one = pow(r, power - 1.0f);
                float r_pow_n = r * r_pow_n_minus_one;
                float theta = glm::atan(v.y, v.x) * power;
                float phi = glm::asin(v.z / r) * power;
                dr = power * r_pow_n_minus_one * dr + 1.0;
                v = c + r_pow_n * glm::vec3(glm::cos(theta) * glm::cos(phi), glm::cos(phi) * glm::sin(theta), -glm::sin(phi));

                r = length(v);     
                if (r > bailout) break;  
            }
            
            return 0.5 * glm::log(r) * r / dr;
        }
};

#endif /* MANDELBULB_H */