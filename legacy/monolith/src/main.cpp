#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>

static unsigned int ss_id = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height);
float get_sun_rotate_angle_around_itself(float day);
float get_earth_rotate_angle_around_sun(float day);
float get_earth_rotate_angle_around_itself(float day);
float get_moon_rotate_angle_around_earth(float day);
float get_moon_rotate_angle_around_itself(float day);

// screen settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;

// shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec3 vertexColour;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
        "gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
        "vertexColour = aCol;\n"
    "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColour;\n"
    "in vec3 vertexColour;\n"
    "void main()\n"
    "{\n"
        "FragColour = vec4(vertexColour, 1.0f);\n"
    "}\n\0";


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // shaders
    int success;
    char error_msg[512];
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, error_msg);
        std::cout << "Vertex Shader Failed: " << error_msg << std::endl;
    }
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, error_msg);
        std::cout << "Fragment Shader Failed: " << error_msg << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, error_msg);
        std::cout << "Program Link Error: " << error_msg << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    // set vertex data
    float cube_vertices_with_colors[] = {
        // front face
        -1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, // position, color (red)
        -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 
        -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, // end of front face
        
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, // position, color (green)
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, // end of back face
        
        // left face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, // position, color (blue)
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, // end of left face
        
        // right face
         1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  1.0f, // position, color (cyan)
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f, // end of right face
        
        // top face
        -1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f, // position, color (magenta)
        -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f, // end of top face
        
        // bottom face
        -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f, // position, color (yellow)
        -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f, // end of bottom face
    };
    
    // define initial cube positions in world space
    glm::vec3 positions[]
    {
        glm::vec3(0.0f,  0.0f, 0.0f), // sun
        glm::vec3(40.0f, 0.0f, 0.0f), // earth
        glm::vec3(52.0f, 0.0f, 0.0f)  // moon
    };

    //
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_with_colors), cube_vertices_with_colors, GL_STATIC_DRAW);
    std::cout << "size of cube vertices with colour: " << sizeof(cube_vertices_with_colors) << std::endl;

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // initialize day for appropriate rotations
    float day = 0.0f, inc = 1.0f/24.0f;
    
    // initialize camera variables
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::vec3 earthPos = positions[1];
    glm::vec3 moonPos = positions[2];
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        day += inc;
        // render
        // ------
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear depth buffer

        // activate shader
        glUseProgram(shaderProgram);
        
        // camera
        view = glm::lookAt(glm::vec3(30.0f, 20.0f, 90.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 1000.0f);
        
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc  = glGetUniformLocation(shaderProgram, "projection");
        
        // pass them to the shaders
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // render cubes
        glBindVertexArray(VAO);
        for(int i = 0; i < 3; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            
            // scale and rotate cube i
            if (i == 0) // sun
            {
                model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0));
                model = glm::rotate(model, glm::radians(get_sun_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::translate(model, positions[i]);
                
            }
            else if (i == 1) // earth
            {
                model = glm::rotate(model, glm::radians(get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // revolve around sun
                model = glm::translate(model, positions[i]);
                model = glm::rotate(model, glm::radians(-get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // reverse revolve around sun
                model = glm::rotate(model, glm::radians(-23.4f), glm::vec3(0.0f, 0.0f, 1.0f)); // axis tilt
                model = glm::rotate(model, glm::radians(get_earth_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // self-revolve
                model = glm::scale(model, glm::vec3(4.0, 4.0, 4.0));
                //earthPos = glm::vec3(model[3]);
                
            }
            else // moon
            {
                model = glm::rotate(model, glm::radians(get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // revolve around sun
                model = glm::translate(model, positions[i-1]);
                model = glm::rotate(model, glm::radians(-get_earth_rotate_angle_around_sun(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // reverse revolve around sun
                model = glm::rotate(model, glm::radians(get_moon_rotate_angle_around_earth(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // revolve around earth
                model = glm::translate(model,glm::vec3(12.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(-get_moon_rotate_angle_around_earth(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // reverse revolve around earth
                model = glm::rotate(model, glm::radians(get_moon_rotate_angle_around_itself(day)), glm::vec3(0.0f, 1.0f, 0.0f)); // self-revolve
                model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
                moonPos = glm::vec3(model[3]);
            }
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input
// ---------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    //press p to capture screen
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("Assignment1-ss", buffer_width, buffer_height);
    }
}
// glfw: window re-sizing callback
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
// capture .ppm
// ---------------------------------------------------------------------------------------------
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) 
{
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte * pixels = new GLubyte [totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id ++;
    delete [] pixels;
    fout.flush();
    fout.close();
}

//* the following functions use the revolve/orbit period in days provided by Figure 3 in the assignment pdf file *//
// calculate sun rotation angle
// ---------------------------------------------------------------------------------------------
float get_sun_rotate_angle_around_itself(float day)
{
    double deg_per_day = 360.0f/27.0f;
    double angle = deg_per_day * day;
    return angle;
}
// calculate earth rotation angles
// ---------------------------------------------------------------------------------------------
float get_earth_rotate_angle_around_sun(float day)
{
    double deg_per_day = 360.0f/365.0f;
    double angle = deg_per_day * day;
    return angle;
}
float get_earth_rotate_angle_around_itself(float day)
{
    double deg_per_day = 360.0f;
    double angle = deg_per_day * day;
    return angle;
}
// calculate moon rotation angles
// ---------------------------------------------------------------------------------------------
float get_moon_rotate_angle_around_earth(float day)
{
    double deg_per_day = 360.0f/28.0f;
    double angle = deg_per_day * day;
    return angle;
}
float get_moon_rotate_angle_around_itself(float day)
{
    double deg_per_day = 360.0f/28.0f;
    double angle = deg_per_day * day;
    return angle;
}
