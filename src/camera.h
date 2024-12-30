#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>

class Scene;
class Camera {
    private:
        glm::vec3 pos;
        glm::vec3 front; //unit vector
        Scene *scene;
    public:
        float speed;
        glm::vec3 dir;
        Camera(){}
        Camera(glm::vec3 pos, glm::vec3 front): pos(pos), front(front), speed(0.5f), dir(glm::vec3(0.0f)){}
        void move(glm::vec3 d);
        void rotate(glm::vec2 dir);
        void rotate(glm::vec3 axis, float rad);
        void setScene(Scene *scene){
            this->scene = scene;
        }
        glm::vec3 getVelocity();
        glm::vec3 getPos() const {
            return this->pos;
        }
        glm::vec3 getFront() const {
            return this->front;
        }
};
#include "scene.h"
#endif