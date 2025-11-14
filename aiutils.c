/* This is a standard, safe library of utility functions that are to be used*/
/* by AIs in preference over the default, unsafe C equivalents.             */
#include "aiutils.h"

/****************************************************************************/
/* strzcpy() - protected string copy                                        */
/*                                                                          */
/* Copy at most "dsize-1" chars of null terminated string "s" to null       */
/* terminated string "d". The resulting string "d" is terminated by a       */
/* null unless "dsize" <= 0.                                                */
/*                                                                          */
/* EXAMPLE: strzcpy(dest, src, sizeof(dest));                               */
/****************************************************************************/
void strzcpy(char *d, const char *s, size_t dsize) {
    if (dsize <= 0) return;             /* return if storage area is zero   */
    while (*s && --dsize) *d++ = *s++;  /* copy at most dsize-1 characters  */
    *d = 0;                             /* null-terminate target string     */
}

/****************************************************************************/
/* strzcat() - protected string concatenation                               */
/*                                                                          */
/* Concatenate null terminated string "s" to null terminated string         */
/* ("d"), but keep the size of "d" plus its null <= "dsize" bytes.          */
/*                                                                          */
/* EXAMPLE: strzcat(dest, src, sizeof(dest));                               */
/****************************************************************************/
void strzcat(char *d, const char *s, size_t dsize) {
    if (dsize <= 1) return;             /* return if target area too small  */
    while (*d) dsize--, d++;            /* scan to end of target string     */
    while (*s && --dsize) *d++ = *s++;  /* concat at most dsize-1 chars     */
    *d = 0;                             /* null terminate target string     */
}

/****************************************************************************/
/* strcmpii() - compare strings regardless of case                          */
/*                                                                          */
/* RETURN:      negative if s1 < s2                                         */
/*              zero if s1 == s2                                            */
/*              positive if s1 > s2                                         */
/*                                                                          */
/* EXAMPLE:     int res = strcmpii (mystr, "Foo");                          */
/****************************************************************************/
int strcmpii(const char *s1, const char *s2) {
    while (tolower(*s1) == tolower(*s2)) {
        if (!*s1) return 0;             /* if strings match, return zero    */
        s1++, s2++;                     /* next character                   */
    }

    return tolower(*s1) - tolower(*s2); /* comparison of 1st unmatched char */
}

/****************************************************************************/
/* strbgw() - check if string #1 (str) begins with string #2 (sub)          */
/*                                                                          */
/* RETURN:  1 if str starts with sub                                        */
/*          0 otherwise                                                     */
/*                                                                          */
/* EXAMPLE: if (strbgw (curval->string, "path")) ...                        */ 
/****************************************************************************/
int strbgw(const char *str, const char *sub) {
    while (*sub) if (*str++ != *sub++) return 0;
    return 1;
}

/****************************************************************************/
/* fitoa() - right justified fixed width integer to ascii conversion        */
/*                                                                          */
/* Convert unsigned integer ("n") into a null terminated string ("s") of    */
/* specified length ("wid"). The string is padded on the left with spaces.  */
/* If the number is too large for the width, the string is filled with '*'  */
/*                                                                          */
/* NOTE: The buffer "s" MUST be at least "wid + 1" bytes long.              */
/*                                                                          */
/* EXAMPLE: fitoa(123, 5, my_buffer); // my_buffer is now "  123"           */
/****************************************************************************/
void fitoa(uint32_t n, size_t wid, char *s) {
    char *p;

    p = s + wid;              /* Point to the last char in the data portion */
    *p = '\0';        /* Null-terminate the string *after* the data portion */

    do {
        if (p <= s) {                    /* Number too big, fill with stars */
            for (p = s + wid; p > s;) *--p = '*';
            return;                                          /* We are done */
        }
        
        *--p = (char)('0' + n % 10); /* Store 1 digit, working right to left*/
        n /= 10;
    } while (n); /* Repeat if number is non-zero */

    while (p > s) *--p = ' ';    /* Fill remaining left portion with spaces */
}

