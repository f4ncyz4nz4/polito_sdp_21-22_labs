#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WAIT_TIME_1 0.1
#define WAIT_TIME_2 1
#define STR_SIZE 5
#define STR_NUM 4

int main(int argc, char const *argv[]) {
    int fd1[2], fd2[2], i, j, lenght;
    pid_t child[2];
    char buffer[STR_SIZE + 1];

    pipe(fd1);
    if ((child[0] = fork())) {
        close(fd1[1]);

        pipe(fd2);
        if ((child[1] = fork())) {
            close(fd2[1]);

            for (i = 0; i < STR_NUM; i++) {
                read(fd1[0], buffer, STR_SIZE);
                for (j = 0; j < STR_SIZE; j++) {
                    buffer[j] = toupper(buffer[j]);
                }
                fprintf(stdout, "1: %s\n", buffer);
                read(fd2[0], buffer, STR_SIZE);
                for (j = 0; j < STR_SIZE; j++) {
                    buffer[j] = toupper(buffer[j]);
                }
                fprintf(stdout, "2: %s\n", buffer);
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
                buffer[lenght] = '\0';
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
            buffer[lenght] = '\0';
            write(fd1[1], buffer, STR_SIZE);
        }
    }

    return 0;
}
