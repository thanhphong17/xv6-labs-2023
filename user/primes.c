#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void spawnNewProcess(int pipeDescriptor[2]) {
    int primeNumber;
    int currentNumber;
    close(pipeDescriptor[1]);
    read(pipeDescriptor[0], &primeNumber, 4);
    printf("prime %d\n", primeNumber);

    if (read(pipeDescriptor[0], &currentNumber, 4)) {
        int newPipeDescriptor[2];
        pipe(newPipeDescriptor);
        if (currentNumber % primeNumber != 0) write(newPipeDescriptor[1], &currentNumber, 4);

        while(read(pipeDescriptor[0], &currentNumber, 4)){
            if(currentNumber % primeNumber != 0) write(newPipeDescriptor[1], &currentNumber, 4);
        }
        close(pipeDescriptor[0]);

        if (fork() == 0) {
            spawnNewProcess(newPipeDescriptor);
        } 
        else {
            close(newPipeDescriptor[0]);
            close(newPipeDescriptor[1]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char const *argv[])
{
    int pipeDescriptor[2];
    pipe(pipeDescriptor);
    if (fork() == 0) {
        spawnNewProcess(pipeDescriptor);
    }
    else {
        close(pipeDescriptor[0]);
        for(int i = 2; i <= 35; i++) {
            write(pipeDescriptor[1], &i, 4);
        }
        close(pipeDescriptor[1]);
        wait(0);
        exit(0);
    }
    return 0;
}
