export module ProjectVersion;

import Version;

export namespace ikk
{
    consteval Version getProjectVersion() noexcept
    {
        return Version
            {
                .major = 0,
                .minor = 0,
                .patch = 2
            };
    }
}
