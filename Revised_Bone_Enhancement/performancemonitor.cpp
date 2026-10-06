#define NOMINMAX
#include <windows.h>
#include <psapi.h>

double getCurrentMemoryMB()
{
    PROCESS_MEMORY_COUNTERS pmc{};

    if (GetProcessMemoryInfo(
            GetCurrentProcess(),
            &pmc,
            sizeof(pmc)))
    {
        return static_cast<double>(pmc.WorkingSetSize) /
               (1024.0 * 1024.0);
    }

    return 0.0;
}

double getPeakMemoryMB()
{
    PROCESS_MEMORY_COUNTERS pmc{};

    if (GetProcessMemoryInfo(
            GetCurrentProcess(),
            &pmc,
            sizeof(pmc)))
    {
        return static_cast<double>(pmc.PeakWorkingSetSize) /
               (1024.0 * 1024.0);
    }

    return 0.0;
}