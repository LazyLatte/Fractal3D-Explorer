#ifndef MANDEKBOX_H
#define MANDELBOX_H
#include "fractal.h"

class Mandelbox: public Fractal {
    public:
        float scale;
        float minR;
        float fold;
        Mandelbox(): scale(-3.0f), minR(0.5f), fold(1.0f){}
        float DE(glm::vec3 p) const override {
            const int iter = 16;
            const float bailout2 = 64.0; 
            float minRadius2 = this->minR * this->minR;
            float fixedRadius2 = 1.0;
            
            glm::vec3 c = this->julia ? this->juliaOffset : p;
            glm::vec3 v = p;
            float dr = 1.0;
            float r2;  

            for(int i=0; i<iter; i++){
                v = glm::clamp(v, glm::vec3(-1.0), glm::vec3(1.0)) * 2.0f - v;
                v *= this->fold;
                r2 = dot(v, v);
                if(r2 < minRadius2){
                    float factor = fixedRadius2 / minRadius2;
                    v *= factor;
                    dr *= factor;
                }else if(r2 < fixedRadius2){
                    float factor = fixedRadius2 / r2;
                    v *= factor;
                    dr *= factor;
                }

                v = c + this->scale * v;
                dr = dr * abs(this->scale) + 1.0;
                if(r2 > bailout2) break;
            }
            return sqrt(r2) / abs(dr);
        }
};

#endif /* MANDELBOX_H */