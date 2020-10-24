#ifndef _TIME_H
#define _TIME_H

#include "sys/types.h"

// Time structure
struct tm {
    u8_t tm_sec; /* Seconds [0, 59] */
    u8_t tm_min; /* Minutes [0, 59] */
    u8_t tm_hour; /* Hours [0, 23] */

    u8_t tm_mday; /* Day of month [1, 31] */
    u8_t tm_mon; /* Month [1, 12] */
    u8_t tm_year; /* Years since 1900 */
    u8_t tm_wday; /* Week day [0, 6] (Sunday=0) */
    u16_t tm_yday; /* Year day [1, 365] */

    u8_t tm_isdst; /* Daylight saving flags */
};

// DOC PENDIENTE
struct timespec {
    time_t tv_sec;  /* Time in seconds */
    u64_t tv_nsec; /* Time in nanoseconds */
};

// Function headers
time_t time(time_t *t); /* Time since 1/1/1970 00:00:00 */
char *ctime(const time_t *t); /* Format the time to a broken down time */
char *asctime(const struct tm *tm); /* Same as ctime */
struct tm *gmtime(const time_t *t); /* Broke down a unix time */
struct tm *localtime(const time_t *t); /* Same as gmtime */
time_t mktime(struct tm *tm); /* Convert a given time to a unix time */

#endif