#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <err.h>
#include <errno.h>

void print_help(void);
void print_meminfo(FILE *fp, int n, char modifier);

int main(int argc, char const *argv[], char const *envp[]) {
    
    int factor;
    FILE *meminfo;

    meminfo = fopen("/proc/meminfo", "r");
    if (!meminfo)
        err(errno, "/proc/meminfo");
        

    if (argv[1] == NULL) {
        print_meminfo(meminfo, 1, 'k');
        return 0;
    } 
    
    if (argv[1][0] == '-') {
        
        switch(argv[1][1]) {
            case 'b':
                factor = 1/1024; // FIXME
                break;
            case 'k':
                factor = 1;
                break;
            case 'm':
                factor = 1024;
                break;
            case 'g':
                factor = 1024*1024;
                break;
            default:
                print_help();
        }

        print_meminfo(meminfo, factor, argv[1][1]);
    } else
        print_help();
    


    return 0;
}

void print_meminfo(FILE *fp, int n, char modifier) {

    double mem[6];
    size_t bsize = 512;
    char **line, *token;

    line = (char**) malloc(sizeof(char*));
    mem[4] = 0;

    while (!feof(fp)) {
        getline(line, &bsize, fp);
        token = strtok(*line, " :");

        if (strcmp(token, "MemTotal") == 0) {
            mem[0] = atof(strtok(NULL, " :"))/n;
            continue;
        }
        if (strcmp(token, "MemFree") == 0) {
            mem[2] = atof(strtok(NULL, " :"))/n;
            continue;
        }
        if (strcmp(token, "MemAvailable") == 0) {
            mem[5] = atof(strtok(NULL, " :"))/n;
            continue;
        }
        if (strcmp(token, "Buffers") == 0) {
            mem[4] += atof(strtok(NULL, " :"))/n;
            continue;
        }
        if (strcmp(token, "Cached") == 0) {
            mem[4] += atof(strtok(NULL, " :"))/n;
            continue;
        }
        if (strcmp(token, "Shmem") == 0) {
            mem[3] = atof(strtok(NULL, " :"))/n;
            continue;
        }
    }

    fprintf(stdout, "%-6s %-12s %-12s %-12s %-12s %-12s %-12s\n",
                    " ", "Total", "Used", "Free", "Shared", "Buff/Cache", "Available");
    fprintf(stdout, "%-6s %-12.1f %-12.1f %-12.1f %-12.1f %-12.1f %-12.1f\n",
                    "Mem: ", mem[0], 0.0, mem[2], mem[3], mem[4], mem[5]);
    fprintf(stdout, "Swap: Not yet implemented ;)\n");
}

void print_help(void) {
    fputs("HELP!", stdout);
    exit(EINVAL);
}