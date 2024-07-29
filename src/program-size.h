/**
 * @file program-size.h
 * @brief Getting the size of a program in C.
 *
 * @date July 27th, 2024
 *
 * @copyright Copyright (c) 2024 Lukas R. Jackson
 *
 * @author Lukas R. Jackson (LukasJacksonEG@gmail.com) | (LukeTheEngineer)
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
#ifndef program_size_h
#define program_size_h

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>

/**
 * @brief Counts the total size of the DOS header.
 * 
 * Within portable executables(PE), the DOS header is for ensuring backwards compatibility
 * within DOS systems. The presence of this header allows the executable to be identified as a valid
 * DOS program.
 * 
 * The DOS header contains a signature e_magic(MZ) or 0x5A4D and an offset(e_lfanew) to the PE header. 
 * 
 * @param[in] FILE File to read from
 * @param[in] IMAGE_DOS_HEADER DOS header structure 
 * 
 * @return int 
 */
int _READ_DOS_HEADER(FILE *, IMAGE_DOS_HEADER *);

/**
 * @brief Counts the total size of the PE header.
 * 
 * The PE header contains the signature PE\0\0 or 0x00004550, indicating that the following data 
 * conforms to the PE format. The signature allows the operating systems(OS) to recognize the file as a valid
 * portable executable(PE). 
 * 
 * @param[in] FILE File to read from 
 * @param[in] DWORD peSignature
 * @param[in] long peOffset
 * 
 * @return int 
 */
int _READ_PE_HEADER(FILE *, DWORD *, long);

/**
 * @brief Counts the total size of the COFF file header
 * 
 * The COFF file header provides information such as the target machine architecture, number of sections in the file,
 * and flags describing the file(characteristics). This is especially important since it tells the host operating system
 * how to handle the file and contents. 
 * 
 * @param[in] FILE File to read from
 * @param[in] IMAGE_FILE_HEADER FILE header structure
 * 
 * @return int 
 */
int _READ_FILE_HEADER(FILE *, IMAGE_FILE_HEADER *);

/**
 * @brief Counts the total size of the optional(OPT) header 
 * 
 * Contains essential data for loading and running the executable
 * Despite its name, this header is mandatory for executables. 
 * Inside, it contains:
 * 
 * - Magic: Identifies the file type (e.g., PE32 for 32-bit, PE32+ for 64-bit).
 * - SizeOfCode: The size of the code (text) section(s), in bytes.
 * - SizeOfInitializedData: The size of the initialized data section(s), in bytes.
 * - AddressOfEntryPoint: The address where execution starts. This specifies the relative virtual address (RVA) of the entry point for the executable.
 * - ImageBase: The preferred load address in memory. Indicates the preferred base address of the image when loaded into memory.
 * - SectionAlignment: The alignment (in bytes) of sections when loaded into memory. This value must be greater than or equal to FileAlignment.
 * - FileAlignment: The alignment (in bytes) of sections within the PE file on disk.
 * - SizeOfImage: The total size of the image in memory, including all headers, sections, and padding.
 * - Subsystem: The required subsystem to run the executable, such as the Windows GUI or Windows CUI (console). Common values include IMAGE_SUBSYSTEM_WINDOWS_GUI and IMAGE_SUBSYSTEM_WINDOWS_CUI.
 * - NumberOfRvaAndSizes: The number of entries in the Data Directory. Each entry points to a specific table or structure used by the executable.
 * 
 * @param[in] FILE File to read from
 * @param[in] IMAGE_OPTIONAL_HEADER OPT header structure
 * 
 * @return int 
 */
int _READ_OPT_HEADER(FILE *, IMAGE_OPTIONAL_HEADER *);

/**
 * @brief Counts the total size of the section header
 * 
 * Each section header includes 
 * Name: The section name(.text, .data, .rodata, etc.)
 * VirtualSize and VirtualAddress: Size and address of each section in memory
 * SizeOfRawData and PointerToRawData: Size and file offset of the section's data
 * Characteristics: Flags indicating the section's attributes (executable, writable, etc.)
 * 
 * @param[in] FILE File to read from
 * @param[in] IMAGE_SECTION_HEADER section header structure
 * @param[in] NUMBER_OF_SECTIONS total number of sections
 * 
 * @return int 
 */
int _READ_SECTION_HEADERS(FILE *, IMAGE_SECTION_HEADER *, int);

/**
 * @brief Calculates the total size of all headers within a PE file
 * 
 * Takes the size of each header, adds it up, and returns it.
 * 
 * @param[in] IMAGE_SECTION_HEADER Section header structure
 * @param[in] NUMBER_OF_SECTIONS total number of sections
 * 
 * @return size_t 
 */
size_t calculate_total_header_size(const IMAGE_SECTION_HEADER *, int);

#endif /* End of _WIN32 MACRO */

#ifdef __linux__

#include <elf.h>

/**
 * @brief Reads and calculates the total size of the executable linkable format(ELF) file
 * 
 * @param[in] FILE File to read from 
 * @param[in] Elf32_Ehdr ELF header structure
 * 
 * @return int
 */
int _READ_ELF_HEADER(FILE *, Elf32_Ehdr *);

/**
 * @brief Reads and calculates section headers specific to the Linux format ELF
 * 
 * @param[in] FILE File to read from
 * @param[in] Elf32_Shdr ELF header structure
 * @param[in] int Section header count
 * @param[in] int Section header offset
 * 
 * @return int
 */
int _READ_SECTION_HEADERS_LINUX(FILE *, Elf32_Shdr *, int, int);

/**
 * @brief Calculates the total header size 
 * 
 * @param[in] Elf32_Shdr ELF header structure 
 * @param[in] int Section header count
 * 
 * @return int
 */
size_t calculate_total_header_size(const Elf32_Shdr *, int);

#endif /* End of __linux__ MACRO */

#endif /* program_size_h */