#pragma once
#include "camera.h"
#include "renderer.h"

class Scene {
    private:
        Camera *camera;
        Renderer *renderer;
        int width, height;
    public:
        Scene(IFractal *fractal, Camera *camera, int width, int height);
        ~Scene();
        void switchFractal(IFractal *newFractal);
        void resize(int width, int height);
        void step(float dt);
};