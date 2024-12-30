#include "mandelbox_renderer.h"

MandelboxRenderer::MandelboxRenderer(Mandelbox *mbox, unsigned height, unsigned width): Renderer("./shaders/fs.glsl", height, width){
    this->mbox = mbox;
    this->scale_location = glGetUniformLocation(this->program, "scale");
    this->minR_location = glGetUniformLocation(this->program, "minR");
    this->fold_location = glGetUniformLocation(this->program, "fold");
}
void MandelboxRenderer::draw(float t) const {
    glUniform1f(this->iTime_location, t);

    glUniform1f(this->scale_location, this->mbox->scale);
    glUniform1f(this->minR_location, this->mbox->minR);
    glUniform1f(this->fold_location, this->mbox->fold);
    glUniform1i(this->julia_location, this->mbox->julia);
    glUniform3fv(this->juliaOffset_location, 1,  &this->mbox->juliaOffset[0]);
    
    glUniform3fv(this->pos_location, 1, &(this->scene->getCamera()->getPos()[0]));
    glUniform3fv(this->front_location, 1,  &(this->scene->getCamera()->getFront()[0]));

    glUniform3fv(this->color_location, 1,  &(this->mbox->color.r));
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}