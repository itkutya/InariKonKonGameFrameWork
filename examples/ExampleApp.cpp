import InariKonKon;

class TestScene : public ikk::IScene
{
public:
    void onCreate() noexcept override
    {
        ikk::Log::Info("Scene created!");
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

    void onRender() noexcept override
    {
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
        app->getSceneManager().createScene<TestScene>(sceneOptions);
        app->run();
    }
}