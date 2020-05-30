#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>

#include "shader.h"
#include "ShaderProgram.h"
#include "fpsCounter.h"
#include "screenBounds.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>


// Initial resolution of window
unsigned int winWidth = 512*2;
unsigned int winHeight = 512*2;

// I need these global variables because I cannot provide my own parameters to the GLFW callback functions.
namespace settings {
    bool g_interiorBlack = true;
    bool g_logScale = false;
    bool g_useSuperSampling = true;
    bool g_lockCursor = false;
    bool g_unlockAspectRatio = true;
    bool g_genMandle = false;
    glm::dvec2 g_seed = glm::dvec2(0., 0.);
    ScreenBounds g_screenBounds(glm::dvec2(0., 0.), glm::dvec2(4., 4.)); // Sets default zoom to be [-2, 2], [-2, 2] 
}

namespace input {
    glm::dvec2 g_cursorPos; // Center of screen is (0, 0), borders are +- 1;
}

namespace screenState {
    // Resolution of render
    unsigned int hRes = 1920 * 2;
    unsigned int vRes = 1027 * 2; // Supports non-powers of two, but powers of two will be somewhat faster
    unsigned int renderTexture;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    winWidth = width;
    winHeight = height;

    int superSamplingFactor = settings::g_useSuperSampling ? 2 : 1;

    screenState::hRes = width * superSamplingFactor;
    screenState::vRes = height * superSamplingFactor;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenState::renderTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenState::hRes, screenState::vRes, 0, GL_RGBA, GL_FLOAT, NULL);

    glViewport(0, 0, width, height);
}


// XMousepos and yMousepos given in normalized coords, [-1, 1)
// Positive X axis is to the right, positive Y axis is upwards on the screen
void processInput(GLFWwindow* window)
{
    glm::dvec2 newMousePos;
    glfwGetCursorPos(window, &(newMousePos.x), &(newMousePos.y));
    


    newMousePos.x *= 2. / winWidth;
    newMousePos.x -= 1.;
    newMousePos.y *= -2. / winHeight;
    newMousePos.y += 1.;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glm::dvec2 deltaMouse = newMousePos - input::g_cursorPos;
        settings::g_screenBounds.translate(settings::g_screenBounds.screenVecToWorld(-1. * deltaMouse));
    }
    
    input::g_cursorPos = newMousePos;


    if (!settings::g_lockCursor) {
        settings::g_seed = settings::g_screenBounds.screenPointToWorld(input::g_cursorPos);
    }
}

void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        settings::g_genMandle = !settings::g_genMandle;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        settings::g_useSuperSampling = !settings::g_useSuperSampling;
        framebuffer_size_callback(window, winWidth, winHeight); // Force resizing of window for supersampling settings;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        settings::g_lockCursor = !settings::g_lockCursor;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        settings::g_unlockAspectRatio = !settings::g_unlockAspectRatio;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        settings::g_screenBounds.setCenter(0., 0.);
        if (settings::g_unlockAspectRatio) {
            settings::g_screenBounds.setSize(4. * winWidth / winHeight, 4.);
        } else {
            settings::g_screenBounds.setSize(4., 4.);
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        settings::g_logScale = !settings::g_logScale;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    settings::g_screenBounds.zoom(settings::g_screenBounds.screenPointToWorld(input::g_cursorPos), pow(0.8, -1. * yoffset));
}

