#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int i, j, n, h;
    if (argc != 3) printf("ERRORE");
    n = atoi(argv[1]);
    h = atoi(argv[2]);

    for (i = 0; i < h + 1; i++) {
        for (j = 0; j < n - 1; j++) {
            if (!fork()) break;
        }
    }
    printf("pid=%d\n", getpid());

    return 0;
}