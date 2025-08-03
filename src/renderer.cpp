#include "renderer.h"

Renderer::Renderer(IFractal* fractal, int width, int height): fractal(fractal){
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    this->fractalShader = new Shader(fractal->getName());

    this->fractalShader->setLocation("color_seed");
    this->fractalShader->setLocation("julia");
    this->fractalShader->setLocation("juliaOffset");

    for(const auto& [key, value] : fractal->getParameters()){
        this->fractalShader->setLocation(key.c_str());
    }

    this->screenShader = new Shader("screen.vert", "screen.frag");
    glGenVertexArrays(1, &this->vao);
}

void Renderer::resize(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::draw(int width, int height) const {

    this->fractalShader->use();

    this->fractalShader->setVec3("color_seed", &this->fractal->getColor().x);

    this->fractalShader->setBool("julia", this->fractal->useJulia());

    int dim = this->fractal->getDimension();
    if(dim == 3){
        this->fractalShader->setVec3("juliaOffset", this->fractal->getJuliaOffset());
    }else if(dim == 4){
        this->fractalShader->setVec4("juliaOffset", this->fractal->getJuliaOffset());
    }

    for(const auto& [key, value] : this->fractal->getParameters()){
        this->fractalShader->setFloat(key.c_str(), value.value);
    }

    glBindImageTexture(0, this->texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
    
    glDispatchCompute((width + 15) >> 4, (height + 15) >> 4, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Renderer::render2Screen() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->screenShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::switchFractal(IFractal *newFractal){
    delete this->fractalShader;
    this->fractalShader = new Shader(newFractal->getName());

    this->fractalShader->setLocation("color_seed");
    this->fractalShader->setLocation("julia");
    this->fractalShader->setLocation("juliaOffset");

    for(const auto& [key, value] : newFractal->getParameters()){
        this->fractalShader->setLocation(key.c_str());
    }
    
    this->fractal = newFractal;
}