#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int fd1, ind, *arr;
    FILE *fd;

    fd = fopen("file.txt", "r");
    fd1 = open("file1", O_WRONLY | O_CREAT);

    fscanf(fd, "%d", &ind);

    arr = (int *)malloc((ind + 1) * sizeof(int));
    arr[0] = ind;

    for (int i = 1; i <= ind; i++) {
        fscanf(fd, "%d", &arr[i]);
    }
    write(fd1, arr, (ind + 1) * sizeof(int));

    return 0;
}
