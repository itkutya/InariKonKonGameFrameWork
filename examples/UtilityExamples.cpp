#include <cassert>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

import Utility;
import TypeUtils;

namespace
{
    enum class Access : std::uint8_t
    {
        Read = 1 << 0,
        Write = 1 << 1,
        Execute = 1 << 2
    };

    struct Node final : ikk::Hierarchy<Node>
    {
        [[nodiscard]] const Node* parent() const noexcept
        {
            return static_cast<const Hierarchy<Node>&>(*this).getParent();
        }

        [[nodiscard]] const std::vector<Node*>& children() const noexcept
        {
            return static_cast<const Hierarchy<Node>&>(*this).getChildren();
        }
    };
}

int main()
{
    using namespace ikk;

    // Build configuration, versions, and type traits.
    static_assert(isDebugBuild() != isReleaseBuild());
    constexpr Version version = getProjectVersion();
    static_assert(version.major == 0 && version.minor == 0 && version.patch == 2);
    static_assert(Is<int>::SpecializationOf_v<std::vector> == false);
    static_assert(Is<std::vector<int>>::SpecializationOf_v<std::vector>);
    static_assert(Is<int>::AlternativeOf_v<std::variant<int, float>>);
    static_assert(!Is<char>::AlternativeOf_v<std::variant<int, float>>);
    static_assert(Are<int, float>::UniqueTypes_v);
    static_assert(!Are<int, float, int>::UniqueTypes_v);

    // Errors keep their message as a string view.
    constexpr Error error{"example error"};
    static_assert(error.what() == std::string_view{"example error"});

    // Flags combine enum values and test membership.
    {
        constexpr Flag permissions{Access::Read};
        constexpr auto readWrite = permissions | Access::Write;
        static_assert(readWrite.contains(Access::Read));
        static_assert(readWrite.contains(Access::Write));
        static_assert(!readWrite.contains(Access::Execute));
        Flag<Access> mutableFlags{Access::Read};
        mutableFlags |= Access::Execute;
        assert(mutableFlags.contains(Access::Execute));
    }

    // Colors offer named values and clamped float channels.
    {
        constexpr Color red = Color::Red();
        static_assert(red.r == 255 && red.g == 0 && red.b == 0 && red.a == 255);
        constexpr Color transparent = Color::Transparent();
        static_assert(transparent.a == 0);
        constexpr Colorf miku = Colorf::Miku();
        static_assert(miku.r.value() > 0.0f && miku.r.value() < 1.0f);
        static_assert(Color::CornflowerBlue().b == 237);
    }

    // Hierarchy maintains parent/child links and prevents cycles/duplicates.
    {
        Node root;
        Node child;
        Node otherParent;
        child.setParent(root);
        assert(child.parent() == &root);
        assert(root.children().size() == 1 && root.children().front() == &child);
        root.addChild(child);
        assert(root.children().size() == 1);
        root.setParent(child); // A cycle is ignored.
        assert(root.parent() == nullptr);
        child.setParent(otherParent); // Reparenting detaches the old parent.
        assert(root.children().empty());
        assert(otherParent.children().size() == 1);
        child.removeParent();
        assert(child.parent() == nullptr && otherParent.children().empty());
    }

    // Time helpers and stopwatch state transitions.
    {
        constexpr auto duration = seconds(2) + milliseconds(500);
        static_assert(duration.asMilliseconds().count() == 2500);
        static_assert((duration / 2).asMicroseconds().count() == 1'250'000);
        static_assert((3_ms).asMilliseconds().count() == 3);

        Stopwatch stopwatch{true};
        assert(stopwatch.isRunning());
        stopwatch.stop();
        assert(!stopwatch.isRunning());
        assert(stopwatch.getElapsedTime() >= Time{});
        stopwatch.start();
        assert(stopwatch.isRunning());
        (void)stopwatch.restart();
        assert(stopwatch.isRunning());
        stopwatch.reset();
        assert(!stopwatch.isRunning() && stopwatch.getElapsedTime() == Time{});
    }

    // Logging accepts formatted messages and severity levels.
    Log::Info("UtilityExamples completed (version {}.{}.{})", version.major, version.minor, version.patch);

    return 0;
}
