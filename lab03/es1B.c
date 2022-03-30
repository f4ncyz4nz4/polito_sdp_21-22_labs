#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WAIT_TIME_1 0.1
#define WAIT_TIME_2 1
#define STR_SIZE 8
#define STR_NUM 4

int main(int argc, char const *argv[]) {
    int fd1[2], fd2[2], i, j, lenght, flags, k = 0;
    pid_t child[2];
    char buffer[STR_SIZE + 1];

    pipe(fd1);
    if ((child[0] = fork())) {
        close(fd1[1]);

        flags = fcntl(fd1[0], F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(fd1[0], F_SETFL, flags);

        pipe(fd2);
        if ((child[1] = fork())) {
            close(fd2[1]);

            flags = fcntl(fd2[0], F_GETFL, 0);
            flags |= O_NONBLOCK;
            fcntl(fd2[0], F_SETFL, flags);

            while (k < (STR_NUM * 2)) {
                if (read(fd1[0], buffer, STR_SIZE) > 0) {
                    for (j = 0; j < STR_SIZE; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    fprintf(stdout, "1: %s\n", buffer);
                    k++;
                }
                if (read(fd2[0], buffer, STR_SIZE) > 0) {
                    for (j = 0; j < STR_SIZE; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    fprintf(stdout, "2: %s\n", buffer);
                    k++;
                }
            }

            waitpid(child[0], NULL, 0);
            waitpid(child[1], NULL, 0);

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