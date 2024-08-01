#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pipe1[2];
    int pipe2[2];
    
    pipe(pipe1);
    pipe(pipe2);
    
    int pid = fork();
    
    if (pid == 0)
    {
        char buffer;
        close (pipe1[1]);
        close (pipe2[0]);

        read(pipe1[0], &buffer, 1);
        fprintf(1, "%d: received ping\n",getpid());
        write(pipe2[1], &buffer,1);
        exit(0);
    }
    else
    {
        char buffer = 'A';
        close (pipe1[0]);
        close (pipe2[1]);

        write(pipe1[1], &buffer, 1);
        read(pipe2[0],&buffer,1);
        fprintf(1, "%d: received pong\n",getpid());
        exit(0);
    }
}