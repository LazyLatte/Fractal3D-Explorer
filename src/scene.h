#ifndef SCENE_H
#define SCENE_H
#include "camera.h"
#include "fractals/fractal.h"
#include "renderers/renderer.h"

class Scene {
    private:
        Camera *camera;
        Renderer *renderer;
    public:
        Scene(): camera(NULL){}
        Scene(Camera *camera, Renderer *renderer): camera(camera), renderer(renderer){
            this->renderer->setScene(this);
        }

        void step(float t, float dt){
            glm::vec3 delta = this->camera->getVelocity() * dt;
            this->camera->move(delta);
            this->renderer->draw(t);
            this->camera->speed = abs(this->renderer->DE(this->camera->getPos())) * 1.5f;
            
        }
        void showUniformParametersUI(){
            this->renderer->uniformParametersUI();
        }
        Camera *getCamera() const {
            return this->camera;
        }
};

#endif