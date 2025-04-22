#ifndef __MODEL_H__
#define __MODEL_H__

#include "common.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model);
class Model {
public:
    static ModelUPtr Load(const std::string& filename);

    int GetMeshCount() const { return (int)m_meshes.size(); } // mesh 개수 반환
    MeshPtr GetMesh(int index) const { return m_meshes[index]; }
    void Draw(const Program* program) const;

private:
    Model() {}
    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene); // mesh 정보를 읽어와서 m_meshes 에 저장
    void ProcessNode(aiNode* node, const aiScene* scene); // node 의 mesh 정보를 읽어와서 ProcessMesh 호출

    std::vector<MeshPtr> m_meshes;
    std::vector<MaterialPtr> m_materials;
};

#endif // __MODEL_H__
