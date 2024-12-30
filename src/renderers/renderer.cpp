#include "renderer.h"
#include <iostream>
Renderer::Renderer(const char *fs_filename, int height, int width){
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string shaderDir = "./shaders/";
    std::string shaderSource = readShaderCode(std::string(shaderDir + "vs.glsl").c_str());
    const GLchar* s[1];
    s[0] = shaderSource.c_str();
    glShaderSource(vertexShader, 1, s, NULL);

    shaderSource = readShaderCode(std::string(shaderDir + fs_filename).c_str());
    s[0] = shaderSource.c_str();
    glShaderSource(fragmentShader, 1, s, NULL);
    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    glUseProgram(this->program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float positions[8] = {
        -1, -1,
        1, -1,
        1, 1,
        -1, 1
    };
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLint iRes_location = glGetUniformLocation(this->program, "iResolution");
    glUniform2f(iRes_location, (GLfloat)width, (GLfloat)height);

    this->pos_location = glGetUniformLocation(this->program, "pos");
    this->front_location = glGetUniformLocation(this->program, "front");

    this->julia_location = glGetUniformLocation(this->program, "julia");
    this->juliaOffset_location = glGetUniformLocation(this->program, "juliaOffset");

    this->color_location = glGetUniformLocation(this->program, "color_seed");
    this->iTime_location = glGetUniformLocation(this->program, "iTime");
}

void Renderer::draw(float t) const {
    glUseProgram(this->program);
    glUniform1f(this->iTime_location, t);
    glUniform3fv(this->pos_location, 1, &(this->scene->getCamera()->getPos()[0]));
    glUniform3fv(this->front_location, 1,  &(this->scene->getCamera()->getFront()[0]));

    this->setUniformParameters();
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
void Renderer::setScene(Scene *scene){
    this->scene = scene;
}

std::string readShaderCode(const char *filename){
    std::ifstream in;
    in.open(filename);
    if(!in.is_open()){
        std::cout << "Failed to open file.\n";
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    );
}
