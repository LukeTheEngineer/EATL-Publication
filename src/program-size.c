/**
 * @file program-size.c
 * @brief Getting the size of a program using C++.
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

#include "program-size.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

#endif /* End of _WIN32 MACRO */

#ifdef __linux__

#include <elf.h>

#endif /* End of _WIN32 MACRO */

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

#ifdef _WIN32

    IMAGE_DOS_HEADER dosHeader;
    if (!_READ_DOS_HEADER(file, &dosHeader))
    {
        fprintf(stderr, "Not a valid PE file: %s\n", filename);
        fclose(file);
        return 1;
    }

    DWORD peSignature;
    if (!_READ_PE_HEADER(file, &peSignature, dosHeader.e_lfanew))
    {
        fprintf(stderr, "Not a valid PE file: %s\n", filename);
        fclose(file);
        return 1;
    }

    IMAGE_FILE_HEADER fileHeader;
    if (!_READ_FILE_HEADER(file, &fileHeader))
    {
        fprintf(stderr, "Error reading file header\n");
        fclose(file);
        return 1;
    }

    IMAGE_OPTIONAL_HEADER optionalHeader;
    if (!_READ_OPT_HEADER(file, &optionalHeader))
    {
        fprintf(stderr, "Error reading optional header\n");
        fclose(file);
        return 1;
    }

    IMAGE_SECTION_HEADER *sectionHeaders = malloc(fileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
    if (!sectionHeaders)
    {
        fprintf(stderr, "Memory allocation failure\n");
        fclose(file);
        return 1;
    }

    if (!_READ_SECTION_HEADERS(file, sectionHeaders, fileHeader.NumberOfSections))
    {
        fprintf(stderr, "Error reading section headers\n");
        free(sectionHeaders);
        fclose(file);
        return 1;
    }

    size_t totalSize = calculate_total_header_size(sectionHeaders, fileHeader.NumberOfSections);

    free(sectionHeaders);
    fclose(file);

    printf("Size of the executable: %zu bytes\n", totalSize);
#endif

#ifdef __linux__

    Elf32_Ehdr elfHeader;
    if (!readELFHeader(file, &elfHeader))
    {
        fprintf(stderr, "Not a valid ELF file: %s\n", filename);
        fclose(file);
        return 1;
    }

    Elf32_Shdr *sectionHeaders = malloc(elfHeader.e_shnum * sizeof(Elf32_Shdr));
    if (!sectionHeaders)
    {
        fprintf(stderr, "Memory allocation failure\n");
        fclose(file);
        return 1;
    }

    if (!readSectionHeaders(file, sectionHeaders, elfHeader.e_shnum, elfHeader.e_shoff))
    {
        fprintf(stderr, "Error reading section headers\n");
        free(sectionHeaders);
        fclose(file);
        return 1;
    }

    size_t totalSize = calculateTotalSectionSize(sectionHeaders, elfHeader.e_shnum);

    free(sectionHeaders);
    fclose(file);

    printf("Size of the executable: %zu bytes\n", totalSize);

#endif

    return 0x00;
}

#ifdef _WIN32

// Function implementations
uint8_t _READ_DOS_HEADER(FILE *file, IMAGE_DOS_HEADER *dosHeader)
{
    return fread(dosHeader, sizeof(IMAGE_DOS_HEADER), 1, file) == 1 &&
           dosHeader->e_magic == IMAGE_DOS_SIGNATURE;
}

uint8_t _READ_PE_HEADER(FILE *file, DWORD *peSignature, long peOffset)
{
    if (fseek(file, peOffset, SEEK_SET) != 0)
    {
        return 0;
    }
    return fread(peSignature, sizeof(DWORD), 1, file) == 1 &&
           *peSignature == IMAGE_NT_SIGNATURE;
}

uint8_t _READ_FILE_HEADER(FILE *file, IMAGE_FILE_HEADER *fileHeader)
{
    return fread(fileHeader, sizeof(IMAGE_FILE_HEADER), 1, file) == 1;
}

uint8_t _READ_OPT_HEADER(FILE *file, IMAGE_OPTIONAL_HEADER *optionalHeader)
{
    return fread(optionalHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file) == 1;
}

uint8_t _READ_SECTION_HEADERS(FILE *file, IMAGE_SECTION_HEADER *sectionHeaders, int numberOfSections)
{
    return fread(sectionHeaders, sizeof(IMAGE_SECTION_HEADER), numberOfSections, file) == numberOfSections;
}

size_t calculate_total_header_size(const IMAGE_SECTION_HEADER *sectionHeaders, int numberOfSections)
{
    size_t totalSize = 0;
    for (int i = 0; i < numberOfSections; i++)
    {
        totalSize += sectionHeaders[i].SizeOfRawData;
    }
    return totalSize;
}

#endif

#ifdef __linux__

#include <elf.h>

int readELFHeader(FILE *file, Elf32_Ehdr *elfHeader)
{
    if (fread(elfHeader, sizeof(Elf32_Ehdr), 1, file) != 1)
    {
        return 0;
    }
    return memcmp(elfHeader->e_ident, ELFMAG, SELFMAG) == 0;
}

int readSectionHeaders(FILE *file, Elf32_Shdr *sectionHeaders, int sectionHeaderCount, int sectionHeaderOffset)
{
    if (fseek(file, sectionHeaderOffset, SEEK_SET) != 0)
    {
        return 0;
    }
    return fread(sectionHeaders, sizeof(Elf32_Shdr), sectionHeaderCount, file) == sectionHeaderCount;
}

size_t calculateTotalSectionSize(const Elf32_Shdr *sectionHeaders, int sectionHeaderCount)
{
    size_t totalSize = 0;
    for (int i = 0; i < sectionHeaderCount; i++)
    {
        totalSize += sectionHeaders[i].sh_size;
    }
    return totalSize;
}

#endif