#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include "config.h"
#include <cmath>
#include <iostream>
static glm::vec3 toVec3(const std::array<float, 3>& v) { return glm::vec3(v[0], v[1], v[2]); }
static glm::vec4 toVec4(const std::array<float, 4>& v) { return glm::vec4(v[0], v[1], v[2], v[3]); }

struct ParameterValue {
    float value, min, max;
};

class IFractal {
public:
    virtual ~IFractal() = default;

    // virtual void updateParameters() = 0;
    // virtual void reset() = 0;
    virtual int getDimension() const = 0; // 3 or 4

    virtual const std::unordered_map<std::string, ParameterValue>& getParameters() const = 0;
    virtual std::unordered_map<std::string, ParameterValue>& getParameters() = 0;

    virtual const glm::vec3& getColor() const = 0;
    virtual glm::vec3& getColor() = 0;

    virtual const bool useJulia() const = 0;
    virtual bool& useJulia() = 0;

    virtual const float* getJuliaOffset() const = 0;
    virtual float* getJuliaOffset() = 0;

    virtual float DE(glm::vec3 p) const = 0; 

    virtual const char * getName() const = 0;
};


template <int Dim>
class AbstractFractal : public IFractal {
public:
    AbstractFractal(): color(glm::vec3(0.0, 0.25, 0.75)){}

    int getDimension() const override { return Dim; }

    const std::unordered_map<std::string, ParameterValue>& getParameters() const override { return this->params; };
    std::unordered_map<std::string, ParameterValue>& getParameters() override { return this->params; };

    const glm::vec3& getColor() const override { return this->color; }
    glm::vec3& getColor() override { return this->color; }

    const bool useJulia() const override { return this->julia; }
    bool& useJulia() override { return this->julia; }

    const float* getJuliaOffset() const override { return &this->juliaOffset[0]; };
    float* getJuliaOffset() override { return &this->juliaOffset[0]; };
protected:
    bool julia = false;
    std::array<float, Dim> juliaOffset{};
    glm::vec3 color;
    std::unordered_map<std::string, ParameterValue> params;
    
};

typedef AbstractFractal<3> Fractal3D;
typedef AbstractFractal<4> Fractal4D;

