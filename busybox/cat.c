#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <err.h>

#include <sys/stat.h>


void print_help(void);
void cat(FILE *fp);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i;
	FILE *input;
	struct stat st;

	if (argc == 1) {
		input = stdin;
		cat(input);

	} else if (argc == 2)
		if (argv[1][0] == '-' && argv[1][1] == 'h')
			print_help();

	for (i = 1; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode)) 
			errx(EISDIR, "'%s' is a directory\n", argv[i]);

		if ((input=fopen(argv[i], "r")) == NULL)
			err(errno, "%s", argv[i]);

		cat(input);
		fclose(input);
	}

	return 0;
}

void cat(FILE *fp) {

	int c;

	c = fgetc(fp);
	while (c != EOF) {
		fputc(c, stdout);
		c = fgetc(fp);
	}
}

void print_help(void) {

	fputs("usage: cat file...\n\n", stderr);

	fputs("Concatenate files and print them to stdout.\n", stderr);
	fputs("If no file is specified, use stdin as input file\n", stderr);
	
	exit(EINVAL);
}