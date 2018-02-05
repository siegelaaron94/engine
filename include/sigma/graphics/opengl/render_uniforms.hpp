#ifndef SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP
#define SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/matrix.hpp>

namespace sigma {
namespace opengl {
    struct instance_matrices {
        BOOST_HANA_DEFINE_STRUCT(
            instance_matrices,
            (glm::mat4, model_matrix),
            (glm::mat4, model_view_matrix),
            (glm::mat3, normal_matrix));
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP
