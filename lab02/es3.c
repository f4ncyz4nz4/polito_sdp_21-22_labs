#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 512
#define PATH 300

int bpb_write(int fd, char *buf, int size) {
    for (int i = 0; i < size; i++) {
        if (buf[i] == 32) {
            buf[i] = ' ';
        }
        if (buf[i] == 10) {
            buf[i] = '\n';
        }
    }
    write(fd, buf, size);
    return 0;
}

void cp_function(char *p_dir1, char *p_dir2) {
    DIR *dp;
    struct dirent *dirp;
    struct stat sb;
    int fd1, fd2;
    char buf[BUFSIZE], head[PATH], file_s[PATH], file_d[PATH], dir_s[PATH],
        dir_d[PATH];

    dp = opendir(p_dir1);

    while ((dirp = readdir(dp)) != NULL) {
        if ((strcmp(".", dirp->d_name) != 0) &&
            (strcmp("..", dirp->d_name) != 0)) {
            strcpy(file_s, p_dir1);
            strcat(file_s, dirp->d_name);
            strcpy(file_d, p_dir2);
            strcat(file_d, dirp->d_name);

            stat(file_s, &sb);

            if (S_ISDIR(sb.st_mode)) {
                strcpy(dir_s, file_s);
                strcat(dir_s, "/");
                strcpy(dir_d, file_d);
                strcat(dir_d, "/");

                if (mkdir(dir_d, O_RDWR) != 0) {
                    fprintf(stderr, "Error in creating folder\n");
                    abort();
                }

                chmod(dir_d, S_IRUSR | S_IXUSR | S_IWUSR);
                cp_function(dir_s, dir_d);
            } else {
                sprintf(head, "%s %ld\n", dirp->d_name, sb.st_size);

                if ((fd1 = open(file_s, O_RDONLY)) == -1) {
                    fprintf(stderr, "Error in opening fd1\n");
                    abort();
                }

                if ((fd2 = open(file_d, O_WRONLY | O_CREAT)) == -1) {
                    fprintf(stderr, "Error in opening fd2\n");
                    abort();
                }

                chmod(file_d, S_IRUSR | S_IXUSR | S_IWUSR);

                write(fd2, head, PATH);
                while (read(fd1, buf, BUFSIZE) > 0) {
                    bpb_write(fd2, buf, BUFSIZE);
                }
            }
        }
    }

    closedir(dp);
}

int main(int argc, char **argv) {
    struct stat sb1, sb2;

    if (argc != 3) {
        printf("Errore");
        abort();
    }

    stat(argv[1], &sb1);
    stat(argv[2], &sb2);

    if (!S_ISDIR(sb1.st_mode) || !S_ISDIR(sb2.st_mode)) {
        printf("Errore");
        abort();
    }

    cp_function(argv[1], argv[2]);

    return 0;
}