#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void translate(FILE *fp, const char *src, const char *dst);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i;
	FILE *fp;
	struct stat st;

	if (argc < 4)
		print_help();

	for (i = 3; i<argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		fp = fopen(argv[i], "r");
		if (fp != NULL) {
			translate(fp, argv[1], argv[2]);
			fclose(fp);
		} else 
			err(errno, "%s\n", argv[i]);
	}

	return 0;
}

void translate(FILE *fp, const char *str1, const char *str2) {

	int i, j, c;

	j = strlen(str1);
	if (j != strlen(str2)) {
		fprintf(stderr, "tr: Error: source set and destiny set doesnt match\n");
		exit(EINVAL);
	}

	c = fgetc(fp);
	while (c != EOF) {
		i = 0;
		while (i < j) {
			if (c == str1[i])
				c = str2[i];
			i++;
		}

		fputc(c, stdout);
		c = fgetc(fp);
	}
}

void print_help() {

	fputs("usage: tr str1 str2 files...\n\n", stderr);

	fputs("Translate characters from str1 to str2 for each file, and print the result to stdout\n", stderr);

	exit(EINVAL);
}