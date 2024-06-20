#include <stdexcept>
#include <mygl/AppFrame.hpp>
#include <iostream>

using namespace mygl;

Mouse::Mouse():pos(glm::vec2(0, 0)), pos_old(pos) {}

Mouse::Mouse(double x, double y) {
    this->pos = glm::vec2(x, y);
    this->pos_old = this->pos;
}

Mouse::~Mouse() {
    // delete &pos_old;
    // delete &pos;
}

glm::vec2 Mouse::getPosition() {
    return glm::vec2(this->pos);
}

void Mouse::setPosition(double x, double y) {
    this->pos_old = this->pos;
    this->pos = glm::vec2(x, y);
}

glm::vec2 Mouse::getOffset() {
    return glm::vec2(this->pos  - this->pos_old);
}

void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

AppFrame::AppFrame(ScreenResolution resolution, App * app, std::string title) {
    if (GL_TRUE != glfwInit()) {
        std::cerr << "Error initialising glfw" << std::endl;    
    }
    glfwSetErrorCallback( error_callback );
    
    this->resolution = resolution;
    this->window = glfwCreateWindow(resolution.width, resolution.height, title.c_str(), NULL, NULL);
    if (!this->window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }
    glfwMakeContextCurrent(this->window);

    glfwSetFramebufferSizeCallback(this->window, &AppFrame::framebufferSizeCallback);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int version = gladLoadGL(glfwGetProcAddress);
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;
    unsigned int glVersion = 100 * GLAD_VERSION_MAJOR(version) + 10 * GLAD_VERSION_MINOR(version);

    app->setVersion(glVersion);
    this->app = app;
}

AppFrame::~AppFrame() {
    
}

void AppFrame::framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void AppFrame::processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void AppFrame::mouse_callback(GLFWwindow * window, double x, double y) {
    Mouse::getInstance().setPosition(x, y);
}

bool AppFrame::render() {
    this->app->configure(this->window);
    while(!glfwWindowShouldClose(this->window)) {
        glfwSetCursorPosCallback(this->window, mouse_callback);
        this->app->processUserInput(this->window, Mouse::getInstance().getOffset());
        this->app->updateScene();
        this->app->render();
        glfwSwapBuffers(this->window);
    }
    glfwTerminate();
    return 0;
}