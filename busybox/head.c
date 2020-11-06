#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void head(FILE *fp, int lines);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i, lines;
	FILE *fp;
	struct stat st;

	if (argc < 3)
		print_help();

	if (argv[1][0] == '-') {
		++argv[1];
		lines = atoi(argv[1]);
	} else 
		errx(EISDIR, "%s: invalid argument\n", argv[1]);

	for (i = 2; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		if ((fp = fopen(argv[i], "r")) != NULL) {
			head(fp, lines);
			fclose(fp);
		} else
			err(errno, "%s", argv[i]);
	}

	return 0;
}

void head(FILE *fp, int lines) {

	int i = 0, c, max_lines;

	max_lines = atoi(getenv("COLUMNS"));

	c = fgetc(fp);
	while (i < lines && c != EOF) {
		fputc(c, stdout);
		c = fgetc(fp);

		if (c == '\n')
			i++;

		if (i >= max_lines)
			while (fgetc(stdin) != ' ');
	}
}

void print_help() {

	fputs("usage: head -N files...\n\n", stderr);

	fputs("Print the first N lines of files to stdout\n", stderr);

	exit(EINVAL);
}