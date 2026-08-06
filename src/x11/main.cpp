#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "stb_image.h"

#include "../gl.h"
#include "../state.h"
#include "../config/ShanghaiConfiguration.h"
#include "../Background.h"
#include "../Shanghai.h"
#include "../fonts/FontRenderer.h"
#include "../Random.h"

// Windows stuff
GLFWwindow* glfwWindow;

// Actual Shanghai stuff
EGLState eglState;
ShanghaiConfiguration* config;
Background* background;
FontRenderer* fontRenderer;
std::vector<Shanghai*> shanghais;

// Callback handlers

void resizeHandler([[maybe_unused]] GLFWwindow* window, const int width, const int height) {
    eglState.width = width;
    eglState.height = height;

    for (auto& shanghai : shanghais) {
        shanghai->setScreenGeometry(eglState.width, eglState.height);
    }

    if (background != nullptr) {
        background->setScreenGeometry(eglState.width, eglState.height);
    }

    glViewport(0, 0, width, height);
}

void mouseHandler([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    eglState.curX = (int) xpos;
    eglState.curY = (int) ypos;
}

void mouseButtonHandler(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    double xx, yy;
    glfwGetCursorPos(window, &xx, &yy);

    if (action == GLFW_PRESS) {
        eglState.buttons = 1;
    } else {
        eglState.buttons = 0;
    }
}

// Draw loop
void draw() {
    glViewport(0, 0, (int) eglState.width, (int) eglState.height);

    glClearColor(0, 0, 0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (eglState.curX != -1 && eglState.curY != -1) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(eglState.curX, (int) (eglState.height - eglState.curY), 5, 5);
        glClearColor(0, 0, 0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
    }

    if (config->isBackgroundEnabled()) {
        background->draw(&eglState);
    }

    for (const auto& shanghai : shanghais) {
        shanghai->draw(&eglState);
    }

    Shanghai::updateCursor(shanghais, &eglState);

    glfwSwapBuffers(glfwWindow);
}

int main() {
    std::cout << "Loading configuration...\n";
    config = ShanghaiConfiguration::getInstance();

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW error " << error << ": " << description << '\n';
    });

    glfwInit();

    int monitorCount;
    auto* monitors = glfwGetMonitors(&monitorCount);
    // Default to the primary monitor - due to Linux compat, UINT32_MAX is used to indicate no monitor preference
    uint32_t monitorIndex = config->getOutput() == UINT32_MAX ? 0 : config->getOutput();

    if (monitorCount == 0) {
        std::cerr << "No outputs found\n";
        return -1;
    }

    if (config->getOutput() >= monitorCount && config->getOutput() != UINT32_MAX) {
        std::cerr << "Invalid output index\n";
        return -1;
    }

    auto* monitor = monitors[monitorIndex];

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto* videoMode = glfwGetVideoMode(monitor);
    glfwWindow = glfwCreateWindow(videoMode->width, videoMode->height, "Shanghai", monitor, nullptr);

    if (glfwWindow == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(glfwWindow);
    gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    if (!gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(glfwWindow, resizeHandler);
    glfwSetCursorPosCallback(glfwWindow, mouseHandler);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonHandler);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "OpenGL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (config->isBackgroundEnabled()) {
        background = new Background();
    }

    auto shanghai = new Shanghai();
    fontRenderer = new FontRenderer("TamzenForPowerline10x20r.bdf");
    // shanghai->positionX = -40;
    // shanghai->positionY = 1300;
    // shanghai->flip = false;
    // shanghai->getStateMachine()->setState(ShanghaiState::WALL_CLIMB);
    shanghai->getStateMachine()->setState(ShanghaiState::THROWING);
    shanghais.push_back(shanghai);
    // ShanghaiState states[] = {ShanghaiState::CRAWLING, ShanghaiState::SITTING_AND_LOOKING, ShanghaiState::SITTING, ShanghaiState::WALKING, ShanghaiState::JUMP};
    // // ShanghaiState states[] = {ShanghaiState::WALL_HOLD, ShanghaiState::WALL_CLIMB};
    // for (int i = 0; i < 40; ++i) {
    //     auto* shanghai = new Shanghai();
    //     shanghai->positionX = i * 128;
    //     shanghais.push_back(shanghai);
    //
    //     shanghai->getStateMachine()->setState(states[(int) (Random::rand() * std::size(states))]);
    //     shanghai->flip = Random::rand() < 0.5;
    // }

    std::cout << "Starting output...\n";

    resizeHandler(glfwWindow, videoMode->width, videoMode->height);
    XSetWindowBorderWidth(glfwGetX11Display(), glfwGetX11Window(glfwWindow), 0);
    XMoveResizeWindow(glfwGetX11Display(), glfwGetX11Window(glfwWindow), 0, 0, videoMode->width, videoMode->height);

    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwPollEvents();
        draw();
    }

    glfwTerminate();

    return 0;
}