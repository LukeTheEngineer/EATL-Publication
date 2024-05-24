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
#include <windows.h>

long get_program_size(const char *filename) {
    HANDLE hFile;
    LARGE_INTEGER fileSize;

    // Open the file
    hFile = CreateFile(filename,               // file name
                       GENERIC_READ,           // open for reading
                       FILE_SHARE_READ,        // share for reading
                       NULL,                   // default security
                       OPEN_EXISTING,          // existing file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Could not open file %s (error %lu)\n", filename, GetLastError());
        return -1;
    }

    // Get the file size
    if (!GetFileSizeEx(hFile, &fileSize)) {
        fprintf(stderr, "Could not get file size (error %lu)\n", GetLastError());
        CloseHandle(hFile);
        return -1;
    }

    // Close the file handle
    CloseHandle(hFile);

    return (long)fileSize.QuadPart;
}

long get_cpu_info(void) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    DWORD bufferSize = 0;
    GetLogicalProcessorInformation(NULL, &bufferSize);
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION *buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
    GetLogicalProcessorInformation(buffer, &bufferSize);

    int numOfProcessors = 0;
    for (DWORD i = 0; i < bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
        if (buffer[i].Relationship == RelationProcessorCore) {
            numOfProcessors++;
        }
    }
    free(buffer);

    printf("Number of logical processors: %lu\n", sysInfo.dwNumberOfProcessors);
    printf("Number of physical cores: %d\n", numOfProcessors);

    return sysInfo.dwNumberOfProcessors;
}

#endif 