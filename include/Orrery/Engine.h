#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef ENGINE_H
#define ENGINE_H

class Engine {
    public: 
        Engine(const char *pWindowLabel, unsigned int windowWidth, unsigned int windowHeight);
        ~Engine();

        int initialize();
        bool isRunning();
        void terminate();
        void processInput();
        GLFWwindow* getWindow();
        float getCurrentDay();
        void onResize(int newWidth, int newHeight);

        static void framebufferSizeCallback(GLFWwindow *pWindow, int width, int height) {
            Engine*pEngine = static_cast<Engine*>(glfwGetWindowUserPointer(pWindow));
            if(pEngine) {
                pEngine->onResize(width, height);
            }
        }

    private:
        GLFWwindow *pWindow_;
        const char *pWindowLabel_;
        unsigned int windowWidth_;
        unsigned int windowHeight_;
        float day_ = 0.0f;
        float inc_ = 0.05f/24.0f;
};

#endif // ENGINE_H