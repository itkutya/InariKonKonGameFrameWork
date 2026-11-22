import InariKonKon;

constexpr const char* vertexSource = R"(
            #version 460 core

            layout(location = 0) in vec3 position;

            layout (std140, binding = 0) uniform Matrices
            {
                mat4 projection;
                mat4 view;
            };
            uniform mat4 model;

            void main()
            {
                gl_Position = projection * view * model * vec4(position, 1.0);
            })";

constexpr const char* fragmentSource = R"(
            #version 460 core

            layout(location = 0) out vec4 color;

            float linearizeDepth(float depth, float near, float far)
            {
                float z = depth * 2.0 - 1.0;
                return (2.0 * near * far) / (far + near - z * (far - near));
            }

            void main()
            {
                color = vec4(vec3(linearizeDepth(gl_FragCoord.z, 0.1, 100.0)), 1.0);
            })";

class TestScene : public ikk::IScene
{
public:
    ikk::Application* app = nullptr;

    ikk::EntityManager entityManager{};

    ikk::Entity entity{};
    ikk::Mesh mesh{};
    ikk::Material material{};

    ikk::Camera<ikk::Projection::Perspective> camera{};

    ikk::Shader<ikk::ShaderType::Vertex> vertexShader{std::string_view{vertexSource}};
    ikk::Shader<ikk::ShaderType::Fragment> fragmentShader{std::string_view{fragmentSource}};

    explicit TestScene(ikk::Application* app) noexcept
        : app{app}
    {
    }

    void onCreate() noexcept override
    {
        ikk::Log::Info("Scene created!");

        mesh.vertexBuffer = ikk::VertexBuffer
            {
                ikk::VertexFormat<ikk::Position2D, ikk::Colorf>{},
                {
                    ikk::vertex(ikk::Position2D{-1.f, -1.f}, ikk::Colorf::Teto()),
                    ikk::vertex(ikk::Position2D{ 1.f, -1.f}, ikk::Colorf::Miku()),
                    ikk::vertex(ikk::Position2D{ 1.f,  1.f}, ikk::Colorf::Rin()),
                    ikk::vertex(ikk::Position2D{-1.f,  1.f}, ikk::Colorf::CornflowerBlue())
                }
            };
        mesh.indexBuffer =
            {
                0, 1, 2, 2, 3, 0
            };

        material.vertexShader = &ikk::BuiltInShaders::Colored2D::vertex;
        material.fragmentShader = &ikk::BuiltInShaders::Colored2D::fragment;

        entity = this->entityManager.createEntity();
        entity.addComponent<ikk::MeshRenderer>(mesh, material);
        entity.addComponent<ikk::Transform3D>();


        const auto view = this->camera.getViewMatrix();
        const auto proj = this->camera.getProjectionMatrix(1280.f / 720.f);

        struct Matrices
        {
            ikk::Mat4x4f projection{};
            ikk::Mat4x4f view{};
        };

        //vertexShader.setUniformBufferObject<Matrices>(Matrices{ .projection = proj, .view = view });
        const ikk::Mat4x4f model = this->entity.getComponent<ikk::Transform3D>()->get().getWorldMatrix();
        vertexShader.setUniform("model", model);
    }

    void onDestroy() noexcept override
    {
        ikk::Log::Info("Scene destroyed!");
    }

    void onEnter() noexcept override
    {
        ikk::Log::Info("Scene entered!");
    }

    void onExit() noexcept override
    {
        ikk::Log::Info("Scene exited!");
    }

    void onEvent(const ikk::Event&) noexcept override
    {
    }

    void onUpdate(const ikk::Time&) noexcept override
    {
    }

    void onRender(const std::unique_ptr<ikk::IRenderer>& renderer) noexcept override
    {
        this->entityManager.update(ikk::RenderSystem{renderer.get()});
    }
};

int main()
{
    auto [major, minor, patch] = ikk::getProjectVersion();
    ikk::Log::Info("InariKonKon v{}.{}.{}", major, minor, patch);

    constexpr ikk::Application::CreateSettings appSettings
        {
            .windowSettings = { .title = u8"日本", .size = ikk::Vec2u{ 1280u, 720u } },
            .version = { .major = 0, .minor = 0, .patch = 1 },
            .renderer = ikk::TypeIdentity<ikk::OpenGL>{}
        };

    if (auto app = ikk::Application::create(appSettings);
        app.has_value() == true)
    {
        constexpr ikk::IScene::CreateOptions sceneOptions
            {
                .name = "TestScene",
                .replaceActiveScene = true
            };
        app->getSceneManager().createScene<TestScene>(sceneOptions, &app.value());
        app->run();
    }
}
