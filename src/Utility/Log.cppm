module;

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "fmt/format.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

export module Log;

import NonCopyable;
import NonMovable;
import DebugUtils;
import Flag;

export namespace ikk
{
    class Log final : NonCopyable, NonMovable
    {
    public:
        enum struct Level : std::uint8_t
        {
            Info = 0,
            Warning,
            Error,
            Critical
        };

        enum struct Flags : std::uint8_t
        {
            None        = 0 << 0,
            Date        = 1 << 0,
            Severity    = 1 << 1,
            Debug       = 1 << 2,
            File        = 1 << 3,
            Default     = Date | Severity | Debug,
            All         = None | Date | Severity | Debug | File
        };

        template<class... Args>
        Log(Level level, Flag<Flags> flags, const fmt::format_string<Args...>& fmt, Args&&... args) noexcept;

        ~Log() noexcept = default;

        template<Log::Flags flags = Log::Flags::Default, class... Args>
        static void Info(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept;

        template<Log::Flags flags = Log::Flags::Default, class... Args>
        static void Warning(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept;

        template<Log::Flags flags = Log::Flags::Default, class... Args>
        static void Error(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept;

        template<Log::Flags flags = Log::Flags::Default, class... Args>
        static void Critical(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept;

        static void setFileLoggerPath(const std::filesystem::path& path) noexcept;
    private:
        struct State
        {
            std::mutex mutex{};
            std::filesystem::path filePath = "Logs/Logs.txt";
            std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink = nullptr;
            std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink = nullptr;
            std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers{};
        };

        static State& getState();
        static std::shared_ptr<spdlog::logger> getLogger(const std::string& pattern, bool toFile);
    };
}

namespace ikk
{
    template<class... Args>
    Log::Log(Level level, Flag<Flags> flags, const fmt::format_string<Args...>& fmt, Args&&... args) noexcept
    {
        if (flags.contains(Flags::Debug) == true && isReleaseBuild() == true) return;

        static constexpr std::string_view debug    = "[DEBUG] ";
        static constexpr std::string_view date     = "[%Y.%m.%d. %H:%M:%S] ";
        static constexpr std::string_view severity = "%^[%l]%$ ";
        static constexpr std::string_view message  = "%v ";
        static constexpr std::size_t maxPatternStringSize = debug.size() + date.size() + severity.size() + message.size();

        static constexpr auto convertLevel = [](Log::Level level) noexcept
        {
            switch (level)
            {
            case Level::Info:       return spdlog::level::info;
            case Level::Warning:    return spdlog::level::warn;
            case Level::Error:      return spdlog::level::err;
            case Level::Critical:   return spdlog::level::critical;
            }
        };

        try
        {
            std::string pattern{};
            pattern.reserve(maxPatternStringSize);

            if (flags.contains(Flags::Debug) == true)       pattern += debug;
            if (flags.contains(Flags::Date) == true)        pattern += date;
            if (flags.contains(Flags::Severity) == true)    pattern += severity;
                                                                pattern += message;
            if (pattern.empty() == false)                       pattern.pop_back(); //Remove space at the end

            const std::shared_ptr<spdlog::logger> logger = getLogger(pattern, flags.contains(Flags::File) == true);
            logger->log(convertLevel(level), fmt::format(fmt, std::forward<Args>(args)...));
        }
        catch (...)
        {
            std::fputs("Failed to log message\n", stderr);
        }
    }

    template<Log::Flags flags, class... Args>
    void Log::Info(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept
    {
        Log{Log::Level::Info, Flag<Log::Flags>{flags}, fmt, std::forward<Args>(args)...};
    }

    template<Log::Flags flags, class ... Args>
    void Log::Warning(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept
    {
        Log{Log::Level::Warning, Flag<Log::Flags>{flags}, fmt, std::forward<Args>(args)...};
    }

    template<Log::Flags flags, class ... Args>
    void Log::Error(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept
    {
        Log{Log::Level::Error, Flag<Log::Flags>{flags}, fmt, std::forward<Args>(args)...};
    }

    template<Log::Flags flags, class ... Args>
    void Log::Critical(const fmt::format_string<Args...>& fmt, Args&&... args) noexcept
    {
        Log{Log::Level::Critical, Flag<Log::Flags>{flags}, fmt, std::forward<Args>(args)...};
    }

    void Log::setFileLoggerPath(const std::filesystem::path& path) noexcept
    {
        try
        {
            State& state = getState();
            std::lock_guard lock{state.mutex};

            state.filePath = path;
            state.fileSink.reset();
            std::erase_if(state.loggers, [](const auto& kv) { return kv.first.starts_with("file|"); });
        }
        catch (...)
        {
            std::fputs("Failed to set file logger path\n", stderr);
        }
    }

    Log::State& Log::getState()
    {
        static State state{};
        return state;
    }

    std::shared_ptr<spdlog::logger> Log::getLogger(const std::string& pattern, bool toFile)
    {
        State& state = getState();
        std::lock_guard lock{state.mutex};

        const std::string key = (toFile == true ? "file|" : "console|") + pattern;
        if (const auto it = state.loggers.find(key); it != state.loggers.end())
            return it->second;

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        std::vector<spdlog::sink_ptr> sinks{consoleSink};

        if (toFile)
        {
            consoleSink->set_pattern(pattern + fmt::format(" [-> file: {}]", state.filePath.string()));

            if (state.fileSink == nullptr)
            {
                state.fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(state.filePath.string(), false);
                state.fileSink->set_pattern(pattern);
            }
            sinks.emplace_back(state.fileSink);
        }
        else
            consoleSink->set_pattern(pattern);

        auto logger = std::make_shared<spdlog::logger>(key, sinks.begin(), sinks.end());
        logger->flush_on(spdlog::level::warn);

        state.loggers.emplace(key, logger);
        return logger;
    }
}

export [[nodiscard]] inline constexpr ikk::Log::Flags operator|(ikk::Log::Flags lhs, ikk::Log::Flags rhs) noexcept
{
    return static_cast<ikk::Log::Flags>(std::to_underlying(lhs) | std::to_underlying(rhs));
}