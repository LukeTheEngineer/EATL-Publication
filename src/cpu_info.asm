%ifdef __linux__

section .data
    filename db "/proc/self/statm", 0
    buffer   times 64 db 0       ; buffer to hold the content of the file
    len      equ $ - buffer

section .bss
    bytes_read resb 4            ; variable to store the number of bytes read

get_cpu_info:
    ; Placeholder for actual CPU info retrieval
    ; This can be implemented using /proc/cpuinfo or CPUID instruction
    ret

get_program_size:
    ; Open the /proc/self/statm file
    mov eax, 2                   ; sys_open
    mov rdi, filename            ; filename
    mov rsi, 0                   ; O_RDONLY
    syscall

    ; Save file descriptor
    mov rdi, rax

    ; Read the file
    mov eax, 0                   ; sys_read
    mov rsi, buffer              ; buffer
    mov rdx, len                 ; buffer length
    syscall

    ; Close the file
    mov eax, 3                   ; sys_close
    syscall

    ; Extract memory usage information
    ; The first value in buffer is the total program size
    mov rsi, buffer              ; point to the start of the buffer
    call atoi                    ; convert ASCII to integer

    ; Store the result
    mov [bytes_read], eax

    ret

atoi:
    ; Convert ASCII string to integer
    xor eax, eax                 ; clear eax (result)
    xor ecx, ecx                 ; clear ecx (iterator)

atoi_loop:
    movzx edx, byte [rsi + ecx]  ; load byte from buffer
    test edx, edx                ; check if null terminator
    jz atoi_end                  ; if zero, end of string
    sub edx, '0'                 ; convert ASCII to integer
    imul eax, eax, 10            ; multiply result by 10
    add eax, edx                 ; add current digit
    inc ecx                      ; move to the next character
    jmp atoi_loop

atoi_end:
    ret

%endif

%ifdef _WIN32

.data
    ProcessMemoryInfo PROCESS_MEMORY_COUNTERS <>
    SystemInfo SYSTEM_INFO <>
    msg db "Process Memory Info Retrieved", 0
    cpuInfoMsg db "CPU Info Retrieved", 0

.data?
    hProcess dd ?

get_cpu_info PROC
    ; Get CPU information
    INVOKE GetSystemInfo, ADDR SystemInfo

    ; Display CPU information message
    INVOKE MessageBoxA, NULL, ADDR cpuInfoMsg, ADDR cpuInfoMsg, MB_OK

    ret
get_cpu_info ENDP

get_program_size PROC
    ; Get current process handle
    INVOKE GetCurrentProcess
    mov hProcess, eax

    ; Call GetProcessMemoryInfo
    INVOKE GetProcessMemoryInfo, hProcess, ADDR ProcessMemoryInfo, SIZEOF ProcessMemoryInfo

    ; ProcessMemoryInfo.WorkingSetSize has the memory usage in bytes
    ; Display memory information message
    INVOKE MessageBoxA, NULL, ADDR msg, ADDR msg, MB_OK

    ret
get_program_size ENDP

END main

%endif
