#include "Scene.h"

#include <vector>
#include <TypedBuffer.h>
#include <iostream>
#include <queue>
#include <shader_structs.h>

#define NB_OBJECTS 28

namespace OM3D
{

    namespace
    {
        auto max3(const glm::vec3& a) -> float
        {
            return std::max(std::max(a.x, a.y), a.z);
        } 
    } // namespace
    

    Scene::Scene()
    {}

    void Scene::add_object(SceneObject obj)
    {
        auto ptr = obj.material().get();

        auto iter = _objects.find(ptr);
        if (iter == _objects.end())
        {
            std::vector<SceneObject> vec{ std::move(obj) };
            _objects.emplace(std::make_pair(ptr, vec));
        }
        else
            iter->second.emplace_back(std::move(obj));
    }

    void Scene::add_light(PointLight obj)
    {
        _point_lights.emplace_back(std::move(obj));
    }

    std::size_t Scene::object_count() const
    {
        return _objects.size();
    }

    Span<const PointLight> Scene::point_lights() const
    {
        return _point_lights;
    }

    Camera& Scene::camera()
    {
        return _camera;
    }

    const Camera& Scene::camera() const
    {
        return _camera;
    }

    void Scene::set_sun(glm::vec3 direction, glm::vec3 color)
    {
        _sun_direction = direction;
        _sun_color = color;
    }

    void Scene::render() const
    {
        // Fill and bind frame data buffer
        TypedBuffer<shader::FrameData> buffer(nullptr, 1);
        {
            auto mapping = buffer.map(AccessType::WriteOnly);
            mapping[0].camera.view_proj = _camera.view_proj_matrix();
            mapping[0].point_light_count = u32(_point_lights.size());
            mapping[0].sun_color = _sun_color;
            mapping[0].sun_dir = glm::normalize(_sun_direction);
        }
        buffer.bind(BufferUsage::Uniform, 0);

        // Fill and bind lights buffer
        TypedBuffer<shader::PointLight> light_buffer(
            nullptr, std::max(_point_lights.size(), size_t(1)));
        {
            auto mapping = light_buffer.map(AccessType::WriteOnly);
            for (size_t i = 0; i != _point_lights.size(); ++i)
            {
                const auto& light = _point_lights[i];
                mapping[i] = { light.position(), light.radius(), light.color(),
                               0.0f };
            }
        }
        light_buffer.bind(BufferUsage::Storage, 1);

        auto frustum = _camera.build_frustum();
        glm::vec3 normals[5] = {
            frustum._top_normal,   frustum._bottom_normal, frustum._left_normal,
            frustum._right_normal, frustum._near_normal,
        };
        auto near_dist = _camera.position();

        // FIXME: Find a way to avoid copying the objects
        std::vector<glm::mat4> to_display;

        for (const auto& [mat, vec] : _objects)
        {

            for (const auto& obj : vec)
            {
                float radius = obj.get_sphere_radius();
                bool culled = false;
                for (const glm::vec3& normal : normals)
                {
                    glm::vec3 center = obj.transform()
                        * glm::vec4(obj.get_sphere_position(), 1.0f);
                    auto scale = obj.get_transform_components().scale;
                    if (glm::dot(normal, center - near_dist) < -(radius * max3(scale)))
                    {
                        culled = true;
                    }
                }

                if (!culled)
                {
                    to_display.push_back(obj.transform());
                    // obj.render();
                }
            }

            if (to_display.size()) {
                TypedBuffer<glm::mat4> transforms{to_display.data(), to_display.size()};
                mat->bind();
                vec[0].render_batch(transforms);
                to_display.clear();
            }
        }
    }

} // namespace OM3D
