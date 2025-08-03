#pragma once
#include "fractals/fractal.h"

class UIParameterEditor {
    public:
        UIParameterEditor(IFractal *fractal) : fractal(fractal) {}
        void switchFractal(IFractal *newFractal) { this->fractal = newFractal; }
        void draw() const;
    private:
        IFractal *fractal;
};
