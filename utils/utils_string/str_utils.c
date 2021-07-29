#include "str_utils.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

static int is_trim_char(char c);

void strrepl(char *str, char from, char to) {
  if (str == NULL) {
    return;
  }

  while (*str != 0) {
    if (*str == from) {
      *str = to;
    }
    str++;
  }
}

static int is_trim_char(char c) {
  return isspace((unsigned char)c) || c == '\n' || c == '\r';
}

void str_trim(char *str) {
  char *start = str;
  char *end = start + strlen(str) - 1;

  while (end >= start) { /* trim right */
    if (!is_trim_char(*end)) {
      break;
    }
    end--;
  }
  end++;
  *end = '\0';

  while (is_trim_char(*start)) { /* trim left */
    start++;
  }

  if (start != str) /* there is a string */ {
    memmove(str, start, end - start + 1);
  }
}
/*
 Find the first occurrence of pattern in text, ignore case.
 time complexity O(T*P)*/
const char *str_case_str(const char *const text, const char *const pattern) {
  const char *retPtr = NULL;

  if ((NULL != text) && (NULL != pattern)) {
    if (pattern[0] == '\0') {
      retPtr = &text[0];
    } else {
      int32_t i;

      for (i = 0; text[i] != '\0'; ++i) {
        int32_t j = 0;

        for (j = 0; text[i + j] != '\0'; j++) {
          int32_t symb1 = tolower((int32_t)text[i + j]);
          int32_t symb2 = tolower((int32_t)pattern[j]);

          if (symb1 != symb2) {
            break;
          }
        }

        if (pattern[j] == '\0') {
          retPtr = &text[i];
          break;
        }
      }
    }
  }

  return retPtr;
}

/*inOutStr must be in RAM not in ROM(FLASH)*/
/* exapmle - EXAPMLE*/
bool str2uppercase(char *const inOutStr) {
  bool res = false;
  if (NULL != inOutStr) {
    uint32_t index;
    uint32_t strLen = strlen(inOutStr);
    for (index = 0; index < strLen; index++) {
      if (('a' <= inOutStr[index]) && (inOutStr[index] <= 'z')) {
        inOutStr[index] = inOutStr[index] - 32u;
      }
    }
    res = true;
  }
  return res;
}

/*
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char *rx_strtok_r(char *str, const char *delim, char **nextp) {
  char *ret;

  if (str == NULL) {
    str = *nextp;
  }

  str += strspn(str, delim);

  if (*str == '\0') {
    return NULL;
  }

  ret = str;

  str += strcspn(str, delim);

  if (*str) {
    *str = '\0';
    str++;
  }

  *nextp = str;

  return ret;
}

bool wildcmp(const char *wild, const char *string) {
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((toupper(*wild) != toupper(*string)) && (*wild != '?')) {
      return false;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      ++wild;
      if (!*wild) {
        return true;
      }
      mp = wild;
      cp = string + 1;
    } else if ((toupper(*wild) == toupper(*string)) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp;
      cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

int replace_char(char *in_out_str, char orig, char rep) {
  char *ix = in_out_str;
  int n = 0;
  while ((ix = strchr(ix, orig)) != NULL) {
    *ix++ = rep;
    n++;
  }
  return n;
}

/* Compare S1 and S2, ignoring case, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int __strcasecmp(const char* s1, const char* s2) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    int result;
    if(p1 == p2)
        return 0;
    while((result = tolower(*p1) - tolower(*p2++)) == 0)
        if(*p1++ == '\0')
            break;
    return result;
}