int main(void) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Realtime Julia Renderer", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, winWidth, winHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyPressCallback);
    glfwSetScrollCallback(window, scrollCallback);
    //
    float vertices[] = {
         // positions     // Tex coords
         1.00f,  1.00f, 0.0f,  1.f,  1.f, // top right
         1.00f, -1.f, 0.0f,  1.f,  0.f,  // bottom right
        -1.f, -1.f, 0.0f,  0.f,  0.f,  // bottom left
        -1.f,  1.00f, 0.0f,  0.f,  1.f,   // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    glGenTextures(1, &screenState::renderTexture);

   
    glBindTexture(GL_TEXTURE_2D, screenState::renderTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenState::renderTexture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screenState::hRes, screenState::vRes, 0, GL_RGBA, GL_FLOAT, NULL);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, hRes, vRes, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, screenState::renderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


    // Vertex and fragment shaders are pretty much empty, and just pass through vertex/texture coord data
    Shader vertex("doNothing.vert", GL_VERTEX_SHADER);
    Shader fragment("doNothing.frag", GL_FRAGMENT_SHADER);
    ShaderProgram renderProg;
    renderProg.attach(vertex);
    renderProg.attach(fragment);
    renderProg.link();

    Shader juliaShader("julia.comp", GL_COMPUTE_SHADER);
    ShaderProgram juliaProg;
    juliaProg.attach(juliaShader);
    juliaProg.link();
    
    Shader mandleShader("mandle.comp", GL_COMPUTE_SHADER);
    ShaderProgram mandleProg;
    mandleProg.attach(mandleShader);
    mandleProg.link();


    std::cout << "\n\nJulia Renderer 1.1\n";
    std::cout << "Click right button to switch between log and linear scale\n";
    std::cout << "(Changing to log scale will also greatly increase the iteration count)\n";
    std::cout << "Press 'S' to enable / disable 4x supersampling (Higher quality, but more expensive)\n";
    std::cout << "Press 'P' to lock the cursor position and prevent the set from changing\n";
    std::cout << "Drag with left mouse to pan around\n";
    std::cout << "\nUse the scroll wheel to zoom in/out at mouse location\n";
    std::cout << "Press 'R' to reset the camera view"<< std::endl;
    std::cout << "Press 'A' to switch between forced 1:1 aspect ratio, and unlocked aspect ratio\n";
    std::cout << "To apply a change in aspect ratio, reset the window with 'R'\n\n";

    std::cout << "Press 'M' to switch between mandlebrot and julia sets" << std::endl;



    input::g_cursorPos;

    FpsCounter fpsCounter;
    framebuffer_size_callback(window, winWidth, winHeight); // Force resizing of window for supersampling settings;
    while (!glfwWindowShouldClose(window))
    {
        fpsCounter.update(glfwGetTime());
        std::stringstream title;
        title << "Realtime Julia Renderer - FPS: " <<  std::setprecision(0) << std::setiosflags(std::ios::fixed) << fpsCounter.getFPS();
        glfwSetWindowTitle(window, title.str().c_str());

        // Input (updates global variable for mouse position)
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Sets our colour settings, and current mouse position for the compute shader
        if (settings::g_genMandle) {
            mandleProg.use();
            //mandleProg.setVec2("seed", settings::g_seed);
            mandleProg.setUvec2("resolution", screenState::hRes, screenState::vRes);
            mandleProg.setBool("blackoutInterior", settings::g_interiorBlack);
            mandleProg.setBool("useLogScale", settings::g_logScale);
            mandleProg.setVec2("llWindowPos", settings::g_screenBounds.getLLcorner());
            mandleProg.setVec2("windowSize", settings::g_screenBounds.getSize());
        } else {
            juliaProg.use();
            juliaProg.setVec2("seed", settings::g_seed);
            juliaProg.setUvec2("resolution", screenState::hRes, screenState::vRes);
            juliaProg.setBool("blackoutInterior", settings::g_interiorBlack);
            juliaProg.setBool("useLogScale", settings::g_logScale);
            juliaProg.setVec2("llWindowPos", settings::g_screenBounds.getLLcorner());
            juliaProg.setVec2("windowSize", settings::g_screenBounds.getSize());
        }


        glDispatchCompute((screenState::hRes + 15) / 16, (screenState::vRes + 15) / 16, 1); // For local work group size 16. Ensures entire texture is written to

        glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
        
        // Render screen-sized quad
        renderProg.use();
        glBindTexture(GL_TEXTURE_2D, screenState::renderTexture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // End drawing current frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}
