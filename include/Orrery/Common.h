#pragma once
#include <glm/glm.hpp>

namespace Graphics {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
}

namespace Scene {
    struct CelestialBody {
        glm::vec3 position;
        glm::vec3 scale;
        float axisTilt;
        float selfRotationPeriod;
        float orbitPeriod;
    };
}