#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void tail(FILE *fp, int lines);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i, lines;
	FILE *fp;
	struct stat st;

	if (argc < 3)
		print_help();

	if (argv[1][0] == '-')
		lines = atoi(++argv[1]);
	else 
		errx(EINVAL, "'%s' is not a valid argument\n", argv[1]);

	for (i = 2; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		if ((fp = fopen(argv[i], "r")) != NULL) {
			tail(fp, lines);
			fclose(fp);
		} else
			err(errno, "%s\n", argv[i]);
	}

	return 0;
}

void tail(FILE *fp, int lines) {
	errx(EINPROGRESS, "Not yet implemented\n");
}

void print_help() {

	fputs("usage: tail -N FILEs...\n", stderr);
	fputs("Print the last N lines of files to stdout\n", stderr);

	exit(0);
}