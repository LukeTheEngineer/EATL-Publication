#ifdef __linux__
long get_cpu_info(void);

long get_program_size(void);
#endif
#ifdef _WIN32
long get_cpu_info(void);

long get_program_size(const char *filename);
#endif