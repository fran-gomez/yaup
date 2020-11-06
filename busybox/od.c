#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void od(FILE *fp);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i;
	FILE *fp;
	struct stat st;

	if (argc < 2)
		print_help();

	for (i = 1; i < argc; i++) {
		stat(argv[i], &st);
		if (!S_ISREG(st.st_mode))
			errx(EISDIR, "'%s' is not a regular file", argv[i]);

		fp = fopen(argv[i], "r");
		if (!fp)
			err(errno, "%s\n", argv[i]);

		od(fp);
		fclose(fp);
	}

	return 0;
}

void od(FILE *fp) {

	int c, cnt = 0;
	unsigned int offset = 0;

	c = fgetc(fp);
	fprintf(stdout, "%07o", offset);
	while (c != EOF) {
		fprintf(stdout, "%06o ", c);
		c = fgetc(fp);

		cnt++;
		if (cnt == 8) {
			offset += 16;
			fprintf(stdout, "\n%07o ", offset);
			cnt = 0;
		}
	}
}

void print_help() {

	fputs("usage: od files...\n\n", stderr);

	fputs("Print the octal dump for each file\n", stderr);
}