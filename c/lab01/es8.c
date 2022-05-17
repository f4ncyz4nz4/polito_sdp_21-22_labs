#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int **A;
    int **B;
    int i;
    int z;
    int c;
    int **C;
} str;

void *mat_mul(void *par) {
    str *mat;
    int somma = 0, i, c, z, j, **A, **B, **C;

    mat = (str *)par;

    A = mat->A;
    B = mat->B;
    C = mat->C;
    z = mat->z;
    c = mat->c;
    i = mat->i;

    for (j = 0; j < c; j++) {
        somma += (A[i][j]) * (B[j][z]);
    }

    C[i][z] = somma;

    pthread_exit(NULL);
}

int main() {
    int i, z, r = 3, c = 4, x = 2, **A, **B, **C;
    void *status;
    pthread_t *tid = (pthread_t *)malloc(r * x * sizeof(pthread_t));
    str *mat = (str *)malloc(r * x * sizeof(str));

    A = (int **)malloc(r * sizeof(int *));
    for (i = 0; i < r; i++) {
        A[i] = (int *)malloc(c * sizeof(int));
    }
    B = (int **)malloc(c * sizeof(int *));
    for (i = 0; i < c; i++) {
        B[i] = (int *)malloc(x * sizeof(int));
    }
    C = (int **)malloc(r * sizeof(int *));
    for (i = 0; i < r; i++) {
        C[i] = (int *)malloc(x * sizeof(int));
    }

    for (i = 0; i < r; i++)
        for (z = 0; z < c; z++) A[i][z] = rand() % 10;
    for (i = 0; i < c; i++)
        for (z = 0; z < x; z++) B[i][z] = rand() % 10;
    for (i = 0; i < r; i++)
        for (z = 0; z < x; z++) C[i][z] = 0;

    for (i = 0; i < (r * x); i++) {
        mat[i].A = A;
        mat[i].B = B;
        mat[i].C = C;
        mat[i].c = c;
    }

    for (i = 0; i < r; i++) {
        for (z = 0; z < x; z++) {
            mat[(i * x) + z].i = i;
            mat[(i * x) + z].z = z;
            pthread_create(&tid[(i * x) + z], NULL, mat_mul,
                           (void *)&mat[(i * x) + z]);
        }
        z = 0;
    }
    for (i = 0; i < (r * x); i++) {
        pthread_join(tid[i], &status);
    }
    free(tid);

    for (i = 0; i < r; i++) {
        printf("\n");
        for (z = 0; z < c; z++) printf("%d ", A[i][z]);
    }
    printf("\n");
    printf("\n");
    for (i = 0; i < c; i++) {
        printf("\n");
        for (z = 0; z < x; z++) printf("%d ", B[i][z]);
    }
    printf("\n");
    printf("\n");
    for (i = 0; i < r; i++) {
        printf("\n");
        for (z = 0; z < x; z++) printf("%d ", C[i][z]);
    }
    printf("\n");
    return 0;
}