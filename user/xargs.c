#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argumentCount, char *argumentValues[]) {
    if (argumentCount < 2) {
        printf("custom_xargs : minimum number of arguments is 2!\n");
        exit(1);
    }
    if (argumentCount - 1 >= MAXARG) {
        printf("custom_xargs : maximum number of arguments is %d!\n", MAXARG);
    }
    char buffer[512], *commandArguments[MAXARG];
    for (int i = 1; i < argumentCount; i++)
        commandArguments[i-1] = argumentValues[i];
    while (1) {
        int index = argumentCount - 1;
        gets(buffer, 64);
        if (buffer[0] == 0)
            break;
        commandArguments[index] = buffer;
        index++;
        for (char *ptr = buffer; *ptr; ptr++) {
            if (*ptr == ' ') {
                *ptr = 0;
                commandArguments[index] = ptr + 1;
                index++;
            } else if (*ptr == '\n') {
                *ptr = 0;
            }
        }
        int processId = fork();
        if (processId == 0) {
            exec(argumentValues[1], commandArguments);
        } else if (processId < 0) {
            printf("fork error!\n");
            exit(1);
        } else {
            wait(0);
        }
    }
    wait(0);
    exit(0);
}