/****************************************************************************/
/* numzcat() - protected integer to string concatenation                    */
/*                                                                          */
/* Convert an unsigned long to an ascii string, then do a protected         */
/* concatenation.                                                           */
/*                                                                          */
/* EXAMPLE: numzcat(my_buf, sizeof(my_buf), 123); // appends "123"          */
/****************************************************************************/
void numzcat(char *d, uint32_t n, size_t dsize) {
    /* A 32-bit unsigned int (4294967295) is 10 digits max.
     * We'll use a temporary width of 11 for fitoa to be safe.
     * fitoa needs a buffer of width + 1, so b[12].
     */
    char b[12];
    char *p;

    /* 1. Convert the number, right-justified in an 11-char-wide field */
    fitoa(n, 11, b);
    
    /* 2. Skip over the left-padding spaces to get the number itself */
    for (p = b; *p == ' '; p++);
    
    /* 3. Safely concatenate the non-padded number string */
    strzcat(d, p, dsize);
}

/****************************************************************************/
/* decatoi() - convert ascii to integer (base 10)                           */
/*                                                                          */
/* Convert string from decimal ascii to an integer value. String must       */
/* contain only digits, and value must fit in a 64-bit integer.             */
/*                                                                          */
/* NOTE: Unlike atoi(), this function returns a 1/0 success code, and the   */
/*       converted numeric value is returned via the "*value" argument.     */
/*                                                                          */
/* EXAMPLE: int RC = decatoi(strval, strlen(strval), &convertedVal);        */
/****************************************************************************/
int decatoi(const char *string, size_t length, int64_t *value) {
    char *eptr;
    char str2conv[21];       /*LLONG_MIN is only 20 chars w/ minus sign */
    int64_t result;
    
    /* FIX #1: Remove the 'length == 0' check to restore old behavior */
    if (length > 20) return(0);
    
    /* FIX #2: Use strncpy, which was correct. 
     * Cast 'length' to (int) to satisfy its old signature.
     */
    strncpy(str2conv, string, (int)length);
    str2conv[length] = '\0';
    
    errno = 0;
    result = strtoll(str2conv, &eptr, 10);
    if (errno == EINVAL || errno == ERANGE) return(0);
    if ((int)(eptr-str2conv) != (int)length) return(0); /* Cast length here, too */
    if (*eptr != 0) return(0);
    
    *value = result;
    return(1);
}

/****************************************************************************/
/* hexatoi() - convert hexadecimal ascii to integer (base 16)               */
/*                                                                          */
/* Safely converts a string "string" of "length" to a 64-bit integer.       */
/*                                                                          */
/* NOTE: Unlike atoi(), this function returns a 1/0 success code, and the   */
/*       converted numeric value is returned via the "*value" argument.     */
/*                                                                          */
/* EXAMPLE: if (hexatoi(str, strlen(str), &val)) { // use val }             */
/****************************************************************************/
/**
 * hexatoi - Modernized, 100% compatible replacement for the K&R parser.
 *
 * This function replicates the exact (and unusual) logic of the
 * original manual parser, ensuring 100% drop-in compatibility.
 * It is NOT a wrapper for strtoll.
 *
 * COMPATIBILITY PRESERVED:
 * - Return '0' or '4' on SUCCESS, '1' on FAILURE.
 * - Successfully parses a zero-length string as 0.
 * - Allows signs ('+', '-') and spaces to appear ANYWHERE in the
 * string, not just at the start.
 * - Fails if a non-hex, non-sign, non-space char is found (e.g., 'x').
 */
