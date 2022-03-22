#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int n = 0, *vett, i = 0;

    sleep(2);
    n = atoi(argv[1]);
    vett = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        if (fork())
            vett[i] = 0;
        else
            vett[i] = 1;
    }
    for (i = 0; i < n; i++) printf("%d", vett[i]);
    printf("\n");

    free(vett);

    return 0;
}