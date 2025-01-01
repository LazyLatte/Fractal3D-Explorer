#ifndef FRACTAL_H
#define FRACTAL_H
#include <glm/glm.hpp>

template <typename T>
class Fractal {
    public:
        bool julia;
        T juliaOffset;
        glm::vec3 color;
        Fractal(): julia(false), juliaOffset(T()), color(glm::vec3(0.0, 0.25, 0.75)){}
        virtual float DE(glm::vec3 p) const = 0;
};

typedef Fractal<glm::vec3> Fractal3D;
typedef Fractal<glm::quat> Fractal4D;
#endif /* FRACTAL_H */