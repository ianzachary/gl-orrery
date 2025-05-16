#include "Orrery/Renderer.h"

Renderer::Renderer(Mesh mesh, unsigned int shader) : sphereMesh_(mesh), shaderProgram_(shader){}
Renderer::~Renderer() {}

void Renderer::initialize() {
    auto vertices = this->createSphereVertices(100, 100);
    sphereMesh_.setVertices(vertices);
    earthTex_ = loadTexture("assets/textures/earth-day.jpg");
    moonTex_ = loadTexture("assets/textures/moon.jpg");
    sunTex_ = loadTexture("assets/textures/sun.jpg");

    sphereMesh_.initialize();
}

void Renderer::render(float day) {
    unsigned int viewLoc = glGetUniformLocation(shaderProgram_, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram_, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_));

    glm::mat4 sunModel = glm::mat4(1.0f);
    const auto& sun = celestialBodies_[0];
    float sunRotationAngle = (360.0f / sun.selfRotationPeriod) * day;
    sunModel = glm::translate(sunModel, sun.position);
    sunModel = glm::rotate(sunModel, glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    sunModel = glm::scale(sunModel, sun.scale);
    unsigned int modelLoc = glGetUniformLocation(shaderProgram_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sunModel));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTex_);
    glUniform1i(glGetUniformLocation(shaderProgram_, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram_, "isSun"), GL_TRUE);
    sphereMesh_.draw();


    glm::mat4 earthModel = glm::mat4(1.0f);
    const auto& earth = celestialBodies_[1];
    float earthOrbitAngle = (360.0f / earth.orbitPeriod) * day;
    float earthRotationAngle = (360.0f / earth.selfRotationPeriod) * day;
    earthModel = glm::rotate(earthModel, glm::radians(earthOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    earthModel = glm::translate(earthModel, earth.position);
    earthModel = glm::rotate(earthModel, glm::radians(-earthOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    earthModel = glm::rotate(earthModel, glm::radians(earth.axisTilt), glm::vec3(0.0f, 0.0f, 1.0f));
    earthModel = glm::rotate(earthModel, glm::radians(earthRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    earthModel = glm::scale(earthModel, earth.scale);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthModel));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTex_);
    glUniform1i(glGetUniformLocation(shaderProgram_, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram_, "isSun"), GL_FALSE);
    sphereMesh_.draw();


    glm::mat4 moonModel = glm::mat4(1.0f);
    const auto& moon = celestialBodies_[2];
    float moonOrbitAngle = (360.0f / moon.orbitPeriod) * day;
    float moonRotationAngle = (360.0f / moon.selfRotationPeriod) * day;
    moonModel = glm::rotate(moonModel, glm::radians(earthOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    moonModel = glm::translate(moonModel, earth.position);
    moonModel = glm::rotate(moonModel, glm::radians(-earthOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    moonModel = glm::rotate(moonModel, glm::radians(moonOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    moonModel = glm::translate(moonModel, moon.position);
    moonModel = glm::rotate(moonModel, glm::radians(-moonOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    moonModel = glm::rotate(moonModel, glm::radians(moon.axisTilt), glm::vec3(0.0f, 0.0f, 1.0f));
    moonModel = glm::rotate(moonModel, glm::radians(moonRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    moonModel = glm::scale(moonModel, moon.scale);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moonModel));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTex_);
    glUniform1i(glGetUniformLocation(shaderProgram_, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram_, "isSun"), GL_FALSE);
    sphereMesh_.draw();

}

void Renderer::setupCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    view_ = glm::lookAt(position, target, up);
}

void Renderer::setProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projection_ = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Renderer::createCelestialBody(glm::vec3 position, glm::vec3 scale, float axisTilt, float selfRotationPeriod, float orbitPeriod) {
    Scene::CelestialBody body;
    body.position = position;
    body.scale = scale;
    body.axisTilt = axisTilt;
    body.selfRotationPeriod = selfRotationPeriod;
    body.orbitPeriod = orbitPeriod;

    celestialBodies_.push_back(body);
}

std::vector<Graphics::Vertex> Renderer::createCubeVertices() {
    std::vector<Graphics::Vertex> vertices;
    
    // Front face (red)
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});

    // Back face (green)
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});

    // Left face (blue)
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f)});

    // Right face (cyan)
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 1.0f)});

    // Top face (magenta)
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f)});

    // Bottom face (yellow)
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back(Graphics::Vertex{glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});

    return vertices;
}

std::vector<Graphics::Vertex> Renderer::createSphereVertices(unsigned int longitudeSegments, unsigned int latitudeSegments) {
    std::vector<Graphics::Vertex> grid, vertices;
    for (unsigned int y = 0; y <= latitudeSegments; ++y) {
        float v = float(y) / latitudeSegments;
        float theta = v * glm::pi<float>();

        for (unsigned int x = 0; x <= longitudeSegments; ++x) {
            float u = float(x) / longitudeSegments;
            float phi = u * glm::two_pi<float>();
            float sinT = sin(theta);

            glm::vec3 pos { sinT * cos(phi), cos(theta), sinT * sin(phi) };
            glm::vec2 uv { u, 1.0f - v };
            glm::vec3 normal = glm::normalize(pos);

            grid.push_back({ pos, normal, uv });
        }
    }
    
    unsigned int lon = 100, lat = 100;
    for (unsigned int y = 0; y < lat; ++y) {
        for (unsigned int x = 0; x < lon; ++x) {
            int i0 =  y    * (lon + 1) + x;
            int i1 = (y+1) * (lon + 1) + x;
            int i2 =  y    * (lon + 1) + (x+1);
            int i3 = (y+1) * (lon + 1) + (x+1);

            vertices.push_back(grid[i0]);
            vertices.push_back(grid[i1]);
            vertices.push_back(grid[i2]);

            vertices.push_back(grid[i2]);
            vertices.push_back(grid[i1]);
            vertices.push_back(grid[i3]);
        }
    }
    return vertices;
}

unsigned int Renderer::loadTexture(const char* path) {
    int w, h, n;
    unsigned char* data = stbi_load(path, &w, &h, &n, 0);

    GLenum format = (n == 4 ? GL_RGBA : GL_RGB);
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}
