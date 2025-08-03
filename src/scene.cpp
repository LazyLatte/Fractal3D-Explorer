#include "scene.h"
#include <GL/glew.h>

Scene::Scene(IFractal *fractal, Camera *camera, int width, int height): camera(camera), width(width), height(height){
    this->renderer = new Renderer(fractal, width, height);

    glViewport(0, 0, width, height);
}

Scene::~Scene(){
    delete this->renderer;
}

void Scene::switchFractal(IFractal *newFractal){
    this->renderer->switchFractal(newFractal);
}

void Scene::resize(int width, int height){
    this->width = width;
    this->height = height;

    this->renderer->resize(width, height);

    glViewport(0, 0, width, height);
}

void Scene::step(float dt){
    glm::vec3 delta = this->camera->getVelocity() * dt;
    this->camera->move(delta);
    this->renderer->draw(this->width, this->height);
    this->renderer->render2Screen();

    this->camera->speed = abs(this->renderer->getFractal()->DE(this->camera->getPos())) * 3.0f;
}
