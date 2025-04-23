#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::ProcessInput(GLFWwindow *window)
{
    if (!m_cameraControl)
        return;
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);

    m_framebuffer = Framebuffer::Create({Texture::Create(std::max(1, width), std::max(1, height), GL_RGBA)}); // 화면이 리사이징 될때 framebuffer도 리사이징

    m_deferGeoFramebuffer = Framebuffer::Create({Texture::Create(width, height, GL_RGBA16F, GL_FLOAT),
                                                 Texture::Create(width, height, GL_RGBA16F, GL_FLOAT),
                                                 Texture::Create(width, height, GL_RGBA, GL_UNSIGNED_BYTE)}); // deferred geometry pass framebuffer

    // ssao
    m_ssaoFramebuffer = Framebuffer::Create({Texture::Create(width, height, GL_RED)}); // ssao framebuffer 1 채널
}

void Context::MouseMove(double x, double y)
{
    if (!m_cameraControl)
        return;
    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f)
        m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f)
        m_cameraYaw -= 360.0f;

    if (m_cameraPitch > 89.0f)
        m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f)
        m_cameraPitch = -89.0f;

    m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            m_prevMousePos = glm::vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_cameraControl = false;
        }
    }
}

void Context::DrawScene(const glm::mat4 &view,
                        const glm::mat4 &projection,
                        const Program *program)
{

    program->Use();
    auto modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 1.0f, 40.0f)); // 지면
    auto transform = projection * view * modelTransform;
    program->SetUniform("transform", transform);
    program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(program);
    m_box->Draw(program);

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.75f, -4.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    program->SetUniform("transform", transform);
    program->SetUniform("modelTransform", modelTransform);
    m_box1Material->SetToProgram(program);
    m_box->Draw(program);

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    program->SetUniform("transform", transform);
    program->SetUniform("modelTransform", modelTransform);
    m_box2Material->SetToProgram(program);
    m_box->Draw(program);

    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.75f, -2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    program->SetUniform("transform", transform);
    program->SetUniform("modelTransform", modelTransform);
    m_box2Material->SetToProgram(program);
    m_box->Draw(program);

    // draw model
    modelTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.55f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    transform = projection * view * modelTransform;
    program->SetUniform("transform", transform);
    program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(program);
}

