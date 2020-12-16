#ifndef _CTYPE_H
#define _CTYPE_H

#define isupper(c) (c >= 65 && c <= 90)
#define islower(c) (c >= 97 && c <= 122)
#define isalpha(c) (isupper(c) || islower(c))

#define isdigit(c)  (c >= '0' && c <= '9')
#define isxdigit(c) (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

#define isspace(c) (c == 32 || c == 9 || c == 10)
#define ispunct(c)

#define isalnum(c) (isalpha(c) || isdigit(c))
#define isprint(c) (c >= 32 && c <= 126)
#define iscntrl(c) (c >= 0 && c <= 31 || c == 127)
#define isascii(c) (c >= 0 && c <= 127)

#define toupper(c) (c - 'a' + 'A')
#define tolower(c) (c - 'A' + 'a')
#define toascii(c) (c & 127)

#endif