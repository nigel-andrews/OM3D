#include "StaticMesh.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <limits>
#include <numeric>

namespace OM3D
{

    BoundingSphere::BoundingSphere(const glm::vec3& center, float radius)
        : _center(center)
        , _radius(radius)
    {}

    class Range
    {
    public:
        Range()
        {
            min = std::numeric_limits<float>::max();
            max = std::numeric_limits<float>::min();
        }

        void stretch_to(float value)
        {
            if (value < min)
                min = value;
            else if (value > max)
                max = value;
        }

        float min;
        float max;
    };

    class Bounds
    {
    public:
        Bounds(const MeshData& data)
        {
            for (const auto& v : data.vertices)
                this->stretch_to(v.position);
        }

        void stretch_to(const glm::vec3& pos)
        {
            x.stretch_to(pos.x);
            y.stretch_to(pos.y);
            z.stretch_to(pos.z);
        }

        glm::vec3 get_center() const
        {
            return glm::vec3((x.min + x.max) / 2., (y.min + y.max) / 2.,
                             (z.min + z.max) / 2.);
        }

        glm::vec3 get_min() const
        {
            return glm::vec3(x.min, y.min, z.min);
        }

        glm::vec3 get_max() const
        {
            return glm::vec3(x.max, y.max, z.max);
        }

        Range x;
        Range y;
        Range z;
    };

    extern bool audit_bindings_before_draw;

    StaticMesh::StaticMesh(const MeshData& data)
        : _vertex_buffer(data.vertices)
        , _index_buffer(data.indices)
    {
        Bounds bounds(data);

        glm::vec3 center = bounds.get_center();
        float radius = std::max(glm::distance(bounds.get_min(), center),
                                glm::distance(bounds.get_max(), center));

        this->_bounding_sphere = BoundingSphere(center, radius);
    }

    void StaticMesh::draw() const
    {
        _vertex_buffer.bind(BufferUsage::Attribute);
        _index_buffer.bind(BufferUsage::Index);

        // Vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
        // Vertex normal
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        // Vertex uv
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(6 * sizeof(float)));
        // Tangent / bitangent sign
        glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(8 * sizeof(float)));
        // Vertex color
        glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(12 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        if (audit_bindings_before_draw)
        {
            audit_bindings();
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glDrawElements(GL_TRIANGLES, int(_index_buffer.element_count()),
                       GL_UNSIGNED_INT, nullptr);

        glDisable(GL_CULL_FACE);
    }

    void StaticMesh::draw_batch(const TypedBuffer<glm::mat4>& transforms) const
    {
        _vertex_buffer.bind(BufferUsage::Attribute);
        _index_buffer.bind(BufferUsage::Index);
        transforms.bind(BufferUsage::Storage, 5);

        // Vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
        // Vertex normal
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        // Vertex uv
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(6 * sizeof(float)));
        // Tangent / bitangent sign
        glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(8 * sizeof(float)));
        // Vertex color
        glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex),
                              reinterpret_cast<void*>(12 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        if (audit_bindings_before_draw)
        {
            audit_bindings();
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glDrawElementsInstanced(
            GL_TRIANGLES, int(_index_buffer.element_count()), GL_UNSIGNED_INT,
            nullptr, transforms.element_count());

        glDisable(GL_CULL_FACE);
    }

} // namespace OM3D
