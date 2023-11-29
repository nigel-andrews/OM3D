#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

namespace OM3D
{

    SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh,
                             std::shared_ptr<Material> material)
        : _mesh(std::move(mesh))
        , _material(std::move(material))
    {}

    void SceneObject::render() const
    {
        if (!_material || !_mesh)
        {
            return;
        }
        _mesh->draw();
    }

    // FIXME: This should not be a member function, but a free function or a static member function.
    void SceneObject::render_batch(const TypedBuffer<glm::mat4>& transforms) const
    {
        if (!_material || !_mesh)
        {
            return;
        }

        _mesh->draw_batch(transforms);
    }

    void SceneObject::set_transform(const glm::mat4& tr)
    {
        _transform = tr;
    }

    const glm::mat4& SceneObject::transform() const
    {
        return _transform;
    }

} // namespace OM3D
