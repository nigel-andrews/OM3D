#ifndef SCENE_H
#define SCENE_H

#include <Camera.h>
#include <PointLight.h>
#include <SceneObject.h>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "Material.h"

namespace OM3D
{

    class Scene : NonMovable
    {
    public:
        using ObjectStorage =
            std::map<const Material*,
                     std::map<const StaticMesh*, std::vector<SceneObject>>>;

        Scene();

        static Result<std::unique_ptr<Scene>>
        from_gltf(const std::string& file_name);

        void render() const;

        void add_object(SceneObject obj);
        void add_light(PointLight obj);

        const ObjectStorage& objects() const
        {
            return _objects;
        }

        std::size_t object_count() const;
        Span<const PointLight> point_lights() const;

        Camera& camera();
        const Camera& camera() const;

        void set_sun(glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f));

        friend std::ostream& operator<<(std::ostream& os, const Scene& s);

    private:
        // std::vector<SceneObject> _objects;

        ObjectStorage _objects;
        std::vector<PointLight> _point_lights;

        glm::vec3 _sun_direction = glm::vec3(0.2f, 1.0f, 0.1f);
        glm::vec3 _sun_color = glm::vec3(1.0f);

        Camera _camera;
    };

} // namespace OM3D

#endif // SCENE_H
