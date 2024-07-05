#pragma once
#include <glad/gl.h>
#include <stdexcept>
#include <optional>
#include <iostream>
#include <vector>

namespace mygl {
    struct ScreenResolution;
    struct FrameBufferConfiguration;
    class FrameBuffer;
}

/**
 * @brief A screen resolution consisting of width and height parameters.
 * 
 */
struct mygl::ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
	ScreenResolution(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}
	ScreenResolution(): width(800), height(600) {}
};

struct mygl::FrameBufferConfiguration
{
    GLenum color_profile = GL_RGB;
    GLenum color_type = GL_RGB;
    GLenum data_type = GL_UNSIGNED_BYTE;
    bool use_depth_buffer = true;
    GLuint num_color_buffers = 1;
};


class mygl::FrameBuffer
{
public:
    FrameBuffer(FrameBufferConfiguration & config, ScreenResolution & screen_res);
    ~FrameBuffer();

    void use();
    GLuint getID();
    GLuint getColorBufferID(size_t idx);
    void setDebugName(const std::string name);
    std::string getDebugName();
private:
    GLuint ID;
    std::vector<GLuint> texture_color_buffer_IDs;
    std::optional<GLuint> RBO = std::nullopt;
    std::string debug_name;
};
