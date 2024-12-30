#include "camera.h"
#include <stdio.h>
const float rotateFactor = 0.0008f;

glm::vec3 Camera::getVelocity(){
    if(this->dir.x == 0.0f && this->dir.y == 0.0f && this->dir.z == 0.0f) return glm::vec3(0.0);
    glm::vec3 right = glm::cross(this->front, glm::vec3(0, 1, 0));
    glm::vec3 up = glm::cross(right, this->front);
    return glm::normalize(this->dir.x * right + this->dir.y * up + this->dir.z * this->front) * this->speed;
}
void Camera::move(glm::vec3 d){
    this->pos += d;
}

void Camera::rotate(glm::vec2 dir){
    float r = glm::length(dir);
    
    glm::vec3 y_axis = glm::vec3(0, dir.x > 0 ? 1 : -1, 0);
    float max_rad =  glm::acos(glm::dot(this->front, y_axis) / glm::length(front)) - 0.52359877559; //0.52359877559 = pi / 6
    float rad = glm::min(r * rotateFactor, max_rad);
    
    glm::vec3 right = glm::cross(this->front, glm::vec3(0, 1, 0));
    glm::vec3 up = glm::cross(right, this->front);

    glm::vec3 axis = glm::normalize(dir.x * right + dir.y * up);

    glm::quat q = glm::angleAxis(rad, axis);
    this->front = glm::mat4_cast(q) * glm::vec4(this->front, 1.0);
}

void Camera::rotate(glm::vec3 axis, float rad){
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0), rad, axis);
    this->pos = rotation_matrix * glm::vec4(this->pos, 1.0);
    this->front = glm::normalize(-this->pos);
}