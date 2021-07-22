#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdbool.h>

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRNCPY(dst, src)                                                      \
  do {                                                                         \
    strncpy((dst), (src), sizeof(dst));                                        \
    (dst)[sizeof(dst) - 1] = 0;                                                \
  } while (0)
#define STRNCAT(dst, src)                                                      \
  do {                                                                         \
    strncat((dst), (src), sizeof(dst) - strlen(dst) - 1);                      \
    (dst)[sizeof(dst) - 1] = 0;                                                \
  } while (0)
#define BZERO_S(s) bzero(&(s), sizeof(s));

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define LOCATION __FILE__ ":" TOSTRING(__LINE__)

void strrepl(char *str, char from, char to);
void str_trim(char *str);
bool wildcmp(const char *wild, const char *string);
bool str2uppercase(char *const inOutStr);
const char *str_case_str(const char *const text, const char *const pattern);
char *rx_strtok_r(char *s, const char *delim, char **lasts);

#ifdef __cplusplus
}
#endif

#endif /* STR_UTILS_H */