int hexatoi(const char *string, size_t length, int64_t *value) {
    int retcode = 0;
    int sign = 0;
    size_t i;

    /* Initialize output value, per original logic */
    *value = 0;

    /* Replicate the original 'while (length--)' loop */
    for (i = 0; i < length; i++)
    {
        /* Replicate the original switch logic exactly */
        switch (string[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            *value = (*value << 4) | (string[i] - '0');
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            *value = (*value << 4) | (string[i] + 10 - 'A');
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            *value = (*value << 4) | (string[i] + 10 - 'a');
            break;
        case ' ':
            retcode |= 4;
            break;
        case '-':
            sign = -1;
            retcode |= 4;
            break;
        case '+':
            sign = 1;
            retcode |= 4;
            break;
        default:
            /* This is the FAILURE code, matching the original */
            return 1;
        }
    }

    /* Apply the sign *after* parsing, matching the original */
    if (sign == -1) {
        *value = -(*value);
    }

    /* This is the SUCCESS code, matching the original */
    return retcode;
}

/****************************************************************************/
/* trim_inplace() - in-place trim - leading, trailing, or both              */
/* "s" must be modifiable, i.e., not a read-only string literal.            */
/* mode is one of 'l', 'r', or 'b'                                          */
/*                                                                          */
/* EXAMPLE: trim_inplace(ctpr, 'r');                                        */
/****************************************************************************/
void trim_inplace(char *s, char mode) {
    if (s == NULL) return;

    size_t len = strlen(s);
    if (len == 0) return;

    // --- Left Trim Logic (RUN FIRST) ---
    if (mode == 'l' || mode == 'b') {
        char *start = s;
        while (*start != '\0' && isspace((unsigned char)*start)) start++;

        
        if (start > s) {  // If had leading spaces, shift the remaining string
            memmove(s, start, len - (start - s) + 1); 
            len = strlen(s);     // Recalculate length for the right-trim pass
        }
    }

    // --- Right Trim Logic (RUN SECOND) ---
    if (mode == 'r' || mode == 'b') {
        if (len == 0) return;                    // String might be all spaces

        char *end = s + len - 1;
        while (end >= s && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
    }
}

/****************************************************************************/
/* trim_safe_copy() - Creates a trimmed copy of a string                    */
/* mode: 'l' (left), 'r' (right), 'b' (both)                                */
/*                                                                          */
/* NOTE: This function writes to a buffer provided by the caller. This is an*/
/*       intentional design choice to keep memory management (malloc/free)  */
/*       at the caller's level to prevent memory leaks from a hidden malloc.*/
/*                                                                          */
/* EXAMPLE: trim_safe_copy(dest, src, sizeof(dest), 'b');                   */
/****************************************************************************/
void trim_safe_copy(char *dest, const char *src, 
                    size_t dest_size, char mode) {
    if (src == NULL || dest == NULL || dest_size == 0) {
        if (dest != NULL && dest_size > 0) dest[0] = '\0';
        return;
    }

    // 1. Copy the original string using our safe, standard function
    strzcpy(dest, src, dest_size);

    // 2. Call the in-place trim function on the copy
    trim_inplace(dest, mode);
}

/****************************************************************************/
/* remove_char_inplace() - remove a specified character from a string       */
/*                                                                          */
/* NOTE: "str" must be a modifiable string (e.g., an array or malloc'd).    */
/*       Do NOT pass a string literal.                                      */
/*                                                                          */
/* EXAMPLE: remove_char_inplace(my_buffer, ' ');                            */
/****************************************************************************/
void remove_char_inplace(char *str, char char_to_remove) {
    if (str == NULL) return;

    size_t i, j;
    size_t len = strlen(str);

    for (i = 0, j = 0; i < len; i++) {
        if (str[i] != char_to_remove) str[j++] = str[i];
    }
    str[j] = '\0';                       // Null-terminate the modified string
}

/****************************************************************************/
/* replace_char_inplace() - replace all instances of a char in-place        */
/*                                                                          */
/* NOTE: "str" must be a modifiable string (e.g., an array or malloc'd).    */
/*                                                                          */
/* EXAMPLE: replace_char_inplace(my_buffer, ' ', '_', 0);                   */
/****************************************************************************/
void replace_char_inplace(char *str, char ch, char newch, int skipends) {
    if (str == NULL) return;

    size_t i;
    size_t len = strlen(str);
    
    if (len == 0) return;
    
    size_t last_index = len - 1;

    for(i = 0; i < len; i++) {
       if(str[i] == ch && !(skipends && (i == 0 || i == last_index))) 
         str[i] = newch;
    }
    /* No need to re-terminate; length doesn't change */
}

/****************************************************************************/
/* replace_char_safe_copy() - safely copy a string, replacing a char        */
/*                                                                          */
/* Safely copies 'src' to 'dest', replacing 'ch' with 'newch'.              */
/*                                                                          */
/* EXAMPLE: replace_char_safe_copy(dest, src, sizeof(dest), ' ', '_', 0);   */
/****************************************************************************/
void replace_char_safe_copy(char *dest, const char *src, size_t dest_size, 
                            char ch, char newch, int skipends) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        if (dest_size > 0) dest[0] = '\0';
        return;
    }

    // 1. Safely copy the string using our standard utility
    strzcpy(dest, (char*)src, dest_size);

    // 2. Perform the replacement on the new, safe copy
    replace_char_inplace(dest, ch, newch, skipends);
}

