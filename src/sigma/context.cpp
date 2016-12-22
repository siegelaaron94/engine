#include <sigma/context.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <sigma/game.hpp>
#include <sigma/graphics/renderer.hpp>

#include <boost/dll/import.hpp>

namespace sigma {
context::context()
    : current_game(nullptr)
    , current_renderer(nullptr)
{
}

context::~context()
{
    current_game = nullptr;
    current_renderer = nullptr;
    game_classes.clear();
    renderer_classes.clear();
}

graphics::texture_cache& context::textures()
{
    return current_renderer->textures();
}

graphics::shader_cache& context::shaders()
{
    return current_renderer->shaders();
}

graphics::material_cache& context::materials()
{
    return current_renderer->materials();
}

graphics::static_mesh_cache& context::static_meshes()
{
    return current_renderer->static_meshes();
}

bool context::load_plugin(boost::filesystem::path path)
{
    uint32_t total = 0;
    uint32_t i = 0;
    while (true) {
        try {
            auto klass = boost::dll::import<graphics::renderer_class>(path, "renderer_class_" + std::to_string(i), boost::dll::load_mode::append_decorations);
            renderer_classes[klass->name] = klass;
            i++;
            total++;
        } catch (boost::system::system_error e) {
            break;
        }
    }
    i = 0;
    while (true) {
        try {
            auto klass = boost::dll::import<game_class>(path, "game_class_" + std::to_string(i), boost::dll::load_mode::append_decorations);
            game_classes[klass->name] = klass;
            i++;
            total++;
        } catch (boost::system::system_error e) {
            break;
        }
    }
    return total != 0;
}

void context::set_game_class(std::string game_class)
{
    current_game = game_classes[game_class]->create(this);
}

void context::set_renderer_class(std::string renderer_class)
{
    current_renderer = renderer_classes[renderer_class]->create();
}

void context::update(std::chrono::duration<float> dt)
{
    if (current_game)
        current_game->update(dt);
}

void context::render(glm::ivec2 size)
{
    if (current_renderer != nullptr && current_game != nullptr) {
        glm::mat4 m = glm::mat4(1);
        /*m = glm::translate(m,glm::vec3(0.0f,0.0f,z));
		    z = 0;*/
        //z-=.05;
        graphics::view_port vp{
            current_game->entities,
            current_game->transforms,
            current_game->static_mesh_instances,
            glm::perspective(0.785398f,(float)size.x/(float)size.y,0.01f,1000.0f),
            m
        };
        // TODO projection matrix and view matrix
		current_renderer->resize(size);
        current_renderer->render(vp);
    }
}
}