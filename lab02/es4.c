#include <dirent.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void *paral_visit(void *par) {
    DIR *d;
    struct dirent *dirp;
    struct stat sb;
    pthread_t t;
    char *name_d = (char *)par, file[257];

    d = opendir(name_d);

    while ((dirp = readdir(d)) != NULL) {
        strcpy(file, name_d);
        strcat(file, dirp->d_name);
        stat(file, &sb);

        if (S_ISDIR(sb.st_mode)) {
            if ((strcmp(".", dirp->d_name) != 0) &&
                (strcmp("..", dirp->d_name) != 0)) {
                strcat(file, "/");
                pthread_create(&t, NULL, paral_visit, (void *)file);
                pthread_join(t, NULL);
            }
        } else {
            fprintf(stdout, "Thread=%ld file=%s\n", pthread_self(), file);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    struct stat *sb = (struct stat *)malloc((argc - 1) * sizeof(struct stat));
    pthread_t *thr = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));
    int i;

    if (argc < 1) {
        printf("Too few arguments\n");
        abort();
    }

    for (i = 0; i < (argc - 1); i++) {
        stat(argv[i + 1], &sb[i]);
        if (!S_ISDIR(sb[i].st_mode)) {
            printf("Argument is not a dir\n");
            abort();
        }
        pthread_create(&thr[i], NULL, paral_visit, (void *)argv[i + 1]);
    }

    for (i = 0; i < (argc - 1); i++) {
        pthread_join(thr[i], NULL);
    }

    free(thr);
    free(sb);

    return 0;
}