#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    
    // As stdin, stdout and stderr still doesn't exist
    // let's create them
    int stdin = open("/dev/console", O_RDONLY, 0);
    int stdout = open("/dev/console", O_RDWR, 0);

    dup2(stdin, 0);
    dup2(stdout, 1);
    dup2(stdout, 2); // stderr

    if (stdin > 2) close(stdin);
    if (stdout > 2) close(stdout);
    
    return 0;
}
