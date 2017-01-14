#include "simple_game.hpp"

#include <glm/vec3.hpp>

#include <boost/math/constants/constants.hpp>

#include <chrono>

simple_game::simple_game(sigma::graphics::renderer* renderer)
    : sigma::game(renderer)
    , generator_(6546534)
    , position_distribution_{ { -20, 0, -20 }, { 20.0f, .5, 20.0f } }
    , rotation_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 2.0f * boost::math::constants::pi<float>() } }
    , scale_distribution_{ 0.1f, 10.0f }
    , color_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 1.0f } }
{
    load("../data/material_test_scene.scn");

    int number = 0;
    int MAX = 8;
    for (int x = 0; x < MAX; ++x) {
        for (int z = 0; z < MAX; ++z) {
            number++;
            auto material_ball = renderer->static_meshes().duplicate(sigma::resource::identifier{ "static_mesh://material_ball:material_ball" }, sigma::resource::identifier{ "static_mesh://material_ball:material_ball" + std::to_string(number) });
            auto red_plastic = renderer->materials().duplicate(sigma::resource::identifier{ "material://white_plastic" }, sigma::resource::identifier{ "material://red_plastic" + std::to_string(number) });
            material_ball->set_material(0, red_plastic);
            red_plastic->set_uniform("basecolor", glm::vec3{ 1, 0, 0 });
            red_plastic->set_uniform("roughness", x / float(MAX));
            red_plastic->set_uniform("metalness", 1.0f - (z / float(MAX)));

            auto e = entities.create();
            auto& txform = transforms.add(e);
            txform.set_position(glm::vec3{ 1.5f * x, 0, 1.5f * z });
            static_mesh_instances.add(e, material_ball);
        }
    }

    // load("../data/water_packed.scn");
    //
    // std::uniform_int_distribution<int> point_light_count_distribution_{ 0, 500 };
    // int number_of_point_lights = point_light_count_distribution_(generator_);
    // for (int i = 0; i < number_of_point_lights; ++i) {
    //     auto e = entities.create();
    //     auto& txform = random_transform(e);
    //     auto& light = point_lights.add(e);
    //     light.color = color_distribution_(generator_);
    //     light.intensity = scale_distribution_(generator_);
    // }
    //
    // std::vector<sigma::resource::identifier> static_mesh_ids = {
    //     "static_mesh://water_packed:Torus",
    //     "static_mesh://water_packed:cube",
    //     "static_mesh://water_packed:piller.000",
    //     "static_mesh://water_packed:piller.001",
    //     "static_mesh://water_packed:piller.002",
    //     "static_mesh://water_packed:piller.003",
    //     "static_mesh://water_packed:piller.004",
    //     "static_mesh://water_packed:shape",
    //     "static_mesh://water_packed:sphere",
    //     "static_mesh://water_packed:suzan",
    // };
    //
    // std::uniform_int_distribution<int> static_mesh_count_distribution_{ 0, 1500 };
    // int number_of_static_meshes = static_mesh_count_distribution_(generator_);
    // std::uniform_int_distribution<std::size_t> static_mesh_distribution_{ 0, static_mesh_ids.size() - 1 };
    // for (int i = 0; i < number_of_static_meshes; ++i) {
    //     auto e = entities.create();
    //     auto& txform = random_transform(e);
    //     auto random_mesh_id = static_mesh_ids[static_mesh_distribution_(generator_)];
    //     static_mesh_instances.add(e, renderer_->static_meshes().get(random_mesh_id));
    // }
    //
    // std::cout << "Rendering " << number_of_point_lights << " point lights." << std::endl;
    // std::cout << "Rendering " << number_of_static_meshes << " static meshes." << std::endl;
}

sigma::transform& simple_game::random_transform(sigma::entity e)
{
    auto& txform = transforms.add(e);
    txform.set_position(position_distribution_(generator_));
    txform.set_rotation(glm::quat{ rotation_distribution_(generator_) });
    txform.set_scale(glm::vec3{ scale_distribution_(generator_) });
    return txform;
}

void simple_game::update(std::chrono::duration<float> dt)
{
    std::cout << " this is a test " << std::endl;
}

SIGMA_EXPORT_GAME_CLASSES(simple_game)
