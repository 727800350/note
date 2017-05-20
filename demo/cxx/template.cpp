#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

void usage(const char *prog) {
    fprintf(stderr, "usage: cat - | %s\n", prog);
}

int process(const char *line){
    fprintf(stdout, "%s\n", line);
    return 0;
}

int main(int argc, char* argv[]) {
    int ret = 0;
    while ((ret = getopt(argc, argv, "h")) != -1) {
        switch (ret) {
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    int num = 0;
    char *line = new char[max_vl];
    while (fgets(line, max_vl, stdin) != NULL) {
        line[strlen(line) - 1] = '\0';
        ret = process(line);
        if (ret != 0) {
            fprintf(stderr, "process %s error %d\n", line, ret);
            return -1;
        }

        num++;
    }

    fprintf(stderr, "total num: %d\n", num);

    delete []line;

    return 0;
}

