#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void usage(const char *prog) {
    fprintf(stderr, "usage: cat - | %s\n", prog);
}

int main(int argc, char* argv[]) {
    int c = 0;
    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    fprintf(stderr, "hello world\n");

    return 0;
}

