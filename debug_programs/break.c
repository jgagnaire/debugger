#include <sys/ptrace.h>
#include <stdio.h>

int         iwanta_locsection() {
    char    c[] = "putain de bordel de ta race la chauve";
    return 0;
}

int         calculate(int p) {
    int     i = 2, j = 2;
    return p + i + j;
}

int         main() {
    printf("Hello, World ! %d\n", calculate(0));
    return 0;
}