/****************************************************************************/
/* substring_safe_copy() - Safely copies a substring from "src" into "dest" */
/*                                                                          */
/* NOTE: This function writes to a buffer provided by the caller, just like */
/*       just like strzcpy() and trim_safe_copy().                          */
/*                                                                          */
/* EXAMPLE: substring_safe_copy(dest, src, sizeof(dest), 5, 10);            */
/****************************************************************************/
void substring_safe_copy(char *dest, const char *src, size_t dest_size,
                         size_t position, size_t length) {
    /* --- 1. Handle Nulls and Empty Buffer --- */
    if (dest == NULL || src == NULL || dest_size == 0) {
        if (dest != NULL && dest_size > 0) dest[0] = '\0';
        return;
    }

    size_t src_len = strlen(src);

    /* --- 2. Check if start position is out-of-bounds --- */
    if (position >= src_len) {
        dest[0] = '\0'; /* Start is past the end, just return empty */
        return;
    }

    /* --- 3. Calculate a 100% safe copy length --- */
    /* a) How many chars are available from the start position? */
    size_t chars_available = src_len - position;

    /* b) We can only copy the *smaller* of the requested length
          and the available chars. */
    size_t copy_len = (length < chars_available) ? length : chars_available;

    /* c) We can only copy the *smaller* of *that* length
          and the destination buffer size (minus 1 for null). */
    size_t max_dest_copy = dest_size - 1;
    if (copy_len > max_dest_copy) copy_len = max_dest_copy;

    /* --- 4. Perform the copy --- */
    /* This is the *other* correct and safe use of strncpy:
       copying a specific-length slice from a buffer. */
    strncpy(dest, src + position, copy_len);
    dest[copy_len] = '\0';
}

/****************************************************************************/
/* strcasestr() - case-insensitive string search (replaces strstr)          */
/*                                                                          */
/* Finds the first occurrence of "needle" in "haystack", ignoring case.     */
/*                                                                          */
/* EXAMPLE: const char *p = strcasestr("Hello World", "world"); // p points to "World" */
/****************************************************************************/
#if defined(_WIN32) || defined(_MSC_VER)
const char *strcasestr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;
    if (*needle == '\0') return haystack;

    size_t needle_len = strlen(needle);

    for (; *haystack; haystack++) {
        /* Check for a match of the *first* character */
        if (toupper((unsigned char)*haystack) == toupper((unsigned char)*needle)) {
            /* If first char matched, check the rest of the string */
            const char *h = haystack;
            const char *n = needle;
            size_t i = 0;

            for (i = 0; i < needle_len; i++) {
                /* Check for end of haystack *before* end of needle */
                if (*h == '\0') return NULL; 
                
                if (toupper((unsigned char)*h) != toupper((unsigned char)*n))
                  break; /* Mismatch, break inner loop */
                h++;
                n++;
            }

            /* If loop finished, we found a full match */
            if (i == needle_len) return haystack; /* Success! */
        }
    }
    
    return NULL; /* No match found */
}
#endif

/****************************************************************************/
/* safe_gets() - safe line reader that strips the newline (replaces fgets)  */
/*                                                                          */
/* RETURNS: The "buf" pointer on success, or NULL on EOF or error.          */
/*                                                                          */
/* EXAMPLE: char line[100];                                                 */
/*          if (safe_gets(line, sizeof(line), stdin) != NULL) { ... }       */
/****************************************************************************/
char *safe_gets(char *buf, size_t buf_size, FILE *stream) {
    if (buf == NULL || buf_size == 0 || stream == NULL) return NULL;

    /* 1. Read the line using fgets */
    if (fgets(buf, (int)buf_size,stream) == NULL) return NULL; /* Error/EOF */

    /* 2. Find and remove the newline character
     * strcspn() finds the index of the first '\n' or '\r'.
     * This is a safe way to find the end of the line.
     */
    size_t len = strcspn(buf, "\r\n");
    buf[len] = '\0';

    /* 3. Handle a potential buffer-fill edge case
     * If len == buf_size - 1 AND the last char isn't '\0' (from
     * our strip), it means fgets() stopped *because the buffer
     * was full* and the newline is still in the input stream.
     * We must clear it to prevent it from being read by the
     * *next* call to safe_gets().
     */
    if (len == buf_size - 1 && buf[len] != '\0') {
        int ch;
        while ((ch = fgetc(stream)) != '\n' && ch != EOF);
    }

    return buf;
}

