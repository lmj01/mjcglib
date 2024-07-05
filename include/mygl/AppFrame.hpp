#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <string>
#include <mygl/FrameBuffer.hpp>
#include <mygl/Scene.hpp>

namespace mygl {
    class Mouse;
    class AppFrame;
    class App;
}

class mygl::Mouse
{
private:
    glm::vec2 pos;
    glm::vec2 pos_old;
    Mouse();
    Mouse(const Mouse&);
    Mouse(double x, double y);
    Mouse & operator = (const Mouse &other) {
        if (this != &other) {
            pos.x = other.pos.x;
            pos.y = other.pos.y;
        }
        return *this;
    }
public:
    static Mouse& getInstance() {
        static Mouse instance;
        return instance;
    }

    ~Mouse();

    glm::vec2 getPosition();
    void setPosition(double x, double y);
    glm::vec2 getOffset();
};

class mygl::AppFrame
{
private:
    ScreenResolution resolution;
    GLFWwindow * window;
    App * app;
    void processInput(GLFWwindow * window);
public:
    AppFrame(ScreenResolution resolution, App * app, std::string title);
    ~AppFrame();

    bool render();
    static void framebufferSizeCallback(GLFWwindow * window, int width, int height);
    static void mouse_callback(GLFWwindow * window, double x, double y);
};

class mygl::App
{
private:
protected:
    unsigned int glVersion;
    Scene * activeScene = nullptr;
public:
    virtual ~App() {};
    virtual void configure(GLFWwindow * window) = 0;
    virtual void processUserInput(GLFWwindow * window, glm::vec2 mouse_offset) = 0;
    virtual void updateScene() = 0;
    virtual void render() = 0;
    virtual void setVersion(unsigned int version) {
        glVersion = version;
    }
    virtual void setActiveScene(Scene * scene) {
        this->activeScene = scene;
    }
    virtual void forwardUserInputToScene(GLFWwindow * window, glm::vec2 mouse_offset) {
        this->activeScene->processMouse(mouse_offset.x, mouse_offset.y);
        this->activeScene->processInput(window);
    }
};