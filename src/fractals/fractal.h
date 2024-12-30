#ifndef FRACTAL_H
#define FRACTAL_H
#include <glm/glm.hpp>

class Fractal {
    public:
        bool julia;
        glm::vec3 juliaOffset;
        glm::vec3 color;
        Fractal(): julia(false), juliaOffset(glm::vec3(0.0)), color(glm::vec3(0.0, 0.25, 0.75)){}
        virtual float DE(glm::vec3 p) const = 0;
};

#endif /* FRACTAL_H */