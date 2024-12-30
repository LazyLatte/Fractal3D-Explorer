#ifndef SCENE_H
#define SCENE_H
#include "camera.h"
#include "fractals/fractal.h"
#include "renderers/renderer.h"

class Scene {
    private:
        Fractal *fractal;
        Camera *camera;
        Renderer *renderer;
    public:
        Scene(): fractal(NULL), camera(NULL){}
        Scene(Fractal *fractal, Camera *camera, Renderer *renderer): fractal(fractal), camera(camera), renderer(renderer){
            this->renderer->setScene(this);
        }

        void step(float t, float dt){
            glm::vec3 delta = this->camera->getVelocity() * dt;
            this->camera->move(delta);
            this->renderer->draw(t);
            this->camera->speed = abs(this->fractal->DE(this->camera->getPos())) * 1.5f;
            
        }
        Camera *getCamera() const {
            return this->camera;
        }
        Fractal *getFractal() const {
            return this->fractal;
        }
};

#endif