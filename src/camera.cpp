#include "camera.h"
#include "ubo_bindings.h"
#include <glm/gtc/quaternion.hpp>
#include <stdio.h>
const float rotateFactor = 0.0008f;

Camera::Camera(glm::vec3 pos, glm::vec3 front): pos(pos), front(front), speed(0.5f), movement(glm::vec3(0.0f)){
    glGenBuffers(1, &this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW); //pos, front
    glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(UBO::Camera), this->ubo, 0, 2 * sizeof(glm::vec3)); 
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &pos.x);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), &front.x);  
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glm::vec3 Camera::getVelocity() const {
    if(this->movement.x == 0.0f && this->movement.y == 0.0f && this->movement.z == 0.0f) return glm::vec3(0.0);
    glm::vec3 right = glm::cross(this->front, glm::vec3(0, 1, 0));
    glm::vec3 up = glm::cross(right, this->front);
    return glm::normalize(this->movement.x * right + this->movement.y * up + this->movement.z * this->front) * this->speed;
}

void Camera::rotate(glm::vec2 dir){
    float r = glm::length(dir);
    
    glm::vec3 y_axis = glm::vec3(0, dir.x > 0 ? 1 : -1, 0);
    float max_rad =  glm::acos(glm::dot(this->front, y_axis) / glm::length(this->front)) - 0.52359877559; //0.52359877559 = pi / 6
    float rad = glm::min(r * rotateFactor, max_rad);
    
    glm::vec3 right = glm::cross(this->front, glm::vec3(0, 1, 0));
    glm::vec3 up = glm::cross(right, this->front);

    glm::vec3 axis = glm::normalize(dir.x * right + dir.y * up);

    glm::quat q = glm::angleAxis(rad, axis);
    this->front = glm::mat4_cast(q) * glm::vec4(this->front, 1.0);

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), &this->front.x);  
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::move(glm::vec3 d){
    this->pos += d;

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &this->pos.x);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //printf("%f %f %f\n", this->pos.x, this->pos.y, this->pos.z);
    //printf("%f %f %f\n", this->front.x, this->front.y, this->front.z);
}