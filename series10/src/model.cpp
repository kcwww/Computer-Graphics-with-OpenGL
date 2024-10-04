#include "model.h"
#include "texture.h"

// model 을 로드하는 함수
ModelUPtr Model::Load(const std::string& filename) {
    auto model = ModelUPtr(new Model());
    if (!model->LoadByAssimp(filename))
        return nullptr;
    return std::move(model);
}

// assimp 를 이용하여 model 을 로드하는 함수
bool Model::LoadByAssimp(const std::string& filename) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false;
    }

    auto dirname = filename.substr(0, filename.find_last_of("/")); // 디렉토리 경로
    auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> TexturePtr { // texture 로드 함수
        if (material->GetTextureCount(type) <= 0)
            return nullptr;
        aiString filepath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath); 
        auto image = Image::Load(dirname + "/" + filepath.C_Str());
        if (!image)
            return nullptr;
        return Texture::CreateFromImage(image.get());
    };
    
    // 하나의 모델에는 여러개의 material 이 존재할 수 있어 m_materials 에 저장
    for (uint32_t i = 0; i < scene->mNumMaterials; i++) { // scene->mMaterials 에 있는 material 정보를 읽어와서 m_materials 에 저장
        auto material = scene->mMaterials[i]; 
        auto glMaterial = Material::Create();
        glMaterial->diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
        glMaterial->specular = LoadTexture(material, aiTextureType_SPECULAR);
        m_materials.push_back(std::move(glMaterial));
    }

    ProcessNode(scene->mRootNode, scene); // scene->mRootNode 의 mesh 정보를 읽어와서 m_meshes 에 저장
    return true;
}

// mesh 정보를 읽어와서 m_meshes 에 저장
void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices); // mesh 의 vertex 개수만큼 vertices 를 resize
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) { // mesh 의 vertex 정보를 읽어와서 vertices 에 저장
        auto& v = vertices[i];
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        v.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    std::vector<uint32_t> indices;
    indices.resize(mesh->mNumFaces * 3); // mesh 의 face 개수 * 3 만큼 indices 를 resize
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        indices[3*i  ] = mesh->mFaces[i].mIndices[0]; // mesh 의 face 정보를 읽어와서 indices 에 저장
        indices[3*i+1] = mesh->mFaces[i].mIndices[1];
        indices[3*i+2] = mesh->mFaces[i].mIndices[2];
    }

    auto glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (mesh->mMaterialIndex >= 0)
        glMesh->SetMaterial(m_materials[mesh->mMaterialIndex]);

    m_meshes.push_back(std::move(glMesh));
}

// node 의 mesh 정보를 읽어와서 ProcessMesh 호출, 노드의 자식 노드에 대해서도 ProcessNode 호출
void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        auto meshIndex = node->mMeshes[i];
        auto mesh = scene->mMeshes[meshIndex];
        ProcessMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::Draw(const Program* program) const {
    for (auto& mesh: m_meshes) {
        mesh->Draw(program);
    }
}
