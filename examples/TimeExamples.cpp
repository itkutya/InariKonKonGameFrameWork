#include <cassert>
#include <chrono>
#include <concepts>
#include <type_traits>
#include <utility>

import Time;
import Stopwatch;

int main()
{
    // ============================================================
    // Time: Type aliases
    // ============================================================

    static_assert(std::same_as<ikk::Second::Rep, float>);
    static_assert(std::same_as<ikk::Second::Period, std::ratio<1>>);
    static_assert(std::same_as<ikk::Second::Duration,
                               std::chrono::duration<float, std::ratio<1>>>);

    static_assert(std::same_as<ikk::Millisecond::Rep, float>);
    static_assert(std::same_as<ikk::Millisecond::Period, std::milli>);
    static_assert(std::same_as<ikk::Microsecond::Rep, float>);
    static_assert(std::same_as<ikk::Microsecond::Period, std::micro>);

    // ============================================================
    // Time: Type concept
    // ============================================================

    static_assert(ikk::TimeConvertible<ikk::Second>);
    static_assert(ikk::TimeConvertible<ikk::Millisecond>);
    static_assert(ikk::TimeConvertible<ikk::Microsecond>);
    static_assert(ikk::TimeConvertible<ikk::Time<float, std::nano>>);

    static_assert(!ikk::TimeConvertible<float>);
    static_assert(!ikk::TimeConvertible<int>);
    static_assert(!ikk::TimeConvertible<std::chrono::seconds>);

    // ============================================================
    // Time: Default construction
    // ============================================================

    {
        constexpr ikk::Second time{};

        assert(time.toDuration<ikk::Second::Duration>().count() == 0.0f);
    }

    // ============================================================
    // Time: Construction from matching chrono duration
    // ============================================================

    {
        constexpr auto duration =
            std::chrono::duration<float, std::ratio<1>>{5.0f};

        constexpr ikk::Second time{duration};

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    // ============================================================
    // Time: Construction from different chrono duration
    // ============================================================

    {
        constexpr auto duration = std::chrono::seconds{5};

        constexpr ikk::Millisecond time{duration};

        static_assert(
            time.toDuration<ikk::Millisecond::Duration>().count() == 5000.0f
        );
    }

    {
        constexpr auto duration = std::chrono::milliseconds{2500};

        constexpr ikk::Second time{duration};

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 2.5f
        );
    }

    // ============================================================
    // Time: seconds()
    // ============================================================

    {
        constexpr auto time = ikk::seconds(5);

        static_assert(std::same_as<
            std::remove_cvref_t<decltype(time)>,
             std::remove_cvref_t<ikk::Second>
        >);

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    {
        constexpr auto time = ikk::seconds(2.5f);

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 2.5f
        );
    }

    // ============================================================
    // Time: milliseconds()
    // ============================================================

    {
        constexpr auto time = ikk::milliseconds(500);

        static_assert(std::same_as<
             std::remove_cvref_t<decltype(time)>,
             std::remove_cvref_t<ikk::Millisecond>
        >);

        static_assert(
            time.toDuration<ikk::Millisecond::Duration>().count() == 500.0f
        );
    }

    // ============================================================
    // Time: microseconds()
    // ============================================================

    {
        constexpr auto time = ikk::microseconds(2500);

        static_assert(std::same_as<
             std::remove_cvref_t<decltype(time)>,
             std::remove_cvref_t<ikk::Microsecond>
        >);

        static_assert(
            time.toDuration<ikk::Microsecond::Duration>().count() == 2500.0f
        );
    }

    // ============================================================
    // Time: User-defined literals
    // ============================================================

    {
        constexpr auto time = 5_ns;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::nano>>().count()
            == 5.0f
        );
    }

    {
        constexpr auto time = 5_us;

        static_assert(
            time.toDuration<ikk::Microsecond::Duration>().count()
            == 5.0f
        );
    }

    {
        constexpr auto time = 5_ms;

        static_assert(
            time.toDuration<ikk::Millisecond::Duration>().count()
            == 5.0f
        );
    }

    {
        constexpr auto time = 5_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count()
            == 5.0f
        );
    }

    {
        constexpr auto time = 2_min;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::ratio<60>>>().count()
            == 2.0f
        );
    }

    {
        constexpr auto time = 2_h;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::ratio<3600>>>().count()
            == 2.0f
        );
    }

    // ============================================================
    // Time: Floating-point literals
    // ============================================================

    {
        constexpr auto time = 2.5_ns;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::nano>>().count()
            == 2.5f
        );
    }

    {
        constexpr auto time = 2.5_us;

        static_assert(
            time.toDuration<ikk::Microsecond::Duration>().count()
            == 2.5f
        );
    }

    {
        constexpr auto time = 2.5_ms;

        static_assert(
            time.toDuration<ikk::Millisecond::Duration>().count()
            == 2.5f
        );
    }

    {
        constexpr auto time = 2.5_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count()
            == 2.5f
        );
    }

    {
        constexpr auto time = 2.5_min;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::ratio<60>>>().count()
            == 2.5f
        );
    }

    {
        constexpr auto time = 2.5_h;

        static_assert(
            time.toDuration<std::chrono::duration<float, std::ratio<3600>>>().count()
            == 2.5f
        );
    }

    // ============================================================
    // Time: asSeconds()
    // ============================================================

    {
        constexpr auto time = 2500_ms;
        constexpr auto seconds = time.asSeconds();

        static_assert(
            seconds.toDuration<ikk::Second::Duration>().count() == 2.5f
        );
    }

    // ============================================================
    // Time: asMilliseconds()
    // ============================================================

    {
        constexpr auto time = 2.5_s;
        constexpr auto milliseconds = time.asMilliseconds();

        static_assert(
            milliseconds
                .toDuration<ikk::Millisecond::Duration>()
                .count() == 2500.0f
        );
    }

    // ============================================================
    // Time: asMicroseconds()
    // ============================================================

    {
        constexpr auto time = 2.5_ms;
        constexpr auto microseconds = time.asMicroseconds();

        static_assert(
            microseconds
                .toDuration<ikk::Microsecond::Duration>()
                .count() == 2500.0f
        );
    }

    // ============================================================
    // Time: toDuration()
    // ============================================================

    {
        constexpr auto time = 2.5_s;

        constexpr auto milliseconds =
            time.toDuration<std::chrono::milliseconds>();

        static_assert(milliseconds.count() == 2500);
    }

    {
        constexpr auto time = 2500_ms;

        constexpr auto seconds =
            time.toDuration<std::chrono::duration<float>>();

        static_assert(seconds.count() == 2.5f);
    }

    {
        constexpr auto time = 2500_ms;

        constexpr auto microseconds =
            time.toDuration<std::chrono::microseconds>();

        static_assert(microseconds.count() == 2'500'000);
    }

    // ============================================================
    // Time: Copy construction
    // ============================================================

    {
        constexpr auto original = 5_s;
        constexpr auto copy = original;

        static_assert(
            copy.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    // ============================================================
    // Time: Move construction
    // ============================================================

    {
        auto original = 5_s;
        auto moved = std::move(original);

        assert(
            moved.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    // ============================================================
    // Time: Copy assignment
    // ============================================================

    {
        auto original = 5_s;
        auto target = 10_s;

        target = original;

        assert(
            target.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    // ============================================================
    // Time: Move assignment
    // ============================================================

    {
        auto original = 5_s;
        auto target = 10_s;

        target = std::move(original);

        assert(
            target.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    // ============================================================
    // Time: operator+=
    // ============================================================

    {
        auto time = 5_s;

        time += 2_s;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 7.0f
        );
    }

    // ============================================================
    // Time: operator-=
    // ============================================================

    {
        auto time = 5_s;

        time -= 2_s;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 3.0f
        );
    }

    // ============================================================
    // Time: operator+
    // ============================================================

    {
        constexpr auto time = 5_s + 2_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 7.0f
        );
    }

    // ============================================================
    // Time: operator-
    // ============================================================

    {
        constexpr auto time = 5_s - 2_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 3.0f
        );
    }

    // ============================================================
    // Time: operator*=
    // ============================================================

    {
        auto time = 5_s;

        time *= 2;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 10.0f
        );
    }

    {
        auto time = 5_s;

        time *= 2.5f;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 12.5f
        );
    }

    // ============================================================
    // Time: operator/=
    // ============================================================

    {
        auto time = 10_s;

        time /= 2;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    {
        auto time = 10_s;

        time /= 2.5f;

        assert(
            time.toDuration<ikk::Second::Duration>().count() == 4.0f
        );
    }

    // ============================================================
    // Time: operator*
    // ============================================================

    {
        constexpr auto time = 5_s * 2;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 10.0f
        );
    }

    {
        constexpr auto time = 5_s * 2.5f;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 12.5f
        );
    }

    // ============================================================
    // Time: operator/
    // ============================================================

    {
        constexpr auto time = 10_s / 2;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 5.0f
        );
    }

    {
        constexpr auto time = 10_s / 2.5f;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 4.0f
        );
    }

    // ============================================================
    // Time: comparison operators
    // ============================================================

    {
        constexpr auto a = 5_s;
        constexpr auto b = 5_s;
        constexpr auto c = 10_s;

        static_assert(a == b);
        static_assert(a != c);

        static_assert(a < c);
        static_assert(a <= b);
        static_assert(a <= c);

        static_assert(c > a);
        static_assert(c >= a);
        static_assert(c >= b);
    }

    // ============================================================
    // Time: negative values
    // ============================================================

    {
        constexpr auto time = -5_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == -5.0f
        );

        static_assert(time < 0_s);
    }

    // ============================================================
    // Time: zero values
    // ============================================================

    {
        constexpr auto time = 0_s;

        static_assert(
            time.toDuration<ikk::Second::Duration>().count() == 0.0f
        );

        static_assert(time == 0_s);
    }

    // ============================================================
    // Time: mixed chrono conversions
    // ============================================================

    {
        constexpr ikk::Second seconds{
            std::chrono::milliseconds{1500}
        };

        static_assert(
            seconds.toDuration<ikk::Second::Duration>().count() == 1.5f
        );
    }

    {
        constexpr ikk::Microsecond microseconds{
            std::chrono::seconds{2}
        };

        static_assert(
            microseconds
                .toDuration<ikk::Microsecond::Duration>()
                .count() == 2'000'000.0f
        );
    }

    // ============================================================
    // Stopwatch: default construction
    // ============================================================

    {
        const ikk::Stopwatch stopwatch;

        assert(stopwatch.isRunning() == false);
    }

    // ============================================================
    // Stopwatch: autostart
    // ============================================================

    {
        const ikk::Stopwatch stopwatch{true};

        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: start()
    // ============================================================

    {
        ikk::Stopwatch stopwatch;

        stopwatch.start();

        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: start() while already running
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto first = stopwatch.getElapsedTime<ikk::Millisecond>();

        stopwatch.start();

        const auto second = stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(second >= first);
        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: stop()
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.stop();

        assert(stopwatch.isRunning() == false);
    }

    // ============================================================
    // Stopwatch: stop() while already stopped
    // ============================================================

    {
        ikk::Stopwatch stopwatch;

        stopwatch.stop();

        assert(stopwatch.isRunning() == false);
    }

    // ============================================================
    // Stopwatch: elapsed time while running
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto elapsed =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(elapsed >= 0_ms);
        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: elapsed time while stopped
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.stop();

        const auto elapsed =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(elapsed >= 0_ms);
        assert(stopwatch.isRunning() == false);
    }

    // ============================================================
    // Stopwatch: different return durations
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto seconds =
            stopwatch.getElapsedTime<ikk::Second>();

        const auto milliseconds =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        const auto microseconds =
            stopwatch.getElapsedTime<ikk::Microsecond>();

        assert(seconds >= 0_s);
        assert(milliseconds >= 0_ms);
        assert(microseconds >= 0_us);
    }

    // ============================================================
    // Stopwatch: restart()
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto elapsed =
            stopwatch.restart<ikk::Millisecond>();

        assert(elapsed >= 0_ms);
        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: restart() while stopped
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.stop();

        const auto elapsed =
            stopwatch.restart<ikk::Millisecond>();

        assert(elapsed >= 0_ms);
        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: restart resets elapsed time
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto first =
            stopwatch.restart<ikk::Millisecond>();

        const auto second =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(second >= 0_ms);

        // The second measurement should start from the restart point.
        assert(second <= first + 100_ms);
    }

    // ============================================================
    // Stopwatch: stop -> start
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.stop();

        const auto stopped =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        stopwatch.start();

        assert(stopwatch.isRunning() == true);

        const auto running =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(running >= stopped);
    }

    // ============================================================
    // Stopwatch: stop -> start preserves elapsed time
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.stop();

        const auto beforeRestart =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        stopwatch.start();

        const auto immediatelyAfterStart =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(immediatelyAfterStart >= beforeRestart);
    }

    // ============================================================
    // Stopwatch: reset()
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.reset();

        assert(stopwatch.isRunning() == false);

        const auto elapsed =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(elapsed == 0_ms);
    }

    // ============================================================
    // Stopwatch: reset -> start
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        stopwatch.reset();

        assert(stopwatch.isRunning() == false);

        stopwatch.start();

        assert(stopwatch.isRunning() == true);

        const auto elapsed =
            stopwatch.getElapsedTime<ikk::Millisecond>();

        assert(elapsed >= 0_ms);
    }

    // ============================================================
    // Stopwatch: copy construction
    // ============================================================

    {
        ikk::Stopwatch original{true};
        ikk::Stopwatch copy{original};

        assert(copy.isRunning() == original.isRunning());
    }

    // ============================================================
    // Stopwatch: move construction
    // ============================================================

    {
        ikk::Stopwatch original{true};
        ikk::Stopwatch moved{std::move(original)};

        assert(moved.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: copy assignment
    // ============================================================

    {
        ikk::Stopwatch original{true};
        ikk::Stopwatch target;

        target = original;

        assert(target.isRunning() == original.isRunning());
    }

    // ============================================================
    // Stopwatch: move assignment
    // ============================================================

    {
        ikk::Stopwatch original{true};
        ikk::Stopwatch target;

        target = std::move(original);

        assert(target.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: repeated restart
    // ============================================================

    {
        ikk::Stopwatch stopwatch{true};

        const auto first =
            stopwatch.restart<ikk::Millisecond>();

        const auto second =
            stopwatch.restart<ikk::Millisecond>();

        const auto third =
            stopwatch.restart<ikk::Millisecond>();

        assert(first >= 0_ms);
        assert(second >= 0_ms);
        assert(third >= 0_ms);

        assert(stopwatch.isRunning() == true);
    }

    // ============================================================
    // Stopwatch: repeated start/stop
    // ============================================================

    {
        ikk::Stopwatch stopwatch;

        stopwatch.start();
        assert(stopwatch.isRunning() == true);

        stopwatch.stop();
        assert(stopwatch.isRunning() == false);

        stopwatch.start();
        assert(stopwatch.isRunning() == true);

        stopwatch.stop();
        assert(stopwatch.isRunning() == false);

        stopwatch.start();
        assert(stopwatch.isRunning() == true);
    }

    return 0;
}