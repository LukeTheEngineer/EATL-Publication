/**
 * @file logger.c
 * @brief Logger definitions
 *
 * @date May 19th, 2024
 *
 * @copyright Copyright (c) 2023 Lukas R. Jackson
 *
 * @author Lukas R. Jackson (LukasJacksonEG@gmail.com)
 *
 * @license BSD-3-Clause License
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *          1. Redistributions of source code must retain the above copyright notice,
 *             this list of conditions and the following disclaimer.
 *
 *          2. Redistributions in binary form must reproduce the above copyright notice,
 *             this list of conditions and the following disclaimer in the documentation
 *             and/or other materials provided with the distribution.
 *
 *          3. Neither the name of the copyright holder nor the names of its
 *             contributors may be used to endorse or promote products derived from
 *             this software without specific prior written permission.
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *          AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *          IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *          FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *          DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *          SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *          CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *          OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *          OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

#endif

#ifdef __linux__

#include <sys/resource.h>

#endif

/* Local includes */
#include "ram-fs.h"
#include "common/logger.h"

#ifdef _WIN32

void enable_virtual_terminal_processing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void return_windows_memory_usage()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        printf("Working set size: %ld bytes\n", pmc.WorkingSetSize);
        printf("Peak working set size: %ld bytes\n", pmc.PeakWorkingSetSize);
        printf("Pagefile usage: %ld bytes\n", pmc.PagefileUsage);
        printf("Peak pagefile usage: %ld bytes\n", pmc.PeakPagefileUsage);
    }
    else
    {
        printf("Could not get memory info\n");
    }
}

#endif

#ifdef __linux__

void return_linux_memory_usage(void)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Maximum resident set size: %ld kilobytes\n", usage.ru_maxrss);
}

#endif

void event_occured(struct log_module *module, const char *message)
{
    if (module == NULL)
    {
        printf("Log module returned NULL\n");
        return;
    }
    if (module->callback)
    {
        module->callback(message);
        return;
    }
    else
    {
        printf("%s: An event occured \n", module->module_name);
    }
}

long long perform_calculation(struct log_module *module, long long a, long long b)
{
    if (module == NULL)
    {
        printf("Log module returned NULL");
        return -1;
    }
    long long result = a * b;

    /* Using a swtich-statement to speed things up*/
    if (result > CALCULATION_MAXIMUM)
    {
        event_occured(module, "Calculation exceeds threshold\n");
    }
    else if (result < CALCULATION_MINIMUM)
    {
        event_occured(module, "Calculation falls below threshold\n");
    }
    else
    {
        printf("Result is within both thresholds (result: %lld)\n", result);
    }
    return result;
}