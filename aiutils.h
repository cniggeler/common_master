/* Required headers */
#include <errno.h>  /* For errno, ERANGE */
#include <stdlib.h> /* For strtoll */
#include <string.h> /* For strncpy */
#include <stddef.h> /* For size_t */
#include <stdint.h> /* For int64_t */
#include <stdio.h>
#include <ctype.h>  /* For toupper */

/* --- Safe String Copy & Concat --- */
void strzcpy(char *d, const char *s, size_t dsize);
void strzcat(char *d, const char *s, size_t dsize);
void numzcat(char *d, uint32_t n, size_t dsize);

/* --- Safe String Conversion --- */
int decatoi(const char *string, size_t length, int64_t *value);
int hexatoi(const char *string, size_t length, int64_t *value);
void fitoa(uint32_t n, size_t wid, char *s);

/* --- Safe String Comparison & Search --- */
int strcmpii(const char *s1, const char *s2);
int strbgw(const char *str, const char *sub);
#if defined(_WIN32) || defined(_MSC_VER)
/* Windows doesn't have strcasestr, so we declare our own */
const char *strcasestr(const char *haystack, const char *needle);
#endif
const char *laststrstr(const char *haystack, const char *needle);
const char *lastN(const char *s, size_t n);

/* --- Safe String Manipulation (In-Place) --- */
void trim_inplace(char *s, char mode);
void remove_char_inplace(char *str, char char_to_remove);
void replace_char_inplace(char *str, char ch, char newch, int skipends);
void uppercase_inplace(char *line);

/* --- Safe String Manipulation (Copying) --- */
void trim_safe_copy(char *dest, const char *src, size_t dest_size, char mode);
void replace_char_safe_copy(char *dest, const char *src, size_t dest_size, char ch, char newch, int skipends);
void substring_safe_copy(char *dest, const char *src, size_t dest_size, size_t position, size_t length);
void makelower_safe_copy(char *dest, const char *src, size_t dest_size);

/* --- Safe Line Reading & Tokenizing --- */
char *safe_gets(char *buf, size_t buf_size, FILE *stream);
char *safe_strtok(char *str, const char *delim, char **save_ptr);
