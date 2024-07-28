; cpu-info-x86.s
; Licensed under BSD-3-Clause License
; July 28th, 2024
; Lukas R. Jackson(LukasJacksonEG@gmail.com) | (LukeTheEngineer)
; This file simply returns CPU info for x86 CPUs
; Part of the Embedded Approach To Logging(EATL) publication


section .data
    vendor db 'xxxxxxxxxxxxxxxx', 0  ; Placeholder for CPU vendor string
    brand db 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx', 0 ; Placeholder for CPU brand string

section .bss
    ; Reserve space for results
    resb 48

section .text
global _start

_start:
    ; Get vendor string
    xor eax, eax        ; Set EAX to 0
    cpuid               ; Execute CPUID instruction

    mov [vendor], ebx   ; Copy EBX to vendor string
    mov [vendor+4], edx ; Copy EDX to vendor string
    mov [vendor+8], ecx ; Copy ECX to vendor string

    ; Get brand string (example using function 0x80000002, 0x80000003, 0x80000004)
    mov eax, 0x80000002
    cpuid
    mov [brand], eax
    mov [brand+4], ebx
    mov [brand+8], ecx
    mov [brand+12], edx

    mov eax, 0x80000003
    cpuid
    mov [brand+16], eax
    mov [brand+20], ebx
    mov [brand+24], ecx
    mov [brand+28], edx

    mov eax, 0x80000004
    cpuid
    mov [brand+32], eax
    mov [brand+36], ebx
    mov [brand+40], ecx
    mov [brand+44], edx

    ; Exit the program
    mov eax, 1          ; Exit system call number
    xor ebx, ebx        ; Status 0
    int 0x80            ; Invoke system call
