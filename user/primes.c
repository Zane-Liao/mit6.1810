#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PRIME 35
#define READEND 0
#define WRIETEND 1

void
child(int *pl)
{
    int pr[2];
    int n;

    close(pl[WRIETEND]);
    int read_result = read(pl[READEND], &n, sizeof(int));
    if(read_result == 0)
        exit(0);
    pipe(pr);
    // close(pl[READEND]);

    if(fork() == 0){
        child(pr);
    }else{
        close(pr[READEND]);
        printf("prime %d\n", n);
        int prime = n;
        while(read(pl[READEND], &n, sizeof(int)) != 0){
            if(n%prime != 0){
                write(pr[WRIETEND], &n, sizeof(int));
            }
        }
        close(pr[WRIETEND]);
        wait((int *) 0);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if(fork() == 0){
        child(p);
    }else{
        close(p[READEND]);
        for(int i=2; i<PRIME+1; i++){
            write(p[WRIETEND], &i, sizeof(int));
        }
        close(p[WRIETEND]);
        wait((int *) 0);
    }
    exit(0);
}