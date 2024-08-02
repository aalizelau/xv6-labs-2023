#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
filter(int left[2]) {
    int prime;
    int n;
    close(left[1]); // Close write end of left pipe
    if (read(left[0], &prime, sizeof(prime)) == 0) {
        exit(0);
    }
    printf("prime %d\n", prime);

    int right[2];
    pipe(right);

    if (fork() == 0) {
        filter(right);
    } else {
        close(right[0]); // Close read end of right pipe
        while (read(left[0], &n, sizeof(n))) {
            if (n % prime != 0) {
                write(right[1], &n, sizeof(n));
            }
        }
        close(right[1]); // Close write end of right pipe
        wait(0);
    }
    close(left[0]); // Close read end of left pipe
    exit(0);
}

int main(int argc, char *argv[]) 
{
    int left[2];
    pipe(left);

    if (fork() == 0) {
        filter(left);
    } else {
        close(left[0]); // Close read end of left pipe
        for (int i = 2; i <= 35; i++) {
            write(left[1], &i, sizeof(i));
        }
        close(left[1]); // Close write end of left pipe
        wait(0);
    }
    exit(0);
}