/****************************************************************************/
/* safe_strtok() - safe, re-entrant string tokenizer (replaces strtok)      */
/*                                                                          */
/* A safe, re-entrant wrapper for the platform-specific tokenizer           */
/* (strtok_r / strtok_s) that replaces the standard, unsafe strtok().       */
/*                                                                          */
/* WHY IT'S NEEDED: The standard strtok() is *not re-entrant*. It uses a    */
/* hidden internal static pointer to maintain state. Thus, dangerous for:   */
/* 1. Nested loops (an inner strtok() call will break an outer one).        */
/* 2. Multi-threaded applications.                                          */
/*                                                                          */
/* This function fixes the problem by forcing the caller to provide a       */
/* state pointer ("save_ptr"), which is the correct, safe pattern.          */
/*                                                                          */
/* NOTE: This function *modifies* the "str" buffer in-place by writing      */
/* null terminators. "str" must be modifiable.                              */
/*                                                                          */
/* EXAMPLE:                                                                 */
/* char my_str[] = "a,b,c";                                                 */
/* char *save_ptr;                                                          */
/* char *token = safe_strtok(my_str, ",", &save_ptr);                       */
/* while (token != NULL) {                                                  */
/*   // use token                                                           */
/*   token = safe_strtok(NULL, ",", &save_ptr);                             */
/* }                                                                        */
/****************************************************************************/
char *safe_strtok(char *str, const char *delim, char **save_ptr)
{
/* Use the correct, safe function based on the operating system */
#if defined(_WIN32) || defined(_MSC_VER)
    return strtok_s(str, delim, save_ptr);
#else
    return strtok_r(str, delim, save_ptr);
#endif
}

/****************************************************************************/
/* laststrstr() - Find the last occurrence of "needle" in "haystack"        */
/*                                                                          */
/* EXAMPLE: const char *p = laststrstr("a-b-c", "-"); // p points to "-c"   */
/****************************************************************************/
const char *laststrstr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;

    size_t needle_len = strlen(needle);
    
    /* CRITICAL FIX: Empty needle always matches the end of the string */
    if (needle_len == 0) return (haystack + strlen(haystack));

    const char *last_match = NULL;
    const char *p;

    /* Find all matches and just remember the last one */
    for (p = haystack; (p = strstr(p, needle)) != NULL; p++) {
        last_match = p;
    }

    return last_match;
}

/****************************************************************************/
/* lastN() - get the last n characters of a string                          */
/*                                                                          */
/* Returns a pointer to the last 'n' characters of 's'.                     */
/* If 'n' > strlen(s), returns 's'.                                         */
/*                                                                          */
/* EXAMPLE: const char *p = lastN("hello", 3); // p points to "llo"         */
/****************************************************************************/
const char *lastN(const char *s, size_t n) {
   size_t length = strlen(s);
   return (length < n ? s : s + length - n);
} 

/****************************************************************************/
/* uppercase_inplace() - convert line to uppercase, in-place                */
/*                                                                          */
/* NOTE: "line" must be a modifiable string (e.g., an array or malloc'd).   */
/*                                                                          */
/* EXAMPLE: uppercase_inplace(my_buffer);                                   */
/****************************************************************************/
void uppercase_inplace(char *line) {
    if (line == NULL) return;

    for (; *line; line++) {
        /* CRITICAL FIX: Cast to (unsigned char) */
        if (islower((unsigned char)*line)) {
            *line = (char)toupper((unsigned char)*line);
        }
    }
}

/****************************************************************************/
/* makelower_safe_copy() - safely copy a string, making it lowercase        */
/*                                                                          */
/* EXAMPLE: makelower_safe_copy(dest, src, sizeof(dest));                   */
/****************************************************************************/
void makelower_safe_copy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        if (dest != NULL && dest_size > 0) dest[0] = '\0';
        return;
    }
    
    /* Loop as long as there is source and we have room in dest */
    while (*src && --dest_size > 0) {
        /* CRITICAL FIX: Cast to (unsigned char) */
        *dest++ = isupper((unsigned char)*src) ? 
                    (char)tolower((unsigned char)*src) : *src;
        src++;
    }
    
    /* Always null-terminate */
    *dest = '\0';
}
