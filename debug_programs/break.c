#include <sys/ptrace.h>
#include <stdio.h>

int         iwanta_locsection() {
    char    c[] = "putain de bordel de ta race la chauve";
    return 0;
}

int         calculate(void) {
    int     i = 2, j = 2;
    return i + j;
}

int         main() {
    printf("Hello, World ! %d\n", calculate());
    return 0;
}
