#ifndef STATICMESH_H
#define STATICMESH_H

#include <graphics.h>
#include <TypedBuffer.h>
#include <Vertex.h>

#include <vector>

namespace OM3D {

struct BoundingSphere {
    glm::vec3 _center;
    float _radius;
    
    BoundingSphere() = default;
    BoundingSphere(const glm::vec3 &center, float radius);
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};

class StaticMesh : NonCopyable {

    public:
        StaticMesh() = default;
        StaticMesh(StaticMesh&&) = default;
        StaticMesh& operator=(StaticMesh&&) = default;

        StaticMesh(const MeshData& data);

        void draw() const;

    private:
        TypedBuffer<Vertex> _vertex_buffer;
        TypedBuffer<u32> _index_buffer;
        BoundingSphere _bounding_sphere;
};

}

#endif // STATICMESH_H
