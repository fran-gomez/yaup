#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>

//#include "regex.h"


void grep(FILE *fp, const char *pattern);
void print_help();
char *parse_regex(const char *reg) { return reg; }


int main(int argc, char const *argv[], char const *envp[]) {

	int i;
	FILE *fp;
	struct stat st;
	char *pattern;

	if (argc < 3)
		print_help();

	pattern = parse_regex(argv[1]);

	for (i = 2; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		if (!(fp = fopen(argv[i], "r")))
			err(errno, "%s\n", argv[i]);

		grep(fp, pattern);
		fclose(fp);
	}


	return 0;
}

void grep(FILE *fp, const char *pattern) {

	int i, j, match, plen;
	char *buff;

	buff = (char*) malloc(sizeof(char)*512);
	plen = strlen(pattern);

	while (!feof(fp)) {
		fscanf(fp, "%[^\n]\n", buff);

		i = 0; j = 0;
		while (buff[i] != '\0')
			if (buff[i] != pattern[j]) {

				i++; j = 0;
				match = 0;
			} else {
				
				i++; j++;
				match = 1;

				if (j >= plen)
					break;
			}

		if (match)
			fprintf(stdout, "%s\n", buff);
	}

	free(buff);
}

void print_help() {

	fputs("usage: grep regex FILEs...\n", stderr);
	fputs("Search for regex in files and print the matching lines\n", stderr);

	exit(EINVAL);
}