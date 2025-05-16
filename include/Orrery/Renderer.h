#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Orrery/Common.h"
#include "Orrery/Shader.h"
#include "Orrery/Mesh.h"

class Renderer {
    private: 
        unsigned int shaderProgram_;
        Mesh sphereMesh_;
        glm::mat4 view_;
        glm::mat4 projection_;
        unsigned int earthTex_;
        unsigned int moonTex_;
        unsigned int sunTex_;
        std::vector<Scene::CelestialBody> celestialBodies_;

        std::vector<Graphics::Vertex> createCubeVertices();
        std::vector<Graphics::Vertex> createSphereVertices(unsigned int longitudeSegments, unsigned int latitudeSegments);
        unsigned int loadTexture(const char* path);

    public:
        Renderer(Mesh mesh, unsigned int shader);
        ~Renderer();

        void initialize();
        void render(float day);
        void setupCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
        void setProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
        void createCelestialBody(glm::vec3 position, glm::vec3 scale, float axisTilt, float selfRotationPeriod, float orbitPeriod);
};

#endif // RENDERER_H