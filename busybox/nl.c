#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <err.h>
#include <errno.h>

#include <sys/stat.h>


void print_help();
void nl(FILE *fp);


int main(int argc, char const *argv[], char const *envp[]) {
	
	int i;
	FILE *fp;
	struct stat st;

	if (argc == 1)
		fp = stdin;

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

		nl(fp);
		fclose(fp);
	}

	return 0;
}

int is_blank(char *line) {

	while (*line++)
		if (!isblank(*line))
			return 0;
	return 1;
}

void nl(FILE *fp) {

	int line_num = 0;
	long unsigned int buff_sz = 512;
	char **buff;

	buff = (char**) malloc(sizeof(char*)*buff_sz);
	while (!feof(fp)) {

		//fscanf(fp, "%[^\n]\n", buff);
		getline(buff, &buff_sz, fp);

		if (!is_blank(*buff))
			fprintf(stdout, "%5d  %s", ++line_num, *buff);
		else
			fprintf(stdout, "\n");
	}
}

void print_help() {

	fputs("usage: nl files...\n\n", stdout);

	fputs("Print the line number alongside the file\n", stdout);
	fputs("Use stdin as input if no file is specified\n", stdout);
	
	exit(EINVAL);
}