#include <sigma/graphics/opengl/shader.hpp>

#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/shader.hpp>

#include <glad/glad.h>

#include <cstdlib>
#include <iostream>
#include <vector>

namespace sigma {
namespace opengl {
    shader_type convert(graphics::shader_type type)
    {
        switch (type) {
        case graphics::shader_type::vertex:
            return shader_type::VERTEX_SHADER;
        case graphics::shader_type::fragment:
            return shader_type::FRAGMENT_SHADER;
        case graphics::shader_type::geometry:
            return shader_type::GEOMETRY_SHADER;
        case graphics::shader_type::tessellation_control:
            return shader_type::TESS_CONTROL_SHADER;
        case graphics::shader_type::tessellation_evaluation:
            return shader_type::TESS_EVALUATION_SHADER;
        default:
            return shader_type::NONE_SHADER;
        }
    }

    shader::shader(shader_type type, std::string source)
    {
        const char* src = source.c_str();

        GL_CHECK(object_ = glCreateShader(GLenum(type)));
        GL_CHECK(glShaderSource(object_, 1, &src, nullptr));
        GL_CHECK(glCompileShader(object_));

        GLint compiled;
        glGetShaderiv(object_, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(object_, GL_INFO_LOG_LENGTH, &length);

            // The length includes the NULL character
            std::vector<GLchar> error_buffer(length);
            glGetShaderInfoLog(object_, length, &length, error_buffer.data());

            std::cout << src << '\n';
            std::string error_string(error_buffer.begin(), error_buffer.end());
            std::cout << error_string << '\n';

            std::abort(); // TODO be more subtle???
        }
    }

    shader::shader(graphics::shader data)
        : shader(convert(data.type), data.source)
    {
    }

    shader::~shader()
    {
        glDeleteShader(object_);
    }

    GLuint shader::get_object() const
    {
        return object_;
    }
}
}
