#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFSIZE 1024

typedef struct
{
    int id;
    long int regNum;
    char surname[31];
    char name[31];
    int mark;
}str;

int main(int argc, char **argv){
    str s;
    FILE *fd1, *fd3;
    int fd2, nr=0;

    if(argc!=4){
        printf("Errore");
        exit(0);
    }

    fd1 = fopen(argv[1], "r");
    fd2 = open(argv[2], O_WRONLY | O_TRUNC);
    
    while((fscanf(fd1, "%d %ld %s %s %d", &s.id, &s.regNum, s.surname, s.name, &s.mark)) != EOF){
        write(fd2, &s, sizeof(str));
        nr++;
    }

    fclose(fd1);
    close(fd2);

    fd2 = open(argv[2], O_RDONLY);
    fd3 = fopen(argv[3], "w");
    while ((read(fd2, &s, sizeof(str))) > 0){
        fprintf(fd3, "%d %ld %s %s %d", s.id, s.regNum, s.surname, s.name, s.mark);
        if(nr>1){
            fprintf(fd3, "\n");
        }
        nr--;
    }
    
    close(fd2);
    fclose(fd3);

    return 0;
}