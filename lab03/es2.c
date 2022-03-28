#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFSIZE 32

void* th_f1(void* par) {
    char* src = (char*)par;
    int i = 0, j = 0;

    while (src[i] != '\0') {
        while (src[i] != '\n') {
            if (src[i] == ' ' && j == 0) {
                do {
                    i++;
                    j++;
                } while (src[i] != ' ');
                src[i - 1]++;
            }
            i++;
        }
        j = 0;
        i++;
    }
    pthread_exit(NULL);
}
void* th_f2(void* par) {
    char* src = (char*)par;
    int i = 0, j = 0, n = 0;
    while (src[n] != '\0') {
        n++;
    }
    n--;

    src + n;
    src[i]--;
    for (i = 0; i < n; i--) {
        if (src[i] != '\n') {
            src[i - 1]--;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const* argv[]) {
    int f_in;
    struct stat sbuf;
    pthread_t th1, th2;
    void* src;

    if ((f_in = open("file_1", O_RDWR)) < 1) {
        fprintf(stderr, "Error in opening file\n");
        abort();
    }

    if (fstat(f_in, &sbuf) < 0) {
        fprintf(stderr, "Error in fstat\n");
        abort();
    }

    printf("file size: %ld\n", sbuf.st_size);

    if ((src = mmap(0, sbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f_in,
                    0)) == MAP_FAILED) {
        fprintf(stderr, "Error in mmap\n");
        abort();
    }

    pthread_create(&th1, NULL, th_f1, (void*)src);
    // pthread_create(&th2, NULL, th_f2, (void*)src);

    pthread_join(th1, NULL);
    // pthread_join(th2, NULL);

    munmap(src, sbuf.st_size);

    return 0;
}