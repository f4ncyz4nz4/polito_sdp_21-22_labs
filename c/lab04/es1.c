#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    pthread_mutex_t mutex;
    sem_t sem1;
    sem_t sem2;
    int count;
} barrier_t;

typedef struct {
    int i;
    int N;
    int n;
    int step;
    sem_t* sem;
    int* v;
    barrier_t* bar;
} arg_t;

void print_array(int* v, int step, int tot) {
    if (step == 0)
        fprintf(stdout, "Initial array v: ");
    else
        fprintf(stdout, "Step %d v: ", step);
    for (int i = 0; i < tot; i++) fprintf(stdout, "%d ", v[i]);
    fprintf(stdout, "\n");
}

void* cumulative_sum(void* par) {
    arg_t* arg = (arg_t*)par;
    int dist = 0;

    while (arg->step < arg->n + 1) {
        pthread_mutex_lock(&(arg->bar->mutex));
        arg->bar->count++;
        if (arg->bar->count == arg->N - 1) {
            for (int i = 0; i < arg->N - 1; i++) {
                sem_post(&(arg->bar->sem1));
            }
        }
        pthread_mutex_unlock(&(arg->bar->mutex));
        sem_wait(&(arg->bar->sem1));
        //----------------
        sem_wait(&(arg->sem[arg->i]));
        dist = pow(2, (arg->step - 1));
        if ((arg->i - dist) >= 0) {
            arg->v[arg->i] += arg->v[arg->i - dist];
        }
        if (arg->i - 1 > 0)
            sem_post(&(arg->sem[arg->i - 1]));
        else
            sem_post(&(arg->sem[arg->N - 1]));
        //----------------
        pthread_mutex_lock(&(arg->bar->mutex));
        arg->bar->count--;
        if (arg->bar->count == 0) {
            print_array(arg->v, arg->step, arg->N);
            for (int i = 0; i < arg->N - 1; i++) {
                sem_post(&(arg->bar->sem2));
            }
        }
        pthread_mutex_unlock(&(arg->bar->mutex));
        sem_wait(&(arg->bar->sem2));
        arg->step++;
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Few arguments\n");
        abort();
    }
    int n = atoi(argv[1]), N = (int)pow(2, n), T = N - 1;
    pthread_t* thr = (pthread_t*)malloc(T * sizeof(pthread_t));
    arg_t* args = (arg_t*)malloc(T * sizeof(arg_t));
    int* v = (int*)malloc(N * sizeof(int));

    sem_t* sem_sum = (sem_t*)malloc(N * sizeof(sem_t));
    sem_init(&sem_sum[N - 1], 0, 1);

    barrier_t* barrier = (barrier_t*)malloc(sizeof(barrier_t));
    sem_init(&(barrier->sem1), 0, 0);
    sem_init(&(barrier->sem2), 0, 0);
    pthread_mutex_init(&(barrier->mutex), NULL);
    barrier->count = 0;

    for (int i = 0; i < N; i++) v[i] = rand() % 10;
    print_array(v, 0, N);

    for (int i = 0; i < T; i++) {
        args[i].i = i + 1;
        args[i].N = N;
        args[i].n = n;
        args[i].step = 1;
        args[i].sem = sem_sum;
        args[i].v = v;
        args[i].bar = barrier;
        pthread_create(&thr[i], NULL, cumulative_sum, (void*)&args[i]);
    }
    for (int i = 0; i < T; i++) {
        pthread_join(thr[i], NULL);
    }

    free(barrier);
    free(sem_sum);
    free(v);
    free(args);
    free(thr);

    return 0;
}