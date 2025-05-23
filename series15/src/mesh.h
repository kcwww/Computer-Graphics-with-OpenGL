#ifndef __MESH_H__
#define __MESH_H__

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "program.h"

struct Vertex
{
    glm::vec3 position; // position 은 3D 좌표
    glm::vec3 normal;   // normal 은 3D normal vector
    glm::vec2 texCoord; // texCoord 은 2D texture coordinate
    glm::vec3 tangent;  // tangent 은 3D tangent vector
};

CLASS_PTR(Material);
class Material
{
public:
    static MaterialUPtr Create()
    {
        return MaterialUPtr(new Material());
    }
    TexturePtr diffuse;
    TexturePtr specular;
    float shininess{32.0f};

    void SetToProgram(const Program *program) const;

private:
    Material() {}
};

CLASS_PTR(Mesh);
class Mesh
{
public:
    static MeshUPtr Create(
        const std::vector<Vertex> &vertices,
        const std::vector<uint32_t> &indices,
        uint32_t primitiveType);
    static MeshUPtr CreateBox();

    // plane mesh 생성
    static MeshUPtr CreatePlane();

    // VAO, VBO, IBO 생성 후 바인딩
    const VertexLayout *GetVertexLayout() const { return m_vertexLayout.get(); }
    BufferPtr GetVertexBuffer() const { return m_vertexBuffer; }
    BufferPtr GetIndexBuffer() const { return m_indexBuffer; }

    // mesh 의 material 설정
    void SetMaterial(MaterialPtr material) { m_material = material; }
    MaterialPtr GetMaterial() const { return m_material; }
    void Draw(const Program *program) const;

    // compute tangent space
    static void ComputeTangents(std::vector<Vertex> &vertices,
                                const std::vector<uint32_t> &indices);

private:
    Mesh() {}
    void Init(
        const std::vector<Vertex> &vertices,
        const std::vector<uint32_t> &indices,
        uint32_t primitiveType);

    uint32_t m_primitiveType{GL_TRIANGLES};
    VertexLayoutUPtr m_vertexLayout;
    BufferPtr m_vertexBuffer;
    BufferPtr m_indexBuffer;
    MaterialPtr m_material;
};

#endif // __MESH_H__
