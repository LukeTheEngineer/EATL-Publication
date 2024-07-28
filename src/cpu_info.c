#include "cpu_info.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __linux__

#include<unistd.h>
#include<fcntl.h>

long get_program_size(void) {
    char buffer[64] = {0};
    int fd = open("LIN_nrf-event-driven", O_RDONLY); // Replace "filename" with the actual filename or pass it as a parameter
    if (fd == -1) {
        perror("open");
        return -1;
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1); // -1 to leave space for null terminator
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return -1;
    }

    close(fd);

    // Ensure buffer is null-terminated
    buffer[bytes_read] = '\0';

    // Convert ASCII to integer
    long program_size = atol(buffer);
    return program_size;
}

long get_cpu_info(void) {
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("fopen");
        return -1;
    }

    char buffer[1024];
    long numOfProcessors = 0;
    long numOfCores = 0;
    while (fgets(buffer, sizeof(buffer), cpuinfo)) {
        if (strncmp(buffer, "processor", 9) == 0) {
            numOfProcessors++;
        }
        if (strncmp(buffer, "cpu cores", 9) == 0) {
            numOfCores = strtol(buffer + 11, NULL, 10);
        }
    }
    fclose(cpuinfo);

    printf("Number of logical processors: %ld\n", numOfProcessors);
    printf("Number of physical cores: %ld\n", numOfCores);

    return numOfProcessors;
}

#endif 

#ifdef _WIN32
//#include <windows.h>


#endif 