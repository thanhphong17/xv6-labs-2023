#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argumentCount, char * argumentValues[]) {

  //int status;
  if (argumentCount != 1) {
    printf("error!\n");
    exit(1);
  }
  else {
   char message[] = "a";
   int parentToChildPipe[2], childToParentPipe[2]; 
   if (pipe(parentToChildPipe) == -1) {
    printf("error creating pipe from parent to child\n");
    exit(1);
   }
   if (pipe(childToParentPipe) == -1) {
    printf("error creating pipe from child to parent\n");
    exit(1);
   }
   int processID = fork();
   if (processID < 0) {
    printf("error fork");
    exit(1);
   }
   else if (processID == 0) { //child process
    close(parentToChildPipe[1]);
    close(childToParentPipe[0]);
    //printf("%d", wait(0));
    if (read(parentToChildPipe[0], message, 1) == 1) {
      printf("%d: received ping\n", getpid());
      write(childToParentPipe[1], message, 1);
    }
    else {
      printf("receiving error!");
      exit(0);
    }
    exit(0);
   } 
   else {  //parent process (processID > 0)
      
      close(parentToChildPipe[0]);
      close(childToParentPipe[1]);
      write(parentToChildPipe[1], message, 1);
      wait(0);
      if(read(childToParentPipe[0], message, 1) == 1){
        printf("%d: received pong\n", getpid());
      }
      else {
        printf("receiving error parent.");
        exit(1);
      }
   }
   exit(0);
 }
}
