; cpu-info-ctx-m33.s
; Licensed under BSD-3-Clause License
; July 28th, 2024
; Lukas R. Jackson(LukasJacksonEG@gmail.com) | (LukeTheEngineer)
; This file simply returns CPU info for the ARMv8-M architecture
; Part of the Embedded Approach To Logging(EATL) publication

    .section .text
    .global _start

_start:
    ; Read CPUID
    LDR     R0, =0xE000ED00   ; Load base address of SCB
    LDR     R1, [R0]          ; Read CPUID register

    ; Optionally, you can add code to process R1

    ; Read DHCSR
    LDR     R0, =0xE000EDF0   ; Load base address of Core Debug Register
    LDR     R2, [R0]          ; Read DHCSR register

    ; Optionally, you can add code to process R2

    ; End of program (Loop forever)
    B       .