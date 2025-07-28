/* 0-just error and simple info, 1 extra info, 3-for developers */
#define VERBOSE_ERROR 0
#define VERBOSE_OUTUT 1
#define VERBOSE_DEBUG 2

int get_param_verbose_level(char *args[], int argc);