void Context::Render()
{
    if (ImGui::Begin("ui window"))
    {
        if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("l.position", glm::value_ptr(m_light.position), 0.01f);
            ImGui::DragFloat3("l.direction", glm::value_ptr(m_light.direction), 0.01f);
            ImGui::DragFloat2("l.cutoff", glm::value_ptr(m_light.cutoff), 0.1f, 0.0f, 180.0f);
            ImGui::DragFloat("l.distance", &m_light.distance, 0.1f, 0.0f, 1000.0f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(m_light.specular));
            ImGui::Checkbox("flash light mode", &m_flashLightMode);
            // blinn check
            ImGui::Checkbox("l.blinn", &m_blinn);
            ImGui::Checkbox("l.directional", &m_light.directional);
            ImGui::DragFloat("ssao radius", &m_ssaoRadius, 0.01f, 0.0f, 5.0f);
        }

        if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("m.shininess", &m_material->shininess, 1.0f, 1.0f, 256.0f);
        }

        ImGui::Checkbox("animation", &m_animation);

        // shadow map depth 확인
        ImGui::Image((ImTextureID)m_shadowMap->GetShadowMap()->Get(),
                     ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
        {
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        }
        ImGui::DragFloat("gamma", &m_gamma, 0.01f, 0.0f, 2.0f);
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera"))
        {
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
    }
    ImGui::End();

    // G-Buffers 확인
    if (ImGui::Begin("G-Buffers"))
    {
        const char *bufferNames[] = {
            "position",
            "normal",
            "albedo/specular",
        };
        static int bufferSelect = 0;
        ImGui::Combo("buffer", &bufferSelect, bufferNames, 3);
        float width = ImGui::GetContentRegionAvailWidth();
        float height = width * ((float)m_height / (float)m_width);
        auto selectedAttachment = m_deferGeoFramebuffer->GetColorAttachment(bufferSelect);
        ImGui::Image((ImTextureID)selectedAttachment->Get(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();

    // ssao imgui
    if (ImGui::Begin("SSAO"))
    {
        float width = ImGui::GetContentRegionAvailWidth();
        float height = width * ((float)m_height / (float)m_width);
        ImGui::Image((ImTextureID)m_ssaoFramebuffer->GetColorAttachment()->Get(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();

    m_cameraFront =
        glm::rotate(glm::mat4(1.0f),
                    glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
                    glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    auto projection = glm::perspective(glm::radians(45.0f),
                                       (float)m_width / (float)m_height, 0.01f, 100.0f);
    auto view = glm::lookAt(
        m_cameraPos,
        m_cameraPos + m_cameraFront,
        m_cameraUp);

    //  shadow map render 1st pass
    auto lightView = glm::lookAt(m_light.position,
                                 m_light.position + m_light.direction,
                                 glm::vec3(0.0f, 1.0f, 0.0f)); // 빛 방향으로부터 view matrix 생성

    auto lightProjection = m_light.directional
                               ? glm::ortho(-10.0f, 10.0f,
                                            -10.0f, 10.0f,
                                            1.0f, 30.0f)
                               : glm::perspective(glm::radians((m_light.cutoff[0] + m_light.cutoff[1]) * 2.0f),
                                                  1.0f, 1.0f, 20.0f); // 빛의 시야각을 이용하여 projection matrix 생성
    m_shadowMap->Bind();                                              // shadow map framebuffer 바인딩
    glClear(GL_DEPTH_BUFFER_BIT);                                     // 깊이 버퍼 초기화
    glViewport(0, 0,
               m_shadowMap->GetShadowMap()->GetWidth(),
               m_shadowMap->GetShadowMap()->GetHeight());                    // shadow map 크기로 viewport 설정
    m_simpleProgram->Use();                                                  // shadow map을 위한 shader 사용
    m_simpleProgram->SetUniform("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // 색상 설정
    DrawScene(lightView, lightProjection, m_simpleProgram.get());            // 장면 그리기

    // defer geometry pass
    m_deferGeoFramebuffer->Bind(); // geometry pass framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    m_deferGeoProgram->Use();
    DrawScene(view, projection, m_deferGeoProgram.get());

    // ssao pass
    m_ssaoFramebuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    m_ssaoProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    m_deferGeoFramebuffer->GetColorAttachment(0)->Bind(); // position
    glActiveTexture(GL_TEXTURE1);
    m_deferGeoFramebuffer->GetColorAttachment(1)->Bind(); // normal

    // ssao noise texture
    glActiveTexture(GL_TEXTURE2);
    m_ssaoNoiseTexture->Bind(); // ssao noise texture

    glActiveTexture(GL_TEXTURE0);
    m_ssaoProgram->SetUniform("gPosition", 0);
    m_ssaoProgram->SetUniform("gNormal", 1);

    // ssao noise texture
    m_ssaoProgram->SetUniform("texNoise", 2);
    m_ssaoProgram->SetUniform("noiseScale", glm::vec2(
                                                (float)m_width / (float)m_ssaoNoiseTexture->GetWidth(),
                                                (float)m_height / (float)m_ssaoNoiseTexture->GetHeight())); // ssao noise texture 크기 설정

    // ssao sample kernel
    m_ssaoProgram->SetUniform("radius", m_ssaoRadius);
    for (size_t i = 0; i < m_ssaoSamples.size(); i++)
    {
        auto sampleName = fmt::format("samples[{}]", i);
        m_ssaoProgram->SetUniform(sampleName, m_ssaoSamples[i]);
    }

    m_ssaoProgram->SetUniform("transform", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)));
    m_ssaoProgram->SetUniform("view", view);
    m_ssaoProgram->SetUniform("projection", projection);
    m_plane->Draw(m_ssaoProgram.get());

    // blit framebuffer
    Framebuffer::BindToDefault();        // default framebuffer 바인딩
    glViewport(0, 0, m_width, m_height); // 원래 viewport 크기로 설정
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    // frame buffer 사용
    // m_framebuffer->Bind();

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // deferred lighting pass
    m_deferLightProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    m_deferGeoFramebuffer->GetColorAttachment(0)->Bind(); // position
    glActiveTexture(GL_TEXTURE1);
    m_deferGeoFramebuffer->GetColorAttachment(1)->Bind(); // normal
    glActiveTexture(GL_TEXTURE2);
    m_deferGeoFramebuffer->GetColorAttachment(2)->Bind(); // albedo
    glActiveTexture(GL_TEXTURE0);
    m_deferLightProgram->SetUniform("gPosition", 0);
    m_deferLightProgram->SetUniform("gNormal", 1);
    m_deferLightProgram->SetUniform("gAlbedoSpec", 2);
    for (size_t i = 0; i < m_deferLights.size(); i++)
    {
        auto posName = fmt::format("lights[{}].position", i);
        auto colorName = fmt::format("lights[{}].color", i);
        m_deferLightProgram->SetUniform(posName, m_deferLights[i].position);
        m_deferLightProgram->SetUniform(colorName, m_deferLights[i].color);
    }
    m_deferLightProgram->SetUniform("transform", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)));
    m_plane->Draw(m_deferLightProgram.get());

    // blit framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_deferGeoFramebuffer->Get()); // read framebuffer 로 설정
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);                            // default framebuffer로 설정
    glBlitFramebuffer(0, 0, m_width, m_height,
                      0, 0, m_width, m_height,
                      GL_DEPTH_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // draw light
    m_simpleProgram->Use();
    for (size_t i = 0; i < m_deferLights.size(); i++)
    {
        m_simpleProgram->SetUniform("color", glm::vec4(m_deferLights[i].color, 1.0f));
        glm::vec4(m_deferLights[i].color, 1.0f);
        m_simpleProgram->SetUniform("transform", projection * view * glm::translate(glm::mat4(1.0f), m_deferLights[i].position) *
                                                     glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        m_box->Draw(m_simpleProgram.get());
    }

    // // skybox render
    // auto skyboxModelTransform = glm::translate(glm::mat4(1.0), m_cameraPos) *
    //                             glm::scale(glm::mat4(1.0), glm::vec3(50.0f));
    // m_skyboxProgram->Use();
    // m_cubeTexture->Bind();
    // m_skyboxProgram->SetUniform("skybox", 0);
    // m_skyboxProgram->SetUniform("transform", projection * view * skyboxModelTransform);
    // m_box->Draw(m_skyboxProgram.get());

    // glm::vec3 lightPos = m_light.position;
    // glm::vec3 lightDir = m_light.direction;
    // if (m_flashLightMode)
    // {
    //     lightPos = m_cameraPos;
    //     lightDir = m_cameraFront;
    // }
    // else
    // {
    //     auto lightModelTransform =
    //         glm::translate(glm::mat4(1.0), m_light.position) *
    //         glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    //     m_simpleProgram->Use();
    //     m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
    //     m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
    //     m_box->Draw(m_simpleProgram.get());
    // }

    // // shadow map을 위한 shader 사용
    // m_lightShadowProgram->Use();
    // m_lightShadowProgram->SetUniform("viewPos", m_cameraPos);
    // m_lightShadowProgram->SetUniform("light.directional", m_light.directional ? 1 : 0);
    // m_lightShadowProgram->SetUniform("light.position", lightPos);
    // m_lightShadowProgram->SetUniform("light.direction", lightDir);
    // m_lightShadowProgram->SetUniform("light.cutoff", glm::vec2(
    //                                                      cosf(glm::radians(m_light.cutoff[0])),
    //                                                      cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1]))));
    // m_lightShadowProgram->SetUniform("light.attenuation", GetAttenuationCoeff(m_light.distance));
    // m_lightShadowProgram->SetUniform("light.ambient", m_light.ambient);
    // m_lightShadowProgram->SetUniform("light.diffuse", m_light.diffuse);
    // m_lightShadowProgram->SetUniform("light.specular", m_light.specular);
    // m_lightShadowProgram->SetUniform("blinn", (m_blinn ? 1 : 0));
    // m_lightShadowProgram->SetUniform("lightTransform", lightProjection * lightView);
    // glActiveTexture(GL_TEXTURE3); // 3번 텍스처 슬롯을 사용하는 이유는
    //                               // shadow map을 3번 슬롯에 바인딩하기 때문
    // m_shadowMap->GetShadowMap()->Bind();
    // m_lightShadowProgram->SetUniform("shadowMap", 3);
    // glActiveTexture(GL_TEXTURE0);

    // DrawScene(view, projection, m_lightShadowProgram.get());

    // // normal map
    // auto modelTransform =
    //     glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f)) *
    //     glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // m_normalProgram->Use();
    // m_normalProgram->SetUniform("viewPos", m_cameraPos);
    // m_normalProgram->SetUniform("lightPos", m_light.position);

    // glActiveTexture(GL_TEXTURE0);
    // m_brickDiffuseTexture->Bind();
    // m_normalProgram->SetUniform("diffuse", 0);

    // glActiveTexture(GL_TEXTURE1);
    // m_brickNormalTexture->Bind();
    // m_normalProgram->SetUniform("normalMap", 1);

    // glActiveTexture(GL_TEXTURE0);
    // m_normalProgram->SetUniform("modelTransform", modelTransform);
    // m_normalProgram->SetUniform("transform", projection * view * modelTransform);

    // m_plane->Draw(m_normalProgram.get());
}

bool Context::Init()
{
    glEnable(GL_MULTISAMPLE); // MSAA

    glEnable(GL_DEPTH_TEST);
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    m_box = Mesh::CreateBox();

    m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    if (!m_simpleProgram)
        return false;

    m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    if (!m_program)
        return false;

    m_textureProgram = Program::Create("./shader/texture.vs", "./shader/texture.fs");
    if (!m_textureProgram)
        return false;

    // m_postProgram = Program::Create("./shader/texture.vs", "./shader/invert.fs");
    m_postProgram = Program::Create("./shader/texture.vs", "./shader/gamma.fs");
    if (!m_postProgram)
        return false;

    m_material = Material::Create();
    m_material->diffuse = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4,
                                      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
            .get());

    m_material->specular = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4,
                                      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
            .get());

    // gray texture
    TexturePtr darkGrayTexture = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)).get());

    TexturePtr grayTexture = Texture::CreateFromImage(
        Image::CreateSingleColorImage(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());

    m_planeMaterial = Material::Create();
    m_planeMaterial->diffuse = Texture::CreateFromImage(
        Image::Load("./image/marble.jpg").get());
    m_planeMaterial->specular = grayTexture;
    m_planeMaterial->shininess = 4.0f;

    m_box1Material = Material::Create();
    m_box1Material->diffuse = Texture::CreateFromImage(
        Image::Load("./image/container.jpg").get());
    m_box1Material->specular = darkGrayTexture;
    m_box1Material->shininess = 16.0f;

    m_box2Material = Material::Create();
    m_box2Material->diffuse = Texture::CreateFromImage(
        Image::Load("./image/container2.png").get());
    m_box2Material->specular = Texture::CreateFromImage(
        Image::Load("./image/container2_specular.png").get());
    m_box2Material->shininess = 64.0f;

    // plane mesh 생성, window texture 로 material 설정
    m_plane = Mesh::CreatePlane();
    m_windowTexture = Texture::CreateFromImage(Image::Load("./image/blending_transparent_window.png").get());

    // cube map texture 생성
    auto cubeRight = Image::Load("./image/skybox/right.jpg", false);
    auto cubeLeft = Image::Load("./image/skybox/left.jpg", false);
    auto cubeTop = Image::Load("./image/skybox/top.jpg", false);
    auto cubeBottom = Image::Load("./image/skybox/bottom.jpg", false);
    auto cubeFront = Image::Load("./image/skybox/front.jpg", false);
    auto cubeBack = Image::Load("./image/skybox/back.jpg", false);
    m_cubeTexture = CubeTexture::CreateFromImages({cubeRight.get(), cubeLeft.get(),
                                                   cubeTop.get(), cubeBottom.get(),
                                                   cubeFront.get(), cubeBack.get()});
    m_skyboxProgram = Program::Create("./shader/skybox.vs", "./shader/skybox.fs");
    if (!m_skyboxProgram)
        return false;
    m_envMapProgram = Program::Create("./shader/env_map.vs", "./shader/env_map.fs");
    if (!m_envMapProgram)
        return false;

    // init grass
    m_grassTexture = Texture::CreateFromImage(Image::Load("./image/grass.png").get());
    m_grassProgram = Program::Create("./shader/grass.vs", "./shader/grass.fs");
    m_grassPos.resize(10000);
    for (size_t i = 0; i < m_grassPos.size(); i++)
    {
        m_grassPos[i].x = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;
        m_grassPos[i].z = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;
        m_grassPos[i].y = glm::radians((float)rand() / (float)RAND_MAX * 360.0f);
    }
    // instance grass
    m_grassInstance = VertexLayout::Create();
    m_grassInstance->Bind();
    m_plane->GetVertexBuffer()->Bind();
    m_grassInstance->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    m_grassInstance->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    m_grassInstance->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));

    m_grassPosBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, m_grassPos.data(), sizeof(glm::vec3), m_grassPos.size());
    m_grassPosBuffer->Bind();
    m_grassInstance->SetAttrib(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(3, 1); // instance data로 사용할 버퍼를 1로 설정
    m_plane->GetIndexBuffer()->Bind();

    // shadow map
    m_shadowMap = ShadowMap::Create(1024, 1024);
    m_lightShadowProgram = Program::Create("./shader/lighting_shadow.vs",
                                           "./shader/lighting_shadow.fs");

    // normal map
    m_brickDiffuseTexture = Texture::CreateFromImage(
        Image::Load("./image/brickwall.jpg", false).get()); // false 는 flip y
    m_brickNormalTexture = Texture::CreateFromImage(
        Image::Load("./image/brickwall_normal.jpg", false).get());
    m_normalProgram = Program::Create("./shader/normal.vs",
                                      "./shader/normal.fs");
    if (!m_normalProgram)
    {
        SPDLOG_ERROR("normal program init failed");
        return false;
    }

    // deferred shading
    m_deferGeoProgram = Program::Create("./shader/defer_geo.vs", "./shader/defer_geo.fs");
    if (!m_deferGeoProgram)
    {
        SPDLOG_ERROR("defer geo program init failed");
        return false;
    }

    // deferred lighting program
    m_deferLightProgram = Program::Create("./shader/defer_light.vs", "./shader/defer_light.fs");
    if (!m_deferLightProgram)
    {
        SPDLOG_ERROR("defer light program init failed");
        return false;
    }
    // deferred light
    m_deferLights.resize(32);
    for (size_t i = 0; i < m_deferLights.size(); i++)
    {
        m_deferLights[i].position = glm::vec3(RandomRange(-10.0f, 10.0f),
                                              RandomRange(1.0f, 4.0f),
                                              RandomRange(-10.0f, 10.0f));
        m_deferLights[i].color = glm::vec3(RandomRange(0.05f, 0.3f),
                                           RandomRange(0.05f, 0.3f),
                                           RandomRange(0.05f, 0.3f));
    }

    // ssao program
    m_ssaoProgram = Program::Create("./shader/ssao.vs", "./shader/ssao.fs");
    if (!m_ssaoProgram)
    {
        SPDLOG_ERROR("ssao program init failed");
        return false;
    }
    // test model
    m_model = Model::Load("./model/backpack.obj");

    // ssao noise texture
    std::vector<glm::vec3> ssaoNoise;
    ssaoNoise.resize(16);
    for (size_t i = 0; i < ssaoNoise.size(); i++)
    {
        // randomly selected tangent direction
        glm::vec3 sample(RandomRange(-1.0f, 1.0f),
                         RandomRange(-1.0f, 1.0f),
                         0.0f);
        ssaoNoise[i] = sample;
    }

    m_ssaoNoiseTexture = Texture::Create(4, 4, GL_RGB16F, GL_FLOAT);
    m_ssaoNoiseTexture->Bind();
    m_ssaoNoiseTexture->SetFilter(GL_NEAREST, GL_NEAREST);
    m_ssaoNoiseTexture->SetWrap(GL_REPEAT, GL_REPEAT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4, 4, GL_RGB, GL_FLOAT, ssaoNoise.data());
    if (!m_ssaoNoiseTexture)
    {
        SPDLOG_ERROR("ssao noise texture init failed");
        return false;
    }
    SPDLOG_INFO("context init success");

    // init ssao sample kernel
    m_ssaoSamples.resize(64);
    for (size_t i = 0; i < m_ssaoSamples.size(); i++)
    {
        // uniformly randomized point in unit hemisphere
        glm::vec3 sample(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(0.0f, 1.0f));

        sample = glm::normalize(sample) * RandomRange();

        // scale for slightly shift to center
        float t = (float)i / (float)m_ssaoSamples.size();
        float t2 = t * t;
        float scale = (1.0f - t2) * 0.1f + t2 * 1.0f;

        m_ssaoSamples[i] = sample * scale;
    }

    return true;
}
