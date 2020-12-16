#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help(void);
void cmp(FILE *f1, FILE *f2);


int main(int argc, char const *argv[], char const *envp[]) {
	
	FILE *f1, *f2;
	struct stat st;

	if (argc != 3)
		print_help();
		

	stat(argv[1], &st);
	if (!S_ISREG(st.st_mode))
		errx(EISDIR, "'%s' is not a regular file\n", argv[1]);
	else
		if ((f1=fopen(argv[1], "r")) == NULL) 
			err(errno, "%s\n", argv[1]);

	stat(argv[2], &st);
	if (!S_ISREG(st.st_mode))
		errx(EISDIR, "'%s' is not a regular file\n", argv[2]);
	else
		if ((f2=fopen(argv[2], "r")) == NULL)
			err(errno, "%s\n", argv[2]);

	fprintf(stdout, "%s %s ", argv[1], argv[2]);
	cmp(f1, f2);
	fclose(f1);
	fclose(f2);

	return 0;
}

void cmp(FILE *f1, FILE *f2) {
	
	int c1, c2;
	int line = 0, byte = 0;

	while (!feof(f1) && !feof(f2)) {
		c1 = fgetc(f1);
		c2 = fgetc(f2);
		byte++;

		if (c1 != c2)
			break;

		if (c1 == '\n' || c2 == '\n')
			line++;
	}

	if (c1 != c2)
		fprintf(stdout, "differ: char %d, line %d\n", byte, line);
	else
		fputs("doesn't differ\n", stdout);
}

void print_help(void) {

	fputs("usage: cmp file1 file2\n\n", stderr);

	fputs("Compare two files\n", stderr);

	exit(EINVAL);
}