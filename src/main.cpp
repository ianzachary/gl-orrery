#include "Orrery/Engine.h"
#include "Orrery/Shader.h"
#include "Orrery/Renderer.h"
#include "Orrery/Mesh.h"

const char *WINDOW_LABEL = "GL ORRERY";
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;

int main() {
    Engine engine(WINDOW_LABEL, SCR_WIDTH, SCR_HEIGHT);
    char const *pVertexShaderSrc = "assets/shaders/3.3.shader.vert";
    char const *pFragmentShaderSrc = "assets/shaders/3.3.shader.frag";
    glm::vec3 cameraPos(30.0f, 20.0f, 90.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    if (engine.initialize()) {
        return -1;
    }
    
    Shader shader(pVertexShaderSrc, pFragmentShaderSrc);
    Mesh cubeMesh;

    Renderer renderer(cubeMesh, shader.getID());
    renderer.initialize();
    renderer.setupCamera(cameraPos, cameraTarget, cameraUp);
    renderer.setProjection(fov, aspectRatio, nearPlane, farPlane);
    renderer.createCelestialBody( // Sun
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(10.0f, 10.0f, 10.0f),
        0.0f,
        27.0f,
        0.0f
    );
    renderer.createCelestialBody( // Earth
        glm::vec3(40.0f, 0.0f, 0.0f),
        glm::vec3(4.0f, 4.0f, 4.0f),
        -23.5f,
        1.0f,
        365.0f
    );
    renderer.createCelestialBody( // Moon
        glm::vec3(12.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 2.0f, 2.0f),
        6.7f,
        28.0f,
        28.0f
    );

    while(engine.isRunning()) {
        engine.processInput();
        float day = engine.getCurrentDay();

        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        renderer.render(day);

        glfwSwapBuffers(engine.getWindow());
        glfwPollEvents();
    }
    engine.terminate();
    std::cout << "Simulation terminated." << std::endl;

    return 0;
}
