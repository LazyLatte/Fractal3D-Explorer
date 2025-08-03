#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Camera {
    private:
        glm::vec3 pos;
        glm::vec3 front;
        GLuint ubo;
    public:
        float speed;
        glm::vec3 movement;
        Camera(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
        ~Camera() { glDeleteBuffers(1, &this->ubo); }
        void move(glm::vec3 d);
        void rotate(glm::vec2 dir);
        
        glm::vec3 getVelocity() const;
        glm::vec3 getPos() const { return this->pos; }
        glm::vec3 getFront() const { return this->front; }
};
