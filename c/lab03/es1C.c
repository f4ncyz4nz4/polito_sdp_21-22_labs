#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WAIT_TIME_1 0.1
#define WAIT_TIME_2 0.6
#define STR_SIZE 5
#define STR_NUM 3

int main(int argc, char const *argv[]) {
    int fd1[2], fd2[2], i, j, lenght, flags, k1 = 0, k2 = 0, max;
    pid_t child[2];
    char buffer[STR_SIZE + 1];
    fd_set readfds;

    setbuf(stdout, 0);
    pipe(fd1);
    if ((child[0] = fork())) {
        close(fd1[1]);

        pipe(fd2);
        if ((child[1] = fork())) {
            close(fd2[1]);

            max = ((fd1[0] > fd2[0]) ? fd1[0] : fd2[0]) + 1;

            while (k1 < STR_NUM || k2 < STR_NUM) {
                FD_ZERO(&readfds);
                if (k1 < STR_NUM) FD_SET(fd1[0], &readfds);
                if (k2 < STR_NUM) FD_SET(fd2[0], &readfds);
                if (select(max, &readfds, NULL, NULL, NULL) < 0) {
                    fprintf(stderr, "Error in select function");
                    abort();
                }
                if (FD_ISSET(fd1[0], &readfds)) {
                    read(fd1[0], buffer, STR_SIZE);
                    for (j = 0; j < STR_SIZE; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    fprintf(stdout, "1: %s\n", buffer);
                    k1++;
                }
                if (FD_ISSET(fd2[0], &readfds)) {
                    read(fd2[0], buffer, STR_SIZE);
                    for (j = 0; j < STR_SIZE; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    fprintf(stdout, "2: %s\n", buffer);
                    k2++;
                }
            }

            waitpid(child[0], NULL, 0);
            waitpid(child[1], NULL, 0);
            sleep(0.1);
            close(fd1[0]);
            close(fd2[0]);

        } else {  // child 2
            close(fd2[0]);
            for (i = 0; i < STR_NUM; i++) {
                sleep(WAIT_TIME_2);
                lenght = 1 + (rand() % STR_SIZE);
                for (j = 0; j < lenght; j++) {
                    buffer[j] = (char)('a' + (rand() % 25));
                }
                write(fd2[1], buffer, STR_SIZE);
            }
        }
    } else {  // child 1
        close(fd1[0]);
        for (i = 0; i < STR_NUM; i++) {
            sleep(WAIT_TIME_1);
            lenght = 1 + (rand() % STR_SIZE);
            for (j = 0; j < lenght; j++) {
                buffer[j] = (char)('a' + (rand() % 25));
            }
            write(fd1[1], buffer, STR_SIZE);
        }
    }

    return 0;
}