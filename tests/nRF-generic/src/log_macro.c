
#include "../../../src/common/logger.h"
#ifdef _WIN32
#include<windows.h>
#endif

int
main(void)
{
#ifdef _WIN32
    enable_virtual_terminal_processing();
#endif
    LOG_MSG(INFO, "Example of LOG MESSAGE macro");
    LOG_WARNING(WARNING, "Example of LOG WARNING macro");
    LOG_ERROR(CRITICAL, "Example of LOG ERROR macro");

    return 0x000;
}