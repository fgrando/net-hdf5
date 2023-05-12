#ifndef CONFIGURATION_FILE_HEADER_INCLUDED_
#define CONFIGURATION_FILE_HEADER_INCLUDED_

#define CFG_FILE_MAX_PARAM_CHARS 20
#define CFG_FILE_MAX_LINE_CHARS  (CFG_FILE_MAX_PARAM_CHARS + 120)

typedef struct {
    int version;
    int port;
    int max_clients;
} cfg_file_t;

int cfg_file_save(char const * const filepath, cfg_file_t const * const data);
int cfg_file_load(char const * const filepath, cfg_file_t * data);
int cfg_file_to_str(cfg_file_t *data, char *buffer, int len);

#endif // CONFIGURATION_FILE_HEADER_INCLUDED_