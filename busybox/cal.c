#include <stdio.h>

#include <errno.h>


const char *wday[] = {
    "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa", NULL
};

const char *month[] = {
    "January", "February", "March",
    "April", "May", "June",
    "July", "August", "September",
    "October", "November", "December", NULL
};

void usage(void) {
    fputs("usage: cal -n NUM...\n\n", stderr);

    fputs("Print a calendar showing the current month\n", stderr);
    fputs("plus N months more.\n", stderr);
    fputs("If no option is passed, prints the current month\n", stderr);

    exit(EINVAL);
}