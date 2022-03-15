#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct
{
    int id;
    long int regNum;
    char surname[31];
    char name[31];
    int mark;
}str;

int main(){
    str s;
    int fd1, id;
    char command;

    fd1=open("file_2", O_RDWR );

    while (1){
        printf("> ");
        fscanf(stdin, "%c%*c", &command);

        if(command=='E'){
            fprintf(stderr, "End program\n");
            return 0;
        }else{
            fscanf(stdin, "%d%*c", &id);
        }

        switch (command)
        {
        case 'R':
            lseek(fd1, (id-1)*sizeof(str), SEEK_SET);
            read(fd1, &s, sizeof(str));
            fprintf(stdout, "%d %ld %s %s %d\n", s.id, s.regNum, s.surname, s.name, s.mark);
            break;
        case 'W':
            lseek(fd1, (id-1)*sizeof(str), SEEK_SET);
            fscanf(stdin, "%d %ld %s %s %d%*c", &s.id, &s.regNum, s.surname, s.name, &s.mark);
            write(fd1, &s, sizeof(str));
            break;
        case 'E':
            fprintf(stderr, "End program\n");
            return 0;
            break;

        default:
            fprintf(stderr, "Error\n"); 
            return -1;
            break;
        }
    }

    return 0;
}