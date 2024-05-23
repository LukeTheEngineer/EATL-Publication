#include "../../../src/common/logger.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif
#ifdef __linux__
#include <sys/resource.h>
#endif

#define MODULE_NAME "EATL-PUB"

void call_custom_callback(const char *message);

int main(void)
{
    double double_data = 55.00;

    struct log_module module =
        {
            .module_name = MODULE_NAME,
            .callback = call_custom_callback,
        };

    union log_data flight_data =
        {
            .double_data = double_data,
        };

    struct log_message message =
        {
            .message = "This is a log message\n",
            .data = flight_data,
        };

#ifdef _WIN32
    enable_virtual_terminal_processing();
    return_windows_memory_usage();
#endif
#ifdef __linux__
    return_linux_memory_usage();
#endif

    int a = 250000;
    int b = 500000;

    perform_calculation(&module, a, b); /* This very obviously exceeds the maximum threshold */

    a = 1;
    b = 11;

    perform_calculation(&module, a, b); /* This should be within the two thresholds */

    a = 0;
    b = 1;

    perform_calculation(&module, a, b); /* This should fall below the minimum threshold */

    return 0x000;
}

void call_custom_callback(const char *message)
{
    printf("Callback message: %s\n",message);
}