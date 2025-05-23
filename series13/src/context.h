#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "framebuffer.h"
#include "shadow_map.h"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUPtr Create();
    void Render();
    void ProcessInput(GLFWwindow *window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);

    // draw scene
    void DrawScene(const glm::mat4 &view,
                   const glm::mat4 &projection,
                   const Program *program);

private:
    Context() {}
    bool Init();
    ProgramUPtr m_program;
    ProgramUPtr m_simpleProgram;
    // texture program
    ProgramUPtr m_textureProgram;
    // post processing program
    ProgramUPtr m_postProgram;
    // gamma
    float m_gamma{1.0f};

    MeshUPtr m_box;

    // window
    MeshUPtr m_plane;
    TexturePtr m_windowTexture;

    /// shadow map
    ShadowMapUPtr m_shadowMap;
    ProgramUPtr m_lightShadowProgram;

    // normal map
    TextureUPtr m_brickDiffuseTexture;
    TextureUPtr m_brickNormalTexture;
    ProgramUPtr m_normalProgram;

    int m_width{640};
    int m_height{480};

    // animation
    bool m_animation{true};

    // clear color
    glm::vec4 m_clearColor{glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};

    // light parameter
    struct Light
    {
        bool directional{false};
        glm::vec3 position{glm::vec3(2.0f, 4.0f, 4.0f)};
        glm::vec3 direction{glm::vec3(-0.5f, -1.5f, -1.0f)};
        glm::vec2 cutoff{glm::vec2(50.0f, 5.0f)};
        float distance{150.0f};
        glm::vec3 ambient{glm::vec3(0.1f, 0.1f, 0.1f)};
        glm::vec3 diffuse{glm::vec3(0.8f, 0.8f, 0.8f)};
        glm::vec3 specular{glm::vec3(1.0f, 1.0f, 1.0f)};
    };
    Light m_light;
    bool m_flashLightMode{false};
    // blinn
    bool m_blinn{true};

    // material parameter
    MaterialPtr m_material;

    // camera parameter
    bool m_cameraControl{false};
    glm::vec2 m_prevMousePos{glm::vec2(0.0f)};
    float m_cameraPitch{-20.0f};
    float m_cameraYaw{0.0f};
    glm::vec3 m_cameraFront{glm::vec3(0.0f, -1.0f, 0.0f)};
    glm::vec3 m_cameraPos{glm::vec3(0.0f, 2.5f, 8.0f)};
    glm::vec3 m_cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)};

    // material
    MaterialPtr m_planeMaterial;
    MaterialPtr m_box1Material;
    MaterialPtr m_box2Material;

    // frame buffer
    FramebufferUPtr m_framebuffer;

    // cubemap
    CubeTextureUPtr m_cubeTexture;
    ProgramUPtr m_skyboxProgram;
    ProgramUPtr m_envMapProgram;

    // grass
    TexturePtr m_grassTexture;
    ProgramUPtr m_grassProgram;
    std::vector<glm::vec3> m_grassPos;
    // instancing
    BufferUPtr m_grassPosBuffer;
    VertexLayoutUPtr m_grassInstance;
};

#endif // __CONTEXT_H__
