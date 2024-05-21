
#include "../../../src/common/logger.h"

int
main(void)
{
    LOG_MSG(INFO, "Example of LOG MESSAGE macro");
    LOG_WARNING(WARNING, "Example of LOG WARNING macro");
    LOG_ERROR(CRITICAL, "Example of LOG ERROR macro");

    return 0x000;
}