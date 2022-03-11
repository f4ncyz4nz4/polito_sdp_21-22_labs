#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    int n;
    int max;
    pthread_t *anc;
}arg;


void* tfunct(void *par){
    pthread_t tid1, tid2;
    int i;
    arg *a, *a1, *a2;
    void *status;

    a=(arg*) par;
    if(a->n < a->max){
        a1=(arg*) malloc(sizeof(arg));
        a2=(arg*) malloc(sizeof(arg));

        a1->anc=(pthread_t*) malloc((a->max)*sizeof(pthread_t));
        a2->anc=(pthread_t*) malloc((a->max)*sizeof(pthread_t));

        a1->max=a->max;
        a2->max=a->max;
        
        a1->n=a->n;
        for(i=0;i<(a->n);i++)
            a1->anc[i]=a->anc[i];
        a1->anc[(a1->n)]=pthread_self();
        a1->n++;

        a2->n=a->n;
        for(i=0;i<(a->n);i++)
            a2->anc[i]=a->anc[i];
        a2->anc[a2->n]=pthread_self();
        a2->n++;

        pthread_create(&tid1, NULL, tfunct,(void *) a1);
        pthread_create(&tid2, NULL, tfunct,(void *) a2);
        
        pthread_join(tid1, &status);
        free(a1->anc);
        free(a1);

        pthread_join(tid2, &status);
        free(a2->anc);
        free(a2);
    }
    else{
        for(i=0;i<a->max;i++)
            printf("tid=%ld ", a->anc[i]);
        printf("\n");
    }
    
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    arg *a1;
    int n=atoi(argv[1]);
    n++;

    a1=(arg*) malloc(sizeof(arg));
    a1->anc=(pthread_t*) malloc(n*sizeof(pthread_t));

    a1->max=n;
    a1->n=0;
    a1->anc=(pthread_t*) malloc((a1->max)*sizeof(pthread_t));

    //a1->anc[a1->n]=pthread_self();

    //(a1->n)++;

    tfunct((void*) a1);

    free(a1->anc);
    free(a1);

    return 0;
}