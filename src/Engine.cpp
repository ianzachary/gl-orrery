#include "Orrery/Engine.h"

Engine::Engine(const char *pWindowLabel, unsigned int windowWidth, unsigned int windowHeight) 
: pWindow_(nullptr), pWindowLabel_(pWindowLabel), windowWidth_(windowWidth), windowHeight_(windowHeight) {}

Engine::~Engine() {}

int Engine::initialize() {
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    pWindow_ = glfwCreateWindow(windowWidth_, windowHeight_, pWindowLabel_, nullptr, nullptr);

    glfwMakeContextCurrent(pWindow_);
    glfwSetFramebufferSizeCallback(pWindow_, framebufferSizeCallback);

    glEnable(GL_DEPTH_TEST);

    return 0;
}

bool Engine::isRunning() {
    return !glfwWindowShouldClose(pWindow_);
}

void Engine::terminate() {
    glfwDestroyWindow(pWindow_);
    glfwTerminate();
}

void Engine::processInput() {
    if (glfwGetKey(pWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow_, true);
    }
}

GLFWwindow* Engine::getWindow() {
    return pWindow_;
}

float Engine::getCurrentDay() {
    return day_ += inc_;
}

void Engine::onResize(int newWidth, int newHeight) {
    windowWidth_ = newWidth;
    windowHeight_ = newHeight;
    glViewport(0, 0, newWidth, newHeight);
}