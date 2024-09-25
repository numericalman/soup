/*
    Author: Roy Bebru (RoyBebru@Gmail.Com)
*/

#ifndef _GET_CONFIG_HPP_
# define _GET_CONFIG_HPP_

# define DEFAULT_CONFIG_FILENAME "default.conf"
# define DEFAULT_LISTEN_PORT 13331U
# define DEFAULT_ANSWER_LENGTH 1000000U
# define DEFAULT_PROTO_VERSION "1.0"

# include <cinttypes>

#define PATH_LOGFILE_PREFIX_SIZE 96

struct config
{
    uint8_t proto_ver_major;
    uint8_t proto_ver_minor;
    uint16_t listen_port;
    uint32_t answer_length;
    char path_logfile_prefix[PATH_LOGFILE_PREFIX_SIZE];
};

int get_config(config& conf, int argc, char *argv[]);
#endif // _GET_CONFIG_HPP_
