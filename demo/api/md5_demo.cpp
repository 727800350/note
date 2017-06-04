#include <stdio.h>
#include <unistd.h>
#include <openssl/md5.h>

/*
 * g++ md5_demo.cpp -lcrypto
 */

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

int usage(const char *prog) {
    fprintf(stderr, "cat - | %s [-h]\n", prog);

    return 0;
}

int process(int vl, char *v, MD5_CTX *c) {
    MD5_Update(c, v, vl);

    return 0;
}

int main(int argc, char* argv[]) {
    char c = 0;
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

    MD5_CTX ctx;
    MD5_Init(&ctx);
    int vl = 0;
    char* v = new char[max_vl];

    while ((vl = fread(v, sizeof(char), max_vl, stdin)) > 0) {
        int ret = process(vl, v, &ctx);
        if (ret != 0) {
            fprintf(stderr, "process error\n");
            return -1;
        }
    }

    unsigned char md5[16] = {0};
    MD5_Final(md5, &ctx);
    for (int i = 0; i < 16; i++)  {
        fprintf(stdout, "%02x", (int)md5[i]);
    }
    fprintf(stdout, "\n");

    delete []v;

    return 0;
}

