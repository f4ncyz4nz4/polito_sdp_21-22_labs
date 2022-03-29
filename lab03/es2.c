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

typedef struct {
    int id;
    long int regNum;
    char surname[31];
    char name[31];
    int mark;
} str;

typedef struct {
    int l;
    char* src;
} param;

void* th_f1(void* par) {
    param* p = (param*)par;
    int i = 0, j = 0;
    str e;

    while (p->src[i] != '\0') {
        while (p->src[i] != '\n') {
            i++;
            j++;
        }
        sscanf(p->src, "%d %ld %s %s %d", &e.id, &e.regNum, e.name, e.surname,
               &e.mark);
        e.regNum++;
        sprintf(p->src, "%d %ld %s %s %d", e.id, e.regNum, e.name, e.surname,
                e.mark);
        if (j < p->l) {
            p->src[i] = '\n';
        }
        p->src += (i + 1);
        i = 0;
    }
    pthread_exit(NULL);
}
void* th_f2(void* par) {
    param* p = (param*)par;
    int i = 0, j = 0;
    str e;

    while (p->src[i] != '\0') {
        while (p->src[i] != '\n') {
            i++;
            j++;
        }
        sscanf(p->src, "%d %ld %s %s %d", &e.id, &e.regNum, e.name, e.surname,
               &e.mark);
        e.mark--;
        sprintf(p->src, "%d %ld %s %s %d", e.id, e.regNum, e.name, e.surname,
                e.mark);
        if (j < p->l) {
            p->src[i] = '\n';
        }
        p->src += (i + 1);
        i = 0;
    }
    pthread_exit(NULL);
}

int main(int argc, char const* argv[]) {
    int f_in;
    struct stat sbuf;
    pthread_t th1, th2;
    param* p1 = (param*)malloc(sizeof(param));
    param* p2 = (param*)malloc(sizeof(param));
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

    p1->l = sbuf.st_size;
    p1->src = (char*)src;
    p2->l = sbuf.st_size;
    p2->src = (char*)src;

    pthread_create(&th1, NULL, th_f1, (void*)p1);
    pthread_create(&th2, NULL, th_f2, (void*)p2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    munmap(src, sbuf.st_size);

    free(p1);
    free(p2);

    return 0;
}