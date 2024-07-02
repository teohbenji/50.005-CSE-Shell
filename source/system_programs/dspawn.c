#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


int main(){
   pid_t pid = fork();
   if (pid == 0){
       printf("Child process with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));
       setsid(); // child tries setsid
       printf("Child process has setsid with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));

   }
   else{
     printf("Parent process with pid %d, pgid %d, session id :%d\n", getpid(), getpgid(getpid()), getsid(getpid()));
       setsid(); // parent tries setsid
       printf("Parent process has setsid with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));
       wait(NULL);

   }
   return 0;
}