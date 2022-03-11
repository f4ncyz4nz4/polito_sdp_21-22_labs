#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int i=0;

void handler1(int sig){
    i++;
    if(i==0)
        printf("success\n");
    if(i==2 || i==-2)
        printf("insuccess\n");
    if(i==3 || i==-3)
        abort();  
}

void handler2(int sig){
    i--;
    if(i==0)
        printf("success\n");
    if(i==2 || i==-2)
        printf("insuccess\n");
    if(i==3 || i==-3)
        abort();   
}

int main(){
    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);
    printf("pid=%d\n", getpid());
    while(1){pause();}

    return 0;
}