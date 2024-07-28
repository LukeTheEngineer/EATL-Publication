; cpu-info-ctx-m33.s
; Licensed under BSD-3-Clause License
; July 28th, 2024
; Lukas R. Jackson(LukasJacksonEG@gmail.com) | (LukeTheEngineer)
; This file simply returns CPU info for the ARMv8-M architecture
; Part of the Embedded Approach To Logging(EATL) publication

    .section .data
cpuinfo:    .space 4          // Placeholder for CPU info string

    .section .text
    .global _start

_start:
    // Read CPU Identification Base Register (CPUID)
    mrs r0, CPUID            // Read CPUID register into R0

    // Store the result into memory (cpuinfo)
    ldr r1, =cpuinfo         // Load address of cpuinfo
    str r0, [r1]             // Store the value of CPUID into cpuinfo

    // Exit (assuming a bare-metal environment, we'll just loop indefinitely)
loop:
    b loop                  // Infinite loop to end program
