#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <string>

namespace mygl {
    struct ScreenResolution;
    class Mouse;
    class AppFrame;
    class App;
}

struct mygl::ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
	ScreenResolution(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}
	ScreenResolution(): width(800), height(600) {}
};

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
public:
    virtual ~App() {};
    virtual void configure(GLFWwindow * window) = 0;
    virtual void processUserInput(GLFWwindow * window, glm::vec2 mouse_offset) = 0;
    virtual void updateScene() = 0;
    virtual void render() = 0;
    virtual void setVersion(unsigned int version) {
        glVersion = version;
    }
};