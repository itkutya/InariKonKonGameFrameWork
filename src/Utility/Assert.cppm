export module Assert;

import std;

import Log;

export namespace ikk
{
    void assert(bool condition, std::source_location location = std::source_location::current()) noexcept
    {
        if (condition == false)
        {
            Log::Critical("File: {}, function: {}, {}:{}",
                location.file_name(), location.function_name(), location.column(), location.line());
            std::abort();
        }
    }

    void assert(bool condition, std::string_view message, std::source_location location = std::source_location::current()) noexcept
    {
        if (condition == false)
        {
            Log::Critical("{}, File: {}, function: {}, {}:{}", message,
                location.file_name(), location.function_name(), location.column(), location.line());
            std::abort();
        }
    }
}