#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

char output_file_path[PATH_MAX];

//Clears the log file, then writes 10 lines to the dspawn log file
static int daemon_work() {
    // NOTE: Edit file directory accordingly 
    strcpy(output_file_path, "/home/spyabi/programming-assignment-1-2024-ci02-r-b/dspawn.log"); 

    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // Clear the log file at the beginning
    fptr = fopen(output_file_path, "w");
    if (fptr == NULL) {
        perror("Error opening log file");
        return EXIT_FAILURE;
    }
    fclose(fptr);

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL) {
        perror("Error opening log file");
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1) {
        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL) {
            perror("Error opening log file");
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int main() {
    // First fork
    pid_t pid = fork();
    if (pid == 0) {
        printf("Intermediate process with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));
        
        // Child process becomes the session leader
        setsid();
        printf("Intermediate process has setsid with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));

        // Ignore SIGCHLD and SIGHUP
        signal(SIGCHLD, SIG_IGN); // Prevent zombie processes
        signal(SIGHUP, SIG_IGN);  // Prevent termination of daemon

        // Second fork
        pid_t pid2 = fork();
        if (pid2 == 0) {
            printf("Daemon process with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));
            // Init daemon

            umask(0); // Ensure daemon's files have max perms

            chdir("/"); // Ensure daemon not tied to any filesystem

            /* Close all open file descriptors */
            int x;
            for (x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
                close(x);
            } 

            int fd0, fd1, fd2;

            /* Attach fds 0,1,2 to /dev/null. */ 
            fd0 = open("/dev/null", O_RDWR);
            fd1 = dup(0);
            fd2 = dup(0);  

            // Execute daemon work
            int result = daemon_work();

            // Exit with appropriate status
            exit(result);
            
        } else {
            // Child process exits
            exit(0);
        }
    } else if (pid > 0) {
        // This is the parent process
        printf("Parent process with pid %d, pgid %d, session id :%d\n", getpid(), getpgid(getpid()), getsid(getpid()));
        
        // Wait for the intermediate process to exit
        wait(NULL);
    } else {
        // Fork failed
        perror("Fork failed");
        return EXIT_FAILURE;
    }

    return 0;
}
