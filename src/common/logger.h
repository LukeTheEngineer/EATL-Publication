/**
 * @file logger.h
 * @brief Logger declarations
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
#ifndef logger_h_
#define logger_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

void enable_virtual_terminal_processing(void);

void return_windows_memory_usage(void);

#else /* If windows is not found */

#endif

#ifdef __linux__

void return_linux_memory_usage(void);

#endif

// Define color escape codes for log output

#define MAX_LOG_MESSAGE_LENGTH 256
#define MAX_MODULE_NAME_LENGTH 50

enum calc_limit {
    CALCULATION_MAXIMUM = 100, /* If a calculation is over a certain amount, trigger an event */
    CALCULATION_MINIMUM = 10   /* If a calculation is below this amount, trigger an event */
};

#define BBLK "\x1B[1;30m"
#define BRED "\x1B[1;31m"
#define BGRN "\x1B[1;32m"
#define BYEL "\x1B[1;33m"
#define BBLU "\x1B[1;34m"
#define BMAG "\x1B[1;35m"
#define BCYN "\x1B[1;36m"
#define BWHT "\x1B[1;37m" // Bold white text
#define RESET_TEXT "\x1B[0m"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

// Define log labels and messages
static const char *INFO = BBLU "MESSAGE" RESET_TEXT;
static const char *WARNING = BYEL "WARNING" RESET_TEXT;
static const char *CRITICAL = BRED "CRITICAL" RESET_TEXT;

static const char *ERROR_MSG = BRED "LOG" RESET_TEXT;
static const char *WARNING_MSG = BYEL "LOG" RESET_TEXT;
static const char *INFO_MSG = BBLU "LOG" RESET_TEXT;

#pragma GCC diagnostic pop

// Define log macros
#define LOG_MSG(label, message) \
    printf("%s %s: " BWHT "%s" RESET_TEXT "\n", label, INFO_MSG, message)
#define LOG_ERROR(label, message) \
    printf("%s %s: " BWHT "%s" RESET_TEXT "\n", label, ERROR_MSG, message)
#define LOG_WARNING(label, message) \
    printf("%s %s: " BWHT "%s" RESET_TEXT "\n", label, WARNING_MSG, message)

// Define packed attribute for structs
#define PACKED __attribute__((packed))

// Define a callback type for logging functions
typedef void (*logcallback)(const char *);

/**
 * @brief Holds the logger module's information
 * 
 * Two members of this struct initialize the module name and callback for specifc events.
 * This is especially needed for event-driven programming as it gives the programmer the power
 * to register their own functions with existing APIs without directly modifying the API itself. 
 * 
 * The Zephyr RTOS uses event-driven programming which calls for the register of callback functions.
 * 
 */
struct log_module
{
    const char *module_name;
    logcallback callback;
} PACKED;

/**
 * @brief Log data used to construct a log message
 * 
 * This union contains various members that allow the programmer to initialize select members 
 * depending on the type of data type needed. For example, the programmer may only require the double_data member 
 * and may only initialize it like so:
 * 
 * const double sensor_data = 55.00;
 * 
 * const log_data double_data_1 = 
 * {
 *      .double_data = sensor_data,
 * };
 * 
 */
union log_data
{
    const char *string_data;
    int int_data;
    double double_data;
}PACKED ;

/**
 * @brief Construct a very basic log message
 * 
 * This struct contains only message and data since we're not too focused on other members.
 * Timestamps are not added since headers such as time.h don't work on embedded systems.
 * Microcontrollers lack proper hardware to return the time.
 * 
 * This may be updated in the future.
 * 
 */
struct log_message
{
    const char *message;
    union log_data data;
} PACKED;

void event_occured(struct log_module *module, const char *message);

long long perform_calculation(struct log_module *module, long long a, long long b);

#endif /* logger_h_ */