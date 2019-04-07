#ifndef SIGMA_TOOLS_TECHNIQUE_LOADER_HPP
#define SIGMA_TOOLS_TECHNIQUE_LOADER_HPP

#include <sigma/tools/json_conversion.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/reflect.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/util/filesystem.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {
    void extract_unifrom_data(std::shared_ptr<context> ctx, const Json::Value& settings, graphics::technique_uniform_data& data)
    {
        auto texture_cache = ctx->cache<graphics::texture>();
        auto cubemap_cache = ctx->cache<graphics::cubemap>();

        for (auto it = settings.begin(); it != settings.end(); ++it) {
            const auto& value = *it;
            if (it.key() == "vertex" || it.key() == "tessellation_control" || it.key() == "tessellation_evaluation" || it.key() == "geometry" || it.key() == "fragment") {
                continue;
            } else if (it.key() == "textures") {
                const auto& texture_object = *it;
                for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2) {
                    data.textures[it2.key().asString()] = texture_cache->handle_for({ std::filesystem::path{ "texture" } / (*it2).asString() }); // TODO warn if tring to set texture more than once
                }
            } else if (it.key() == "cubemaps") {
                const auto& cubemap_object = *it;
                for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2) {
                    data.cubemaps[it2.key().asString()] = cubemap_cache->handle_for({ std::filesystem::path{ "cubemap" } / (*it2).asString() }); // TODO warn if tring to set cubemap more than once
                }
            } else {
                float f;
                glm::vec2 v2;
                glm::vec3 v3;
                glm::vec4 v4;
                if (json::from_json(ctx, value, v4))
                    data.vec4s[it.key().asString()] = v4;
                else if (json::from_json(ctx, value, v3))
                    data.vec3s[it.key().asString()] = v3;
                else if (json::from_json(ctx, value, v2))
                    data.vec2s[it.key().asString()] = v2;
                else if (json::from_json(ctx, value, f))
                    data.floats[it.key().asString()] = f;
            }
        }
    }

    class technique_loader : public resource_loader {
    public:
        technique_loader(std::shared_ptr<context> ctx)
            : resource_loader(ctx)
            , context_(ctx)
        {
        }

        virtual ~technique_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".stech",
                ".smat",
                ".eff"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) override
        {
            auto shader_cache = context_->cache<graphics::shader>();
            auto technique_cache = context_->cache<graphics::technique>();

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            graphics::technique_identifier tech_id;
            json::from_json(context_, settings, tech_id);

            // TODO add any other other shaders here.
            resource::resource_id rid{
                tech_id.vertex,
                tech_id.tessellation_control,
                tech_id.tessellation_evaluation,
                tech_id.geometry,
                tech_id.fragment
            };

            if (technique_cache->contains(rid)) {
                auto h = technique_cache->handle_for(rid);

                auto source_file_time = std::filesystem::last_write_time(source_file);
                auto resource_time = technique_cache->last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: technique { " << rid[0] << ", ";
            for (std::size_t i = 1; i < rid.size() - 1; ++i) {
                if (!rid[i].empty())
                    std::cout << rid[i] << ", ";
            }
            std::cout << rid[rid.size() - 1] << "}\n";

            graphics::technique technique;

            technique.vertex = shader_cache->handle_for({ tech_id.vertex });
            if (!tech_id.tessellation_control.empty())
                technique.tessellation_control = shader_cache->handle_for({ tech_id.tessellation_control });
            if (!tech_id.tessellation_evaluation.empty())
                technique.tessellation_evaluation = shader_cache->handle_for({ tech_id.tessellation_evaluation });
            if (!tech_id.geometry.empty())
                technique.geometry = shader_cache->handle_for({ tech_id.geometry });
            technique.fragment = shader_cache->handle_for({ tech_id.fragment });

            // TODO: (NOW) check for link errors.
            // TODO: (NOW) extract reflection data.

            technique_cache->insert(rid, technique, true);
        }

    private:
        std::shared_ptr<context> context_;
    };
}
}

#endif // SIGMA_TOOLS_TECHNIQUE_MODEL_HPP
