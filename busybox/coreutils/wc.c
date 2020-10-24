#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void wc(FILE *fp);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i;
	FILE *fp;
	struct stat st;

	if (argc == 1) {
		fp = stdin;
		wc(fp);
	}

	if (argc == 2) 
		if (argv[1][0] == '-' && argv[1][1] == 'h')
			print_help();
	
	for (i = 1; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		fp = fopen(argv[i], "r");
		if (!fp) 
			err(errno, "%s\n", argv[i]);
		else {
			fprintf(stdout, "%s: ", argv[i]);
			wc(fp);
			fclose(fp);
		}
	}

	return 0;
}

void wc(FILE *fp) {

	int c, bytes, lines, words;

	bytes = lines = words = 0;

	c = fgetc(fp);
	 while (c != EOF) {
		if (isalnum(c) || ispunct(c))
			bytes++;
		if (isblank(c))
			words++;
		if (c == '\n')
			lines++;

		c = fgetc(fp);
	}

	fprintf(stdout, "%d %d %d\n", bytes, words, lines);
}

void print_help() {

	fputs("usage: wc files...\n\n", stderr);

	fputs("Count the bytes, lines and words of each file.\n", stderr);
	exit(EINVAL);
}