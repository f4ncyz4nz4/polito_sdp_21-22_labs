#include <aio.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WAIT_TIME_1 1
#define WAIT_TIME_2 0
#define STR_SIZE 4
#define STR_NUM 3

int k1 = 0, k2 = 0, f1 = 1, f2 = 1;
char buffer1[STR_SIZE + 1], buffer2[STR_SIZE + 1];

void inc1(int s) {
    for (int j = 0; j < STR_SIZE; j++) {
        buffer1[j] = toupper(buffer1[j]);
    }
    fprintf(stdout, "1: %s\n", buffer1);
    k1++;
    f1 = 1;
    return;
}
void inc2(int s) {
    for (int j = 0; j < STR_SIZE; j++) {
        buffer2[j] = toupper(buffer2[j]);
    }
    fprintf(stdout, "2: %s\n", buffer2);
    k2++;
    f2 = 1;
    return;
}

int main(int argc, char const *argv[]) {
    int fd1[2], fd2[2], i, j, lenght;
    pid_t child[2];
    struct aiocb acb1, acb2;

    setbuf(stdout, 0);
    signal(SIGUSR1, inc1);
    signal(SIGUSR2, inc2);

    pipe(fd1);
    if ((child[0] = fork())) {
        close(fd1[1]);

        pipe(fd2);
        if ((child[1] = fork())) {
            close(fd2[1]);

            acb1.aio_fildes = fd1[0];
            acb1.aio_offset = 0;
            acb1.aio_buf = buffer1;
            acb1.aio_nbytes = STR_SIZE;
            acb1.aio_reqprio = 0;
            acb1.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
            acb1.aio_sigevent.sigev_signo = SIGUSR1;

            acb2.aio_fildes = fd2[0];
            acb2.aio_offset = 0;
            acb2.aio_buf = buffer2;
            acb2.aio_nbytes = STR_SIZE;
            acb2.aio_reqprio = 0;
            acb2.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
            acb2.aio_sigevent.sigev_signo = SIGUSR2;

            while (k1 < STR_NUM || k2 < STR_NUM) {
                if (k1 < STR_NUM && f1) {
                    aio_read(&acb1);
                    f1 = 0;
                }
                if (k2 < STR_NUM && f2) {
                    aio_read(&acb2);
                    f2 = 0;
                }
                pause();
            }

            waitpid(child[0], NULL, 0);
            waitpid(child[1], NULL, 0);
            sleep(0.5);
            close(fd1[0]);
            close(fd2[0]);

        } else {  // child 2
            close(fd2[0]);
            for (i = 0; i < STR_NUM; i++) {
                sleep(WAIT_TIME_2);
                lenght = 1 + (rand() % STR_SIZE);
                for (j = 0; j < lenght; j++) {
                    buffer2[j] = (char)('a' + (rand() % 25));
                }
                write(fd2[1], buffer2, STR_SIZE);
            }
        }
    } else {  // child 1
        close(fd1[0]);
        for (i = 0; i < STR_NUM; i++) {
            sleep(WAIT_TIME_1);
            lenght = 1 + (rand() % STR_SIZE);
            for (j = 0; j < lenght; j++) {
                buffer1[j] = (char)('a' + (rand() % 25));
            }
            write(fd1[1], buffer1, STR_SIZE);
        }
    }

    return 0;
}