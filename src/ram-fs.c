/**
 * @file ram-fs.c
 * @brief Random Access Memory(RAM) filesystem function definitions
 *
 * @date March 28th, 2024
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


/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Local includes */
#include "ram-fs.h"

Directory *log_cache;
Directory *root_dir;

void init_filesystem(void)
{
    if (root_dir == NULL)
    {
        root_dir = create_directory("root");
    }
    else
    {
        printk("Failed to initialize root directory\n");
        return;
    }
    if (log_cache == NULL)
    {
        log_cache = create_directory("log_cache");
    }
    else
    {
        printk("Failed to initialize log cache directory\n");
        return;
    }
    printk("File system initialized\n");
    return;
}

void deinit_filesystem(void)
{
    if (root_dir != NULL)
    {
        free(root_dir);
        root_dir = NULL;
    }
    if (log_cache == NULL)
    {
        free(log_cache);
        log_cache = NULL;
    }
    return;
}

/**
 * @brief Creates a new file.
 *
 * This function allocates memory for a new file structure and initializes it with the provided name and content.
 *
 * @param name Name of the file.
 * @param content Content of the file.
 *
 * @return Pointer to the newly created file.
 */
File *create_file(const char *name, const char *content, file_permissions permissions)
{
    File *file = (File *)crb_malloc(sizeof(File));
    if (file == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(file->name, name, MAX_FILENAME_LENGTH);

    // Copy content and insert \n character
    insert_marker(content);
    strncpy(file->content, content, MAX_FILE_CONTENT_LENGTH);
    file->content[MAX_FILE_CONTENT_LENGTH - 1] = '\0'; // Ensure null-termination
    if (strlen(content) < MAX_FILE_CONTENT_LENGTH - 1)
    {
        file->content[strlen(content)] = '\n'; // Insert newline if there's space
    }

    switch (permissions)
    {
    case AVAILABLE:
        file->permissions = AVAILABLE;
        break;
    case PROTECTED:
        file->permissions = PROTECTED;
        break;
    case RESTRICTED:
        file->permissions = RESTRICTED;
        break;
    default:
        printf("Defaulting to AVAILABLE permissions\n");
        file->permissions = AVAILABLE;
        break;
    }
    file->size = strlen(file->content);
    return file;
}

/**
 * @brief Creates a new directory.
 *
 * This function allocates memory for a new directory structure and initializes it with the provided name.
 *
 * @param name Name of the directory.
 * @return Pointer to the newly created directory.
 */
Directory *create_directory(const char *name)
{
    Directory *dir = (Directory *)crb_malloc(sizeof(Directory));
    if (dir == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(dir->name, name, MAX_FILENAME_LENGTH);
    dir->num_files = 0;
    dir->num_subdirs = 0;
    return dir;
}

/**
 * @brief Appends a file to a directory.
 *
 * This function adds a file pointer to the list of files in the specified directory. If the directory is already full,
 * an error message is printed to stderr.
 *
 * @param dir Pointer to the directory.
 * @param file Pointer to the file to be appended.
 */
void append_to_dir(Directory *dir, File *file)
{
    if (dir->num_files < MAX_FILES)
    {
        dir->files[dir->num_files++] = file;
    }
    else
    {
        fprintf(stderr, "Directory is full, cannot add file.\n");
    }
}

/**
 * @brief Lists all files in a directory.
 *
 * This function prints the names of all files contained within a directory to the standard output.
 *
 * @param root A pointer to the root directory structure.
 */
void ls_dir(Directory *root)
{
    // Print header
    printf("Files: \n");

    // Iterate through each file in the directory
    for (int i = 0; i < root->num_files; ++i)
    {
        // Print the name of the file
        printf("%s\n", root->files[i]->name);
    }
}

void write_to_file(File *file, const char *data)
{
}

int insert_marker(const char *content)
{
    size_t original_length = strlen(content);
    size_t prefix_length = strlen(MARKER);
    size_t suffix_length = strlen(MARKER);

    char *combined_string = (char *)crb_malloc(original_length + prefix_length + suffix_length + 1);

    if (combined_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return -1;
    }
    // Copy the prefix to the combined string
    strcpy(combined_string, MARKER);

    // Append the original string to the combined string
    strcat(combined_string, content);

    // Append the suffix to the combined string
    strcat(combined_string, MARKER);

    // Print the combined string
    printf("%s\n", combined_string);

    return 0U;
}

/**
 * @brief Reads from a file
 *
 * @param[in] root Pointer to the directory.
 * @param[in] file Pointer to the file to be read from
 */
void readfile(File *file)
{
    if (file == NULL)
    {
        fprintf(stderr, "File not found.\n");
        return;
    }

    printf("Contents of file %s:\n", file->name);
    printf("\n %s \n", file->content);
    return;
}

/**
 * @brief Prints the size of a file.
 *
 * @param[in] file Pointer to the file to be sized
 */
int sizeoffile(File *file)
{
    if (file == NULL)
    {
        fprintf(stderr, "File not found.\n");
        return -1;
    }
    return sizeof(file);
}

/**
 * @brief Prints the size of a directory.
 *
 * @param[in] root Pointer to the directory to be sized
 */
int sizeofdir(Directory *root)
{
    if (root == NULL)
    {
        fprintf(stderr, "Directory not found. ");
        return -1;
    }
    return sizeof(root);
}

/**
 * @brief Calculates the total memory used by the in-memory file system.
 *
 * This function recursively calculates the total memory used by the in-memory file system. It traverses through all
 * files and subdirectories, summing up the memory used by each file and directory.
 *
 * @param root Pointer to the root directory of the file system.
 * @return Total memory used by the file system in bytes.
 */
unsigned long long calculate_memory_usage(Directory *root)
{
    size_t total_memory = 0;

    // Calculate memory used by files
    for (int i = 0; i < root->num_files; i++)
    {
        total_memory += sizeof(File) + strlen(root->files[i]->name) + strlen(root->files[i]->content);
    }

    // Calculate memory used by subdirectories
    for (int i = 0; i < root->num_subdirs; i++)
    {
        total_memory += calculate_memory_usage(root->subdirs[i]);
    }

    return total_memory;
}