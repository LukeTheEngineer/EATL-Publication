/**
 * @file ram-fs.h
 * @brief Random Access Memory(RAM) filesystem function declarations
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
#ifndef ram_fs_h_
#define ram_fs_h_

#define RAM_FS __attribute__((section(".RAM-FS")))

#define PACKED __attribute__((packed))

#define MAX_FILENAME_LENGTH 100
#define MAX_FILE_CONTENT_LENGTH 1000
#define MAX_FILES 100
#define MAX_DIRS 100

#define MARKER "[*]"


/**
 * @brief Common file permissions
 */
typedef enum file_permissions
{
    AVAILABLE = 1, /**< Default permission set on a file */
    PROTECTED = 2, /**< Limited permissions but accessible */
    RESTRICTED = 3 /**< Cannot edit */
} PACKED file_permissions;

/**
 * @brief Structure to represent a file
 */
typedef struct
{
    char name[MAX_FILENAME_LENGTH];        /**< Name of the file */
    char content[MAX_FILE_CONTENT_LENGTH]; /**< Content of the file */
    int size;                              /**< Size of the file content */
    file_permissions permissions;          /**< Permissions of given file content | Default permissions of a file is AVAILABLE */
} PACKED File;

/**
 * @brief Structure to represent a directory
 */
typedef struct Directory
{
    char name[MAX_FILENAME_LENGTH];      /**< Name of the directory */
    int num_files;                       /**< Number of files in the directory */
    File *files[MAX_FILES];              /**< Array of pointers to files in the directory */
    int num_subdirs;                     /**< Number of subdirectories in the directory */
    struct Directory *subdirs[MAX_DIRS]; /**< Array of pointers to subdirectories in the directory */
} PACKED Directory;

/**
 * @brief Initializes the Random Access Memory(RAM) filesystem
 * And creates two directories log_cache and root
 * 
 */
RAM_FS void init_filesystem(void);

/**
 * @brief Deinitializes the Random Access Memory(RAM) filesystem
 * And deletes the main directories log_cache and root
 * 
 * @return int | 0 for success -1 for failure
 */
RAM_FS void deinit_filesystem(void);

/**
 * @brief Creates a new file.
 *
 * @param name Name of the file.
 * @param content Content of the file.
 * @return Pointer to the newly created file.
 */
RAM_FS File *create_file(const char *name, const char *content, file_permissions permissions);

/**
 * @brief Creates a new directory.
 *
 * @param name Name of the directory.
 * @return Pointer to the newly created directory.
 */
RAM_FS Directory *create_directory(const char *name);

/**
 * @brief Appends a file to a directory.
 *
 * @param dir Pointer to the directory.
 * @param file Pointer to the file to be appended.
 */
RAM_FS void append_to_dir(Directory *dir, File *file);

/**
 * @brief Lists all files within a given root directory
 *
 * @param[in] root Pointer to the directory.
 */
RAM_FS void ls_dir(Directory *root);

/**
 * @brief Writes to a specific file
 *
 * @param[in] file
 * @param[in] data
 */
RAM_FS void write_to_file(File *file, const char *data);

/**
 * @brief Inserts specific marker to parse content
 *
 * @param content
 * @return RAM_FS
 */
RAM_FS int insert_marker(const char *content);

/**
 * @brief Identifies the [*] marker for parsing
 * 
 * @param[in] file File to identify marker
 * @return 0 on success, -1 on failure
 */
RAM_FS int identify_marker(File *file);

/**
 * @brief Reads from a file
 *
 * @param[in] root Pointer to the directory.
 * @param[in] file Pointer to the file to be read from
 */
RAM_FS void readfile(File *file);

/**
 * @brief Prints the size of a file.
 *
 * @param[in] file Pointer to the file to be sized
 */
RAM_FS int sizeoffile(File *file);

/**
 * @brief Prints the size of a directory.
 *
 * @param[in] root Pointer to the directory to be sized
 */
RAM_FS int sizeofdir(Directory *root);

/**
 * @brief Calculates the total memory used by the in-memory file system.
 *
 * This function recursively calculates the total memory used by the in-memory file system.
 *
 * @param root Pointer to the root directory of the file system.
 * @return Total memory used by the file system in bytes.
 */
RAM_FS unsigned long long calculate_memory_usage(Directory *root);

extern Directory *log_cache;
extern Directory *root_dir;

#endif /* ram_fs_h_ */