#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Material.h>
#include <StaticMesh.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/matrix.hpp>
#include <memory>

namespace OM3D
{
    struct TransformComponents
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 skew;
        glm::vec4 perspective;
    };

    class SceneObject
    {
    public:
        SceneObject(std::shared_ptr<StaticMesh> mesh = nullptr,
                    std::shared_ptr<Material> material = nullptr);

        void render() const;
        void render_batch(const TypedBuffer<glm::mat4>& transforms) const;

        void set_transform(const glm::mat4& tr);
        const glm::mat4& transform() const;

        const std::shared_ptr<Material> material() const
        {
            return _material;
        }

        const std::shared_ptr<StaticMesh> mesh() const
        {
            return _mesh;
        }

        const glm::vec3 get_sphere_position() const
        {
            return _mesh->get_bounding_sphere()._center;
        }

        float get_sphere_radius() const
        {
            return _mesh->get_bounding_sphere()._radius;
        }

        const TransformComponents get_transform_components() const
        {
            TransformComponents components;
            glm::decompose(_transform, components.scale, components.rotation,
                           components.position, components.skew,
                           components.perspective);
            return components;
        }

    private:
        glm::mat4 _transform = glm::mat4(1.0f);

        std::shared_ptr<StaticMesh> _mesh;
        std::shared_ptr<Material> _material;
    };

} // namespace OM3D

#endif // SCENEOBJECT_H
