#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char* filepath;
    int id;
} arg_t;

typedef struct {
    int* a;
    int size;
} res_t;

void merge(int* left, int l_size, int* right, int r_size, int* res) {
    int l = 0, r = 0, k = 0;
    if (right == NULL) {
        for (int i = 0; i < l_size; i++) res[i] = left[i];
        return;
    }
    while (k < (r_size + l_size)) {
        if (l == l_size) {
            res[k++] = right[r++];
        } else if (r == r_size) {
            res[k++] = left[l++];
        } else if (right[r] > left[l]) {
            res[k++] = left[l++];
        } else {
            res[k++] = right[r++];
        }
    }
    return;
}

void* order(void* par) {
    arg_t* arg = (arg_t*)par;
    int fd = open(arg->filepath, O_RDONLY);
    int tmp;
    res_t* res = (res_t*)malloc(sizeof(res_t));

    read(fd, &(res->size), sizeof(int));
    res->a = (int*)malloc(res->size * sizeof(int));
    for (int i = 0; i < res->size; i++) read(fd, &(res->a[i]), sizeof(int));

    close(fd);

    for (int i = 0; i < res->size; i++) {  // loop n times - 1 per element
        for (int j = 0; j < (res->size) - i - 1;
             j++) {  // last i elements are sorted already
            if (res->a[j] > res->a[j + 1]) {  // swop if order is broken
                tmp = res->a[j];
                res->a[j] = res->a[j + 1];
                res->a[j + 1] = tmp;
            }
        }
    }

    pthread_exit((void*)res);
}

int main(int argc, char const* argv[]) {
    arg_t* arg = (arg_t*)malloc((argc - 2) * sizeof(arg_t));
    pthread_t* thr = (pthread_t*)malloc((argc - 2) * sizeof(pthread_t));
    res_t* r = (res_t*)malloc(sizeof(res_t));
    int *res, *old_res, totsize = 0, fd;

    if (argc < 3) {
        fprintf(stderr, "Few arguments\n");
        abort();
    }

    for (int i = 0; i < argc - 2; i++) {
        arg[i].filepath = (char*)malloc(strlen(argv[i + 1]) * sizeof(char));
        strcpy(arg[i].filepath, argv[i + 1]);
        arg[i].id = (i + 1);
        pthread_create(&thr[i], NULL, order, (void*)&arg[i]);
    }
    for (int i = 0; i < argc - 2; i++) {
        pthread_join(thr[i], (void**)&r);

        if (totsize == 0) {
            totsize += r->size;
            res = (int*)malloc(totsize * sizeof(int));
            merge(r->a, r->size, NULL, 0, res);
        } else {
            old_res = res;
            res = (int*)malloc((totsize + r->size) * sizeof(int));
            merge(r->a, r->size, old_res, totsize, res);
            totsize += r->size;
            free(old_res);
        }
    }
    for (int i = 0; i < totsize; i++) printf("%d ", res[i]);

    fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_SYNC, 0666);
    write(fd, res, totsize * sizeof(int));
    close(fd);

    return 0;
}
