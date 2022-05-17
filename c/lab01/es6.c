#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pre_prec_sig = 0;
int prec_sig = 0;

void handler(int sig) {
    if (prec_sig == sig && pre_prec_sig == sig) {
        exit(0);
    }
    if (prec_sig != sig && prec_sig != 0) {
        printf("success\n");
    }
    if (prec_sig == sig) {
        printf("insuccess\n");
    }
    pre_prec_sig = prec_sig;
    prec_sig = sig;
}

int main() {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    printf("pid=%d\n", getpid());
    while (1) {
        pause();
    }

    return 0;
}