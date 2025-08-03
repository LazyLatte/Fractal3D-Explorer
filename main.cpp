#include <fstream>
#include <iostream>
#include <limits>
#include <iomanip>
#include <cassert>
#include <vector>

extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scene.h"
#include "fractals/mandelbulb.h"
#include "fractals/mandelbox.h"
#include "fractals/menger.h"
#include "fractals/sierpinski.h"
#include "fractals/julia4d.h"
#include "fractals/unkown.h"
#include "ui.h"
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int height, int width);

Mandelbox *mbox;
Mandelbulb *mbulb;
Menger *menger;
Sierpinski *sierpinski;
Julia4D *julia4D;
Camera *camera;
Scene *scene;

Unkown *unkown;
const GLuint WIDTH = 1080, HEIGHT = 720;
double cursor_prev_xpos, cursor_prev_ypos;

const char* GetFractalName(void* data, int idx) {
    auto frac = static_cast<IFractal**>(data);
    return frac[idx]->getName();
}


int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fractal3D", NULL, NULL);    
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

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
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

    mbulb = new Mandelbulb();
    mbox = new Mandelbox();
    menger = new Menger();
    sierpinski = new Sierpinski();
    julia4D = new Julia4D();
    unkown = new Unkown();
    IFractal* fractals[] = { mbulb, mbox, menger, sierpinski, julia4D, unkown};
    static int current_fractal = 5;
    camera = new Camera(glm::vec3(0.0, 5.0, 5.0), glm::normalize(glm::vec3(0.0, -1.0, -1.0)));
    scene = new Scene(fractals[current_fractal], camera, WIDTH, HEIGHT);
    float prevTime = 0.0;

    UIParameterEditor ui(fractals[current_fractal]);
    while (!glfwWindowShouldClose(window)){
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {   
            ImGui::SetNextWindowSize(ImVec2(300, 500));
            ImGui::Begin("Setting", NULL);

            if (ImGui::Combo("Combo", &current_fractal, GetFractalName, (void*)&fractals, IM_ARRAYSIZE(fractals))) {
                scene->switchFractal(fractals[current_fractal]);
                ui.switchFractal(fractals[current_fractal]);
            }

            ui.draw();

            ImGui::End();
        }
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = (float)glfwGetTime();

        float dt = timeValue - prevTime;
        scene->step(dt);
        
        prevTime = timeValue;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    delete mbox;
    delete mbulb;
    delete menger;
    delete sierpinski;
    delete julia4D;
    delete unkown;
    delete camera;
    delete scene;
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_W: {
                camera->movement.z++;
                break;
            }
            case GLFW_KEY_S: {
                camera->movement.z--;
                break;
            }
            case GLFW_KEY_A: {
                camera->movement.x--;
                break;
            }
            case GLFW_KEY_D: {
                camera->movement.x++;
                break;
            }
            case GLFW_KEY_E: {
                camera->movement.y++;
                break;
            }
            case GLFW_KEY_C: {
                camera->movement.y--;
                break;
            }
            case GLFW_KEY_P: {
                glm::vec3 camera_pos = camera->getPos();
                glm::vec3 camera_front = camera->getFront();
                std::cout << "Pos: (" << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << ")" << std::endl;
                std::cout << "Dir: (" << camera_front.x << ", " << camera_front.y << ", " << camera_front.z << ")" << std::endl;
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
                camera->movement.z--;
                break;
            }
            case GLFW_KEY_S: {
                camera->movement.z++;
                break;
            }
            case GLFW_KEY_A: {
                camera->movement.x++;
                break;
            }
            case GLFW_KEY_D: {
                camera->movement.x--;
                break;
            }
            case GLFW_KEY_E: {
                camera->movement.y--;
                break;
            }
            case GLFW_KEY_C: {
                camera->movement.y++;
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
    scene->resize(width, height);
}