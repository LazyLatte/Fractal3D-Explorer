#include <fstream>
#include <iostream>
#include <limits>
#include <iomanip>
#include <cassert>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scene.h"
#include "renderers/mandelbox_renderer.h"
#include "renderers/mandelbulb_renderer.h"
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int height, int width);

Mandelbox *mbox;
MandelboxRenderer *mbox_renderer;
Mandelbulb *mbulb;
MandelbulbRenderer *mbulb_renderer;
Camera *camera;
Scene *scene;
const GLuint WIDTH = 640, HEIGHT = 360;
double cursor_prev_xpos, cursor_prev_ypos;
int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbox", NULL, NULL);    
    if (!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    
    
    //framebuffer_size_callback(window, WIDTH, HEIGHT);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    //camera = new Camera(glm::vec3(0.5, 0, -0.25), glm::vec3(0.5, 0, -0.8));
    mbox = new Mandelbox();
    mbox_renderer = new MandelboxRenderer(mbox, HEIGHT, WIDTH);

    mbulb = new Mandelbulb();
    mbulb_renderer = new MandelbulbRenderer(mbulb, HEIGHT, WIDTH);
    camera = new Camera(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 0.0, -1.0));
    //scene = new Scene(mbox, camera, mbox_renderer);
    scene = new Scene(mbulb, camera, mbulb_renderer);
    float prevTime = 0.0;
    //std::cout << std::setprecision(15) << static_cast<float>((float)2.00000429153442 + (float)-1.04987442739457e-07) <<std::endl;
    //std::cout << std::setprecision(15) << static_cast<double>((double)2.00000429153442 + (double)-1.04987442739457e-07) <<std::endl;
    float curTime = -1.0f;
    while (!glfwWindowShouldClose(window)){
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {   
            ImGui::SetNextWindowSize(ImVec2(300, 500));
            ImGui::Begin("Setting", NULL);
            ImGui::SliderFloat("scale", &mbox->scale, -3, 3);
            ImGui::SliderFloat("minR", &mbox->minR, 0, 1);
            ImGui::SliderFloat("fold",&mbox->fold, 0, 2);

            ImGui::SliderFloat("r", &mbox->color.r, 0.0, 1.0);
            ImGui::SliderFloat("g", &mbox->color.g, 0.0, 1.0);
            ImGui::SliderFloat("b",&mbox->color.b, 0.0, 1.0);
            ImGui::End();
        }
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = (float)glfwGetTime();
        //camera->rotate(glm::vec3(0, 1, 0), timeValue - prevTime);
        if(curTime < 0.0){
            curTime = 0.0;
        }else{
            curTime += (timeValue - prevTime);
        }
        scene->step(curTime, timeValue - prevTime);
        
        prevTime = timeValue;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete mbox;
    delete camera;
    delete mbox_renderer;
    delete scene;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_W: {
                camera->dir.z++;
                break;
            }
            case GLFW_KEY_S: {
                camera->dir.z--;
                break;
            }
            case GLFW_KEY_A: {
                camera->dir.x--;
                break;
            }
            case GLFW_KEY_D: {
                camera->dir.x++;
                break;
            }
            case GLFW_KEY_E: {
                camera->dir.y++;
                break;
            }
            case GLFW_KEY_C: {
                camera->dir.y--;
                break;
            }
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            default:
                break;
        }
    }
    if(action == GLFW_RELEASE){
        switch(key){
            case GLFW_KEY_W: {
                camera->dir.z--;
                break;
            }
            case GLFW_KEY_S: {
                camera->dir.z++;
                break;
            }
            case GLFW_KEY_A: {
                camera->dir.x++;
                break;
            }
            case GLFW_KEY_D: {
                camera->dir.x--;
                break;
            }
            case GLFW_KEY_E: {
                camera->dir.y--;
                break;
            }
            case GLFW_KEY_C: {
                camera->dir.y++;
                break;
            }
            default:
                break;
        }
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    const auto& io = ImGui::GetIO(); 
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse){
        int cursor_mode = glfwGetInputMode(window, GLFW_CURSOR);
        glfwSetInputMode(window, GLFW_CURSOR, cursor_mode == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(window, &cursor_prev_xpos, &cursor_prev_ypos);
    }
        
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
        double dx = xpos - cursor_prev_xpos;
        double dy = ypos - cursor_prev_ypos;
        camera->rotate(glm::vec2(-dy, -dx));
        cursor_prev_xpos = xpos;
        cursor_prev_ypos = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